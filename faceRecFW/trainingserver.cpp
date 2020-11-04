#include <QDir>

#include <opencv2/contrib/contrib.hpp>

#include "trainingserver.h"
#include "gaborfeature.h"
#include "pcautils.h"
#include "fisherldautils.h"

TrainingServer::TrainingServer(QObject *parent) :
    QThread(parent)
{
    sample_per_class = SAMPLE_PER_CLASS;
    gabor_feature_len = 10240;
    pca_feature_len_reference = PCA_FEATURE_LEN_REF;
    image_len = 128;
}

void TrainingServer::run()
{
    int progress_value = 0;
    QString output_line;

    if (!db_manager.db_connect()) {
        fprintf(stderr, "Error: DataBase Connection!\n");
        exit(EXIT_FAILURE);
    }

    output_line = QObject::tr("training start....");
    emit updateGUI(output_line, 0);

    sample_class_number = db_manager.get_person_info_count();
    sample_total_number = sample_class_number * sample_per_class;

    int progress_max = 2 * sample_total_number;
    emit updateRange(0, progress_max);

    /*--------Print Training Information--------*/

    std::vector<ImageContent> image_content_list;
    db_manager.get_image_content_list(image_content_list);

    if (image_content_list.empty()) {
        emit updateRange(0, 1);
        output_line = QObject::tr("no image exist in table, quit training");
        emit updateGUI(output_line, 1);
    } else {
        output_line = QObject::tr("sample per class: %1, sample class number: %2")
                .arg(sample_per_class).arg(sample_class_number);
        emit updateGUI(output_line, progress_value);

        /*--------Training Buffer Allocation--------*/

        std::vector<cv::Mat> gabor_feature_list;
        for (int i = 0; i < sample_total_number; i++) {
            cv::Mat gabor_feature;
            gabor_feature.create(gabor_feature_len, 1, CV_64FC1);
            gabor_feature_list.push_back(gabor_feature);
        }

        int flda_feature_len;

        std::vector<cv::Mat> pca_feature_list;
        std::vector<cv::Mat> flda_feature_list;

        cv::Mat pca_matrix_mean;
        cv::Mat pca_transfer_matrix;
        cv::Mat flda_matrix_mean;
        cv::Mat flda_transfer_matrix;
        cv::Mat total_transfer_matrix;
        /*--------Training Progress Started--------*/

        /*--------Extraction Gabor Features--------*/
        for (size_t i = 0; i < image_content_list.size(); i++) {
            int     image_id = image_content_list[i].image_id;
            QString image_filename = image_content_list[i].image_path;

            if (db_manager.has_feature_data(image_id)) {

                db_manager.load_gabor_feature_from_blob(gabor_feature_list[i], image_id);

                output_line = QObject::tr("load gabor feature from image: %1, path: %2")
                        .arg(image_id).arg(image_filename);
                emit updateGUI(output_line, ++progress_value);

            } else {
                cv::Mat face_image;

                face_image = read_gray_image(image_filename.toAscii().data());

                gabor_feature_list[i] =
                        gabor_utils::gabor_feature(face_image);

                output_line = QObject::tr("extract gabor feature for image: %1, path: %2")
                        .arg(image_id).arg(image_filename);
                emit updateGUI(output_line, ++progress_value);

                db_manager.save_gabor_feature_to_blob(gabor_feature_list[i], image_id);
            }
        }
        /*--------End of Extraction Gabor Features--------*/

        output_line = QObject::tr("start pca transfer ....");
        emit updateGUI(output_line, progress_value);

        pca_utils::pca_transfer(gabor_feature_list,
                                pca_transfer_matrix,
                                pca_feature_list,
                                pca_matrix_mean,
                                pca_feature_len_reference);

        /*--------Fisher LDA--------*/
        output_line = QObject::tr("start fisher LDA transfer ....");
        emit updateGUI(output_line, progress_value);

        fisher_lda_utils::flda_transfer(pca_feature_list,
                                        flda_feature_list,
                                        flda_transfer_matrix,
                                        flda_matrix_mean,
                                        sample_class_number,
                                        sample_per_class,
                                        pca_feature_len_reference);

        /*--------End of Fisher LDA--------*/
        flda_feature_len = flda_transfer_matrix.rows;
        /*--------Store Fisher LDA feature--------*/
        for (size_t i = 0; i < flda_feature_list.size(); i++) {
            db_manager.save_flda_feature_to_bolb(flda_feature_list[i],
                                                 image_content_list[i].image_id);

            output_line = QObject::tr("update fisher LDA feature for image: %1")
                    .arg(image_content_list[i].image_id);
            emit updateGUI(output_line, ++progress_value);
        }
        /*--------End of Storage of Fisher LDA feature--------*/

        total_transfer_matrix =
                flda_transfer_matrix * pca_transfer_matrix;

        db_manager.save_mapping_matrix(pca_transfer_matrix,
                                       flda_transfer_matrix,
                                       total_transfer_matrix,
                                       flda_feature_len,
                                       pca_matrix_mean);

        output_line = QObject::tr("End of Training Process...\nHave Fun!");
        emit updateGUI(output_line, progress_value);
    }

    db_manager.db_disconnect();
    emit trainingComplete();
}
