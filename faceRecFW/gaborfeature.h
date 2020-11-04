/********************************************************************
 * Copyright © Lin Guannan. 2011-2012. All rights reserved.
 *
 * File name:     gaborfeature.cpp
 * First Creator: Lin Guannan
 * Data:          2012-7-8
 * Description:   header file for extracting gabor features from norm face
 *                (1) all in namespace GaborUtils
 *                (2) this file is essential to the program
 *                (3) input image size 128x128,CV_8UC1
 *                (4) output matrix size 10240x1, CV_64FC1
 * Others:
 * History:
 *     1. Data:            2012-7-8
 *        Author:          Lin Guannan
 *        Modification:    first creation of the file
 *******************************************************************/

#ifndef GABORFEATURE_H
#define GABORFEATURE_H

#include "common.h"

/* macros used in Gabor processing */
#define V_MAX 5
#define U_MAX 8
#define JUMP_PARAM 3

#define K_MAX    CV_PI / 2
#define F        sqrt(2)
#define SIGMA    2 * CV_PI
#define SIGMA_2  pow(2 * CV_PI, 2)

namespace gabor_utils {
    /* some functions can be used by other modules */
    /* move to common.cpp later? */
    int local_pow(int a, int b);
    double matrix_dev(cv::Mat &ds_image);
    double matrix_mean(cv::Mat &ds_image);

    /* four functions should be only used in this module */
    /* remove from namespace GaborUtils later? */
    void gabor_base(int V, int U, int size, cv::Mat &gabor_re, cv::Mat &gabor_im);
    cv::Mat gabor_conv(cv::Mat &src, cv::Mat &gabor_re, cv::Mat &gabor_im);
    cv::Mat down_sampling(cv::Mat &src, int jump_param);
    void norm_matrix_data(cv::Mat &ds_image);

    /* the main process function, the return matrix is acturally a vector */
    /* with size 10240x1 */
    cv::Mat gabor_feature(cv::Mat &src);
}

#endif // GABORFEATURE_H
