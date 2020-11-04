/********************************************************************
 * Copyright © Lin Guannan. 2011-2012. All rights reserved.
 *
 * File name:     gaborfeature.cpp
 * First Creator: Lin Guannan
 * Data:          2012-7-8
 * Description:   source file for extracting gabor features from norm face
 *
 * Others:
 * History:
 *     1. Data:            2012-7-8
 *        Author:          Lin Guannan
 *        Modification:    first creation of the file
 *******************************************************************/

#include "gaborfeature.h"

int gabor_utils::local_pow(int a, int b)
{
    int result = 1;
    while (b--) result *= a;
    return result;
}

cv::Mat gabor_utils::down_sampling(cv::Mat &src, int jump_param)
{
    cv::Mat dst;
    int step = local_pow(2, jump_param);

    dst.create(src.rows / step, src.cols / step, CV_64FC1);
    for (int x = 0; x < src.cols; x++)
    {
        if (x % step == 0)
        {
            for (int y = 0; y < src.rows; y++)
            {
                if (y % step == 0)
                {
                    dst.at<double>(y / step, x / step)
                            = src.at<double>(y, x);
                }
            }
        }
    }

    return dst;
}

cv::Mat gabor_utils::gabor_conv(cv::Mat &src, cv::Mat &gabor_re, cv::Mat &gabor_im)
{
    cv::Mat dst_re;
    cv::Mat dst_im;
    cv::Mat dst_mod;

    assert(src.cols == src.rows);

    int img_size = src.cols;

    dst_re.create(img_size, img_size, CV_64FC1);
    dst_im.create(img_size, img_size, CV_64FC1);
    dst_mod.create(img_size, img_size, CV_64FC1);

    /* convolute the image with the real part of the Gabor wavelet */
    cv::filter2D(src, dst_re, dst_re.depth(), gabor_re, cv::Point(img_size / 2,
                                                                  img_size / 2));

    /* convolute the image with the imaginary part of the Gabor wavelet */
    cv::filter2D(src, dst_im, dst_im.depth(), gabor_im, cv::Point(img_size / 2,
                                                                  img_size / 2));

    for (int y = 0; y < img_size; y++) {
        for (int x = 0; x < img_size; x++) {
            double temp_re = dst_re.at<double>(y, x);
            double temp_im = dst_im.at<double>(y, x);
            double temp_mod = sqrt(temp_re * temp_re + temp_im * temp_im);
            dst_mod.at<double>(y, x) = temp_mod;
        }
    }

    return dst_mod;
}

void gabor_utils::gabor_base(int V, int U, int size, cv::Mat &gabor_re, cv::Mat &gabor_im)
{
    /* x for cols, y for rows */
    int x, y;
    int max_x =  size / 2;
    int min_x = -size / 2 + 1;
    int max_y =  max_x;
    int min_y =  min_x;
    int half_size = size / 2;

    assert(size % 2 == 0);

    double Kv = K_MAX / pow(F, V);
    double Phi_u = CV_PI * U / 8;
    double Kuv_re = Kv * cos(Phi_u);
    double Kuv_im = Kv * sin(Phi_u);

    double Kuv_mod_2 = pow(Kuv_re, 2) + pow(Kuv_im, 2);

    for (y = min_y; y <= max_y; y++) {
        for (x = min_x; x <= max_x; x++) {
            double pos_mod_2 = x * x + y * y;
            double angle = x * Kuv_re + y * Kuv_im;
            double delta = -Kuv_mod_2 * pos_mod_2 / (2 * SIGMA_2);

            double temp_re = (Kuv_mod_2 / SIGMA_2) * exp(delta) * (cos(angle) - exp(-SIGMA_2 / 2));
            double temp_im = (Kuv_mod_2 / SIGMA_2) * exp(delta) * sin(angle);

            gabor_re.at<double>(half_size - y, x + half_size - 1) = temp_re;
            gabor_im.at<double>(half_size - y, x + half_size - 1) = temp_im;
        }
    }
}

cv::Mat gabor_utils::gabor_feature(cv::Mat &src)
{
    int jump_param = JUMP_PARAM;
    assert(src.cols == src.rows);
    assert(src.cols > jump_param);

    cv::Mat target_image;
    cv::Mat gabor_re;
    cv::Mat gabor_im;
    cv::Mat gabor_mod;
    cv::Mat ds_image;

    target_image.create(src.rows, src.cols, CV_64FC1);
    gabor_re.create(src.rows, src.cols, CV_64FC1);
    gabor_im.create(src.rows, src.cols, CV_64FC1);

    target_image = src.clone();
    int target_size = src.rows * src.cols;
    int img_size = src.rows;
    int dst_size = target_size / local_pow(4, jump_param);
    int gabor_row = dst_size * V_MAX * U_MAX;
    cv::Mat gabor_feature;
    gabor_feature.create(gabor_row, 1, CV_64FC1);

    int index = 0;

    for (int V = 0; V < V_MAX; V++) {
        for (int U = 0; U < U_MAX; U++) {

            gabor_base(V, U, img_size, gabor_re, gabor_im);

            gabor_mod = gabor_conv(target_image, gabor_re, gabor_im);

            ds_image = down_sampling(gabor_mod, jump_param);

            norm_matrix_data(ds_image);

            int i = 0;
            for (int x = 0; x < ds_image.cols; x++) {
                for (int y = 0; y < ds_image.rows; y++) {
                    gabor_feature.at<double>(dst_size * index + i, 0) =
                            ds_image.at<double>(y, x);
                    i++;
                }
            }
            index++;
        }
    }

    return gabor_feature;
}

double gabor_utils::matrix_mean(cv::Mat &ds_image)
{
    double data_sum = 0;
    int size = ds_image.cols * ds_image.rows;

    for (int x = 0; x < ds_image.cols; x++) {
        for (int y = 0; y < ds_image.rows; y++) {
            data_sum += ds_image.at<double>(y, x);
        }
    }

    return data_sum / size;
}

double gabor_utils::matrix_dev(cv::Mat &ds_image)
{
    double data_dev = 0;
    double data_mean = 0;
    double data_temp = 0;

    int size = ds_image.cols * ds_image.rows;
    data_mean = matrix_mean(ds_image);

    for (int x = 0; x < ds_image.cols; x++) {
        for (int y = 0; y < ds_image.rows; y++) {
            data_temp = ds_image.at<double>(y, x) - data_mean;
            data_dev += data_temp * data_temp;
        }
    }

    return sqrt(data_dev / size);
}

void gabor_utils::norm_matrix_data(cv::Mat &ds_image)
{
    double data_mean = matrix_mean(ds_image);
    double data_dev = matrix_dev(ds_image);
    double temp = 0;

    for (int x = 0; x < ds_image.cols; x++) {
        for (int y = 0; y < ds_image.rows; y++) {
            temp = (ds_image.at<double>(y, x) - data_mean) / data_dev;
            ds_image.at<double>(y, x) = temp;
        }
    }
}
