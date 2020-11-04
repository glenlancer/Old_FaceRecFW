#include "pcautils.h"

/* PCA algorithm */
/* Input data: sample_set, output_dims; Output data: pca_matrix, output_set, matrix_mean */
void pca_utils::pca_transfer(std::vector<cv::Mat> &sample_set, cv::Mat &pca_transfer_matrix,
                             std::vector<cv::Mat> &output_set, cv::Mat &mean_matrix,
                             int output_dims_reference)
{
    cv::Mat pca_data;
    cv::Mat eigen_vector_matrix;

    int output_dims = (sample_set.size() > output_dims_reference)
                          ? output_dims_reference : sample_set.size();

    create_pca_sample(pca_data, sample_set);

    /* initial PCA object */
    cv::PCA Pca(pca_data, cv::Mat(), CV_PCA_DATA_AS_COL, output_dims);

    /* calculate mean matrix of sample_set */
    mean_sample_set(sample_set, mean_matrix);

    /* get pca transfer matrix */
    eigen_vector_matrix = Pca.eigenvectors;
    create_pca_transfer_matrix(pca_transfer_matrix, eigen_vector_matrix, output_dims);

    /* project all gabor feature into PCA Subspace */
    for (int i = 0; i < sample_set.size(); i++) {
        cv::Mat pca_feature;
        pca_feature = Pca.project(sample_set[i]);
        output_set.push_back(pca_feature);
    }

    /*-------End of PCA Processing-------*/
}

void pca_utils::create_pca_transfer_matrix(cv::Mat &pca_transfer_matrix,
                                           cv::Mat &eigen_vector_matrix, int output_dims)
{
    pca_transfer_matrix.create(output_dims,
                               eigen_vector_matrix.cols,
                               eigen_vector_matrix.type());

    for (int y = 0; y < output_dims; y++)
    {
        for (int x = 0; x < eigen_vector_matrix.cols; x++)
        {
            pca_transfer_matrix.at<double>(y, x) =
                    eigen_vector_matrix.at<double>(y, x);
        }
    }
}

void pca_utils::create_pca_sample(cv::Mat &pca_data, std::vector<cv::Mat> &sample_set)
{
    pca_data.create(sample_set[0].rows, sample_set.size(), sample_set[0].type());

    for (int x = 0; x < sample_set.size(); x++)
    {
        for (int y = 0; y < sample_set[0].rows; y++)
        {
            pca_data.at<double>(y, x) =
                    sample_set[x].at<double>(y, 0);
        }
    }
}

void pca_utils::mean_sample_set(std::vector<cv::Mat> &sample_set, cv::Mat &matrix_mean)
{
    assert(!sample_set.empty());

    int i, x, y;
    int sample_size = sample_set.size();

    matrix_mean.create(sample_set[0].rows, sample_set[0].cols, sample_set[0].type());
    matrix_mean.setTo(cv::Scalar(0.0));

    for (i = 0; i < sample_size; i++) {
        cv::add(matrix_mean, sample_set[i], matrix_mean);
    }

    for (x = 0; x < matrix_mean.cols; x++) {
        for (y = 0; y < matrix_mean.rows; y++) {
            matrix_mean.at<double>(y, x) =
                    matrix_mean.at<double>(y, x) / sample_size;
        }
    }
}
