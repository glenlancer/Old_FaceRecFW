/********************************************************************
 * Copyright © Lin Guannan. 2011-2012. All rights reserved.
 *
 * File name:     common.cpp
 * First Creator: Lin Guannan
 * Data:          2012-5-19
 * Description:   source file for commonly used functions
 *                frame data transfer and commonly used functions
 * Others:
 * History:
 *     1. Data:            2012-5-20
 *        Author:          Lin Guannan
 *        Modification:    First Creation of the file
 *     2. Data:            2012-7-31
 *        Changed By:      Lin Guannan
 *        Modification:    memory leak caused by construction of QImage
 *******************************************************************/
#include "common.h"

/* frame data transfer and I/O functions for cv::Mat */

/* ugly result, just keep in mind when using this function, */
/* remember to call delete QImage::bits(), when there is no further use */
/* of QImage. Or say, delete the image memory before QImage's destruction */
/* convert cv::Mat to QImage */
QImage mat2qimage(const cv::Mat &src)
{   
    cv::Mat image = src.clone();

    if (image.channels() == 1) {
        cv::cvtColor(image, image, CV_GRAY2RGB);
    } else {
        cv::cvtColor(image, image, CV_BGR2RGB);
    }

    unsigned char *data = new unsigned char[3 * image.cols * image.rows];
    memcpy(data, image.data, 3 * image.cols * image.rows);

    /* data here: the buffer must remain valid throughout the life of the QImage. */
    /* the image does not delete the buffer at destruction. */
    return QImage(data, image.cols, image.rows,
                  // The image is stored using a 24-bit RGB format (8-8-8).
                  QImage::Format_RGB888);
}

/* convert QImage to cv::Mat */
/* this should change a RGBA8888 into a RGB cv::Mat, but */
/* it is not the case, remember to check it */
cv::Mat qimage2mat(const QImage &original_image)
{
    unsigned char *data = new unsigned char[3 * original_image.width() * original_image.height()];

    cv::Mat mat = cv::Mat(original_image.height(), original_image.width(), CV_8UC3,
                          data, original_image.bytesPerLine());

    cv::cvtColor(mat, mat, CV_RGB2BGR);

    return mat;
}

cv::Mat rawdata2mat(unsigned char *data, int rows, int cols, int type)
{
    return cv::Mat(rows, cols, type, data);
}

cv::Mat get_image_roi(cv::Mat &src, cv::Rect &r)
{
    return cv::Mat(src, r);
}

cv::Mat read_gray_image(char *filename)
{
    return cv::imread(filename, cv::IMREAD_GRAYSCALE);
}

cv::Mat read_image(char *filename)
{
    return cv::imread(filename, cv::IMREAD_COLOR);
}

void write_image(char *filename, cv::Mat result)
{
    cv::imwrite(filename, result);
}

/* common functions used in image processing */
int local_pow(int a, int b)
{
    int result = 1;
    while (b--) result *= a;
    return result;
}
