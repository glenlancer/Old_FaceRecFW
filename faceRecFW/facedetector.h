/********************************************************************
 * Copyright © Lin Guannan. 2011-2012. All rights reserved.
 *
 * File name:     facedetector.h
 * First Creator: Lin Guannan
 * Data:          2012-5-19
 * Description:   header file for class FaceDetector
 *                this class implements face detection algorithm used
 *                to extract human faces in one video frame
 * Others:        face detection algorithm here uses Haar-based adaboost
 *                algorithms
 * History:
 *     1. Data:            2012-5-19
 *        Author:          Lin Guannan
 *        Modification:    First Creation of the file, mainly use
 *                         existing OpenCV code
 *******************************************************************/
#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include "common.h"

#define MAX_FILE_NAME  256

#define FACE_CAS_FILENAME      "haarcascade_frontalface_alt2.xml"
#define LEFT_EYE_CAS_FILENAME  "haarcascade_mcs_lefteye.xml"
#define RIGHT_EYE_CAS_FILENAME "haarcascade_mcs_righteye.xml"

enum color_select {
    first_color, second_color, third_color, forth_color
};

class FaceDetector
{
public:
    FaceDetector();
    ~FaceDetector();

    cv::Mat faceLocating(cv::Mat &matImage);

    void set_face_cascade(const std::string &face_cascade_name);
    void set_leye_cascade(const std::string &leye_cascade_name);
    void set_reye_cascade(const std::string &reye_cascade_name);

    cv::Mat get_norm_face();
    bool    norm_face_flag();

private:
    void objectDetection(cv::Mat &image, cv::Rect &draw_r, double scale,
                         cv::CascadeClassifier &cascade, cv::Size minSize);

    void drawRectangle(cv::Mat &image, cv::Rect &draw_r, color_select color);

    /* functions used in face image pre-processing */
    cv::Mat face_rotation(cv::Mat &gray_image, cv::Point &leye, cv::Point &reye);

    std::string frontal_face_cascade_name;
    std::string left_eye_cascade_name;
    std::string right_eye_cascade_name;

    cv::CascadeClassifier frontal_face_cascade;
    cv::CascadeClassifier left_eye_cascade;
    cv::CascadeClassifier right_eye_cascade;

    cv::Mat origin_image;
    cv::Mat gray_image;
    cv::Mat mask_image;
    cv::Mat norm_face;

    double scale_for_frame;
    double scale_for_eye;
    int    norm_face_size;
    int    min_face;

    bool   got_norm_face;
};

#endif // FACEDETECTOR_H
