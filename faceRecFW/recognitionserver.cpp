#include <QTime>
#include <QSplashScreen>

#include "recognitionserver.h"
#include "gaborfeature.h"
#include "pcautils.h"

RecognitionServer::RecognitionServer(QObject *parent) :
    QThread(parent)
{
    /*----create mapping matrix-----*/
    gabor_feature_len = 10240;

    mean_matrix.create(gabor_feature_len, 1, CV_64FC1);

    index_max = 0;
    index_current = 0;
    sample_per_class = SAMPLE_PER_CLASS;
}

void RecognitionServer::reloading_data()
{
    QMutexLocker locker(&mutex);
    db_manager.db_connect();
    db_manager.load_feature_len(flda_feature_len);
    mapping_matrix.release();
    mapping_matrix.create(flda_feature_len, gabor_feature_len, CV_64FC1);
    db_manager.load_general_mapping_matrix(mapping_matrix, mean_matrix);
    db_manager.load_feature_vectors(subclass_content, flda_feature_len);
    db_manager.db_disconnect();
}

void RecognitionServer::set_norm_face(cv::Mat &face)
{
    norm_face = face.clone();
}

void RecognitionServer::system_startup()
{
    db_manager.db_connect();
    QString display;

    cv::Mat start_up_frame = read_image("./resources/webcam_frame_default.jpg");
    QImage start_up_image = mat2qimage(start_up_frame);

    db_manager.load_feature_len(flda_feature_len);
    mapping_matrix.release();
    mapping_matrix.create(flda_feature_len, gabor_feature_len, CV_64FC1);

    /*-----load features from database-----*/
    QSplashScreen *splash = new QSplashScreen;
    Qt::Alignment rightBottom = Qt::AlignRight | Qt::AlignBottom;
    splash->setPixmap(QPixmap::fromImage(start_up_image));
    splash->show();

    display = QString("loading mapping matrix......");
    splash->showMessage(display, rightBottom, Qt::black);
    db_manager.load_general_mapping_matrix(mapping_matrix, mean_matrix);

    display = QString("loading feature vectors......");
    splash->showMessage(display, rightBottom, Qt::black);
    db_manager.load_feature_vectors(subclass_content, flda_feature_len);

    display = QString("system started");
    splash->showMessage(display, rightBottom, Qt::black);
    splash->close();
    delete splash;
    delete start_up_image.bits();

    db_manager.db_disconnect();
    /*-------------------------------------*/
}

void RecognitionServer::get_recognition_result(std::vector<ResultRecord> &result_set)
{
    QMutexLocker locker(&mutex);
    result_set = resultSet;
}

double RecognitionServer::get_threshold()
{
    QMutexLocker locker(&mutex);
    return threshold;
}

void RecognitionServer::run()
{
    QMutexLocker locker(&mutex);

    /* get recognition length */
    index_current = 0;
    index_max = subclass_content.size() * ( sample_per_class + 1 ) + 1;

    updateRange(0, index_max);

    /* get gabor feature */
    write_image("test.jpg", norm_face);
    cv::Mat gabor_feature = gabor_utils::gabor_feature(norm_face);
    updateGUI(++index_current);

    /* get flda feature */
    cv::subtract(gabor_feature, mean_matrix, gabor_feature);
    cv::Mat flda_feature = mapping_matrix * gabor_feature;

    /* get min distance for each subclass */
    for (size_t i = 0; i < subclass_content.size(); i++) {
        SubClass sub_class = subclass_content[i];

        if (!sub_class.feature_distances.empty()) {
            sub_class.feature_distances.clear();
        }

        for (size_t j = 0; j < sub_class.feature_list.size(); j++) {
            double distance =
                    get_euclid_distance(sub_class.feature_list[j],
                                        flda_feature);

            double rel =
                    get_relevance(sub_class.feature_list[j],
                                  flda_feature);

            sub_class.feature_distances.push_back(distance);
            sub_class.feature_rel.push_back(rel);
            updateGUI(++index_current);
        }

        min_per_subclass(sub_class);
        subclass_content[i] = sub_class;
    }

    /* sorting the recognition result */
    sort_result();

    /* calculate the relevance value */
    QString target_person = resultSet[0].employee_id;
    std::vector<cv::Mat> feature_list;

    for (size_t i = 0; i < subclass_content.size(); i++) {
        if (subclass_content[i].employee_id == target_person) {
            feature_list = subclass_content[i].feature_list;
            break;
        }
    }

    cv::Mat mean_face;
    pca_utils::mean_sample_set(feature_list, mean_face);

    threshold = get_relevance(mean_face, flda_feature);

    emit recognitionComplete();
}

void RecognitionServer::min_per_subclass(SubClass &subclass)
{
    double temp = subclass.feature_distances[0];
    double temp_1;

    for (size_t i = 1; i < subclass.feature_distances.size(); i++) {
        if (temp < subclass.feature_distances[i]) {
            temp = subclass.feature_distances[i];
            temp_1 = subclass.feature_rel[i];
        }
    }

    subclass.cos_distance = temp;
    subclass.rel = temp_1;
}

void RecognitionServer::sort_result()
{
    /* create result record vector */
    ResultRecord result_record;

    if (!resultSet.empty()) {
        resultSet.clear();
    }

    for (size_t i = 0; i < subclass_content.size(); i++) {
        result_record.employee_id = subclass_content[i].employee_id;
        result_record.distance = subclass_content[i].cos_distance;
        result_record.rel = subclass_content[i].rel;
        resultSet.push_back(result_record);
        updateGUI(++index_current);
    }

    /* simple bubble sort algorithm */
    for (int limit = resultSet.size() - 1; limit > 0; limit--) {
        bool swapped = false;

        for (int i = 0; i < limit; i++)
        {
            if (resultSet[i].distance > resultSet[i + 1].distance)
            {
                result_record = resultSet[i];
                resultSet[i] = resultSet[i + 1];
                resultSet[i + 1] = result_record;
                swapped = true;
            }
        }

        if (!swapped)
            break; /* if no swaps, finish sorting */
    }
}

double RecognitionServer::get_euclid_distance(cv::Mat &mat_X,
                                              cv::Mat &mat_Y)
{
    assert(mat_X.rows == mat_Y.rows &&
           mat_X.cols == mat_Y.cols);

    int rows = mat_X.rows;
    int cols = mat_Y.cols;

    double sum = 0.0;

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            double x_i = mat_X.at<double>(y, x);
            double y_i = mat_Y.at<double>(y, x);

            sum += (x_i - y_i) * (x_i - y_i);
        }
    }

    return sqrt(sum);
}

/* X = [x1, x2, ..., xm]; Y = [y1, y2, ..., ym]   */
/*                     |x1*y1+x2*y2+...+xm*ym|    */
/*      S(X, Y) = ------------------------------- */
/*                (x1^2+...+xm^2)*(y1^2+...+ym^2) */
double RecognitionServer::get_relevance(cv::Mat &mat_X, cv::Mat &mat_Y)
{
    assert(mat_X.rows == mat_Y.rows &&
           mat_X.cols == mat_Y.cols);

    int rows = mat_X.rows;
    int cols = mat_Y.cols;

    double sum_xy = 0.0;
    double sum_xx = 0.0;
    double sum_yy = 0.0;

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            double x_i = mat_X.at<double>(y, x);
            double y_i = mat_Y.at<double>(y, x);

            sum_xy += x_i * y_i;
            sum_xx += x_i * x_i;
            sum_yy += y_i * y_i;
        }
    }

    return abs(sum_xy) / (sqrt(sum_xx) * sqrt(sum_yy));
}
