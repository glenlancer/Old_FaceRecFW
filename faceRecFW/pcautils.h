#ifndef PCAUTILS_H
#define PCAUTILS_H

#include "common.h"

namespace pca_utils {
    void create_pca_transfer_matrix(cv::Mat &pca_transfer_matrix,
                                    cv::Mat &eigen_vector_matrix, int output_dims);

    void create_pca_sample(cv::Mat &pca_data, std::vector<cv::Mat> &sample_set);

    void pca_transfer(std::vector<cv::Mat> &sample_set, cv::Mat &pca_transfer_matrix,
                      std::vector<cv::Mat> &output_set, cv::Mat &mean_matrix,
                      int output_dims_reference);

    void mean_sample_set(std::vector<cv::Mat> &sample_set, cv::Mat &matrix_mean);
}

#endif // PCAUTILS_H
