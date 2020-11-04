#ifndef FISHERLDAUTILS_H
#define FISHERLDAUTILS_H

#include "common.h"

namespace fisher_lda_utils {
    void flda_transfer(std::vector<cv::Mat> &pca_feature_list,
                       std::vector<cv::Mat> &flda_feature_list,
                       cv::Mat &flda_transfer_matrix,
                       cv::Mat &flda_mean_matrix,
                       int sample_class_number,
                       int sample_per_class,
                       int dim_ref);
}

#endif // FISHERLDAUTILS_H
