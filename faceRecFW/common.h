/********************************************************************
 * Copyright © Lin Guannan. 2011-2012. All rights reserved.
 *
 * File name:     common.h
 * First Creator: Lin Guannan
 * Data:          2012-5-19
 * Description:   header file for commonly used functions
 *                (1) opencv and c/c++ include files
 *                (2) some macros may be used globally
 *                (3) frame data transfer and some other tool-functions
 * Others:
 * History:
 *     1. Data:            2012-5-20
 *        Author:          Lin Guannan
 *        Modification:    first creation of the file
 *     2. Data:            2012-6-29
 *        Author:          Lin Guannan
 *        Modification:    improve comments and coding style
 *******************************************************************/
#ifndef COMMON_H
#define COMMON_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>

#include <iostream>

#include <QImage>

#define __DEBUG
//#undef  __DEBUG

#ifdef __DEBUG
    #define DEBUG_ERR(PRINT_FLAG) perror(PRINT_FLAG "\n")
    #define DEBUG(PRINT_FLAG)     fprintf(stderr, PRINT_FLAG "\n")
#endif /* __DEBUG */

/* global macro definition */
#define MAX_DEVICE_NUM        10

#define SAMPLE_PER_CLASS      10

#define PCA_FEATURE_LEN_REF   80

/* struct definition */
typedef struct {
    int     image_id;
    QString image_path;
} ImageContent;

typedef struct {
    QString employee_id;
    std::vector<cv::Mat> feature_list;
    std::vector<double> feature_distances;
    std::vector<double> feature_rel;
    double cos_distance;
    double rel;
} SubClass;

typedef struct {
    QString employee_id;
    double  distance;
    double  rel;
} ResultRecord;

typedef struct {
    double eigen_value;
    int    index;
} FldaElement;

/* frame data transfer and I/O functions for cv::Mat */
QImage  mat2qimage(const cv::Mat &src);
cv::Mat qimage2mat(const QImage &original_image);
cv::Mat rawdata2mat(unsigned char *data, int rows, int cols, int type);
cv::Mat get_image_roi(cv::Mat &src, cv::Rect &r);

cv::Mat read_gray_image(char *filename);
cv::Mat read_image(char *filename);
void write_image(char *filename, cv::Mat result);

/* common functions used in image processing */
int local_pow(int a, int b);

#endif // COMMON_H
