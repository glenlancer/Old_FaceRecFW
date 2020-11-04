#include "fisherldautils.h"
#include "pcautils.h"

void fisher_lda_utils::flda_transfer(std::vector<cv::Mat> &pca_feature_list,
                                     std::vector<cv::Mat> &flda_feature_list,
                                     cv::Mat &flda_transfer_matrix,
                                     cv::Mat &flda_mean_matrix,
                                     int sample_class_number, int sample_per_class,
                                     int dim_ref)
{
    int rows = pca_feature_list[0].rows;
    int cols = pca_feature_list[0].cols;

    std::vector<cv::Mat> mk_matrix;
    std::vector<cv::Mat> class_storage;
    cv::Mat temp_matrix_1;
    cv::Mat temp_matrix_2;
    cv::Mat temp_matrix_3;
    cv::Mat Sb;
    cv::Mat Sw;

    for (int i = 0; i < sample_per_class; i++) {
        cv::Mat local_matrix;
        local_matrix.create(rows, cols, CV_64FC1);
        class_storage.push_back(local_matrix);
    }

    /* M1, M2, ..., Mk */
    for (int i = 0; i < sample_class_number; i++) {
        cv::Mat Mk;

        for (int j = 0; j < sample_per_class; j++) {
            pca_feature_list[ (i * sample_per_class) + j ]
                    .copyTo(class_storage[j]);
        }

        pca_utils::mean_sample_set(class_storage, Mk);

        mk_matrix.push_back(Mk);
    }

    /* M = (M1 + M2 + ... + Mk) / sample_class_number */
    pca_utils::mean_sample_set(mk_matrix, flda_mean_matrix);

    /* Sb */
    Sb.create(rows, rows, CV_64FC1);
    Sb.setTo(cv::Scalar(0.0));

    for (int i = 0; i < sample_class_number; i++) {
        cv::subtract(mk_matrix[i], flda_mean_matrix, temp_matrix_1);
        cv::transpose(temp_matrix_1, temp_matrix_2);
        temp_matrix_3 = temp_matrix_1 * temp_matrix_2;

        for (int x = 0; x < temp_matrix_3.cols; x++) {
            for (int y = 0; y < temp_matrix_3.rows; y++) {
                temp_matrix_3.at<double>(y, x) =
                        temp_matrix_3.at<double>(y, x) / sample_class_number;
            }
        }

        cv::add(Sb, temp_matrix_3, Sb);
    }

    /* Sw */
    Sw.create(rows, rows, CV_64FC1);
    Sw.setTo(cv::Scalar(0.0));

    for (int i = 0; i < sample_class_number; i++) {
        for (int j = 0; j < sample_per_class; j++) {
            cv::subtract(pca_feature_list[ (i * sample_per_class) + j ],
                         mk_matrix[i],
                         temp_matrix_1);

            cv::transpose(temp_matrix_1, temp_matrix_2);

            temp_matrix_3 = temp_matrix_1 * temp_matrix_2;

            cv::add(Sw, temp_matrix_3, Sw);
        }
    }

    for (int x = 0; x < Sw.cols; x++) {
        for (int y = 0; y < Sw.rows; y++) {
            Sw.at<double>(y, x) =
                    Sw.at<double>(y, x) / (sample_class_number * sample_per_class);
        }
    }

    cv::Mat Sw_inverse;
    cv::invert(Sw, Sw_inverse, cv::DECOMP_SVD);
    cv::Mat Left_Mat = Sw_inverse * Sb;

    cv::Mat eigen_value_vector;
    cv::Mat eigen_vector_matrix;

    cv::eigen(Left_Mat, eigen_value_vector, eigen_vector_matrix);

    std::vector<FldaElement> flda_seq;
    for (int i = 0; i < eigen_value_vector.rows; i++) {
        FldaElement elem;
        elem.eigen_value = eigen_value_vector.at<double>(i, 0);
        elem.index = i;
        flda_seq.push_back(elem);
    }

    /*-----------------*/
    FldaElement temp_elem;
    for (int limit = flda_seq.size() - 1; limit > 0; limit--) {
        bool swapped = false;

        for (int i = 0; i < limit; i++)
        {
            if (flda_seq[i].eigen_value <= flda_seq[i + 1].eigen_value)
            {
                temp_elem = flda_seq[i];
                flda_seq[i] = flda_seq[i + 1];
                flda_seq[i + 1] = temp_elem;
                swapped = true;
            }
        }

        if (!swapped)
            break; /* if no swaps, finish sorting */
    }
    /*-----------------*/

    int range = (sample_class_number < dim_ref) ? sample_class_number : dim_ref;

    flda_transfer_matrix.create(eigen_vector_matrix.rows, range, CV_64FC1);

    std::cerr << eigen_vector_matrix.type() << std::endl;
    std::cerr << CV_64FC1 << std::endl;

    for (int x = 0; x < range; x++) {

        for (int y = 0; y < eigen_vector_matrix.rows; y++) {
            int col_num = flda_seq[x].index;
            flda_transfer_matrix.at<double>(y, x) = eigen_vector_matrix.at<double>(y, col_num);

        }
    }

    cv::transpose(flda_transfer_matrix, flda_transfer_matrix);

    for (size_t i = 0; i < pca_feature_list.size(); i++) {
        cv::Mat flda_feature;

        flda_feature = flda_transfer_matrix * pca_feature_list[i];
        flda_feature_list.push_back(flda_feature);
    }
}
