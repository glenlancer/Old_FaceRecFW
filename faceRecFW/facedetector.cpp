/********************************************************************
 * Copyright © Lin Guannan. 2011-2012. All rights reserved.
 *
 * File name:     facedetector.cpp
 * First Creator: Lin Guannan
 * Data:          2012-5-19
 * Description:   source file for class FaceDetector
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
#include "facedetector.h"

using namespace std;

FaceDetector::FaceDetector()
{
    char filename_mask[20] = "./mask_image.jpg";

    frontal_face_cascade_name = FACE_CAS_FILENAME;
    left_eye_cascade_name     = LEFT_EYE_CAS_FILENAME;
    right_eye_cascade_name    = RIGHT_EYE_CAS_FILENAME;

    if (!frontal_face_cascade.load(frontal_face_cascade_name) ||
        !left_eye_cascade.load(left_eye_cascade_name) ||
        !right_eye_cascade.load(right_eye_cascade_name))
    {
        fprintf(stderr, "--(!)Error loading: cascade files\n");
        exit(EXIT_FAILURE);
    }

    mask_image = read_gray_image(filename_mask);

    scale_for_frame = 5.0;
    scale_for_eye   = 2.0;
    min_face        = 150;
    norm_face_size  = 128;

    got_norm_face = false;
    norm_face.create(norm_face_size, norm_face_size, CV_8UC1);
}

FaceDetector::~FaceDetector() { }

void FaceDetector::drawRectangle(cv::Mat &image, cv::Rect &draw_r, color_select color)
{
    cv::Point pt_1 = cv::Point(draw_r.x, draw_r.y);
    cv::Point pt_2 = cv::Point(draw_r.x + draw_r.width,
                               draw_r.y + draw_r.height);

    cv::Scalar scalar;

    switch (color) {
        case first_color:
            scalar = cv::Scalar(255, 255, 0);
            break;
        case second_color:
            scalar = cv::Scalar(255, 0, 255);
            break;
        case third_color:
            scalar = cv::Scalar(0, 255, 255);
            break;
        case forth_color:
            scalar = cv::Scalar(255, 255, 255);
    }

    cv::rectangle(image, pt_1, pt_2, scalar);
}

/* resize the detection object image by ratio scale can greatly */
/* save the processing time */
void FaceDetector::objectDetection(cv::Mat &image, cv::Rect &draw_r, double scale,
                                   cv::CascadeClassifier &cascade, cv::Size minSize)
{
    cv::Mat main_image, small_image;
    std::vector<cv::Rect> faces;

    main_image.create(image.rows, image.cols, CV_8UC1);
    /* parameter scale here is very important */
    small_image.create(cvRound(image.rows / scale), cvRound(image.cols / scale), CV_8UC1);

    image.copyTo(main_image);

    cv::resize(main_image, small_image, small_image.size(), 0, 0, CV_INTER_LINEAR);
    cv::equalizeHist(small_image, small_image);

    draw_r = cv::Rect(0, 0, 0, 0);

    if (!cascade.empty()) {
        cascade.detectMultiScale(small_image, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, minSize);

        cv::Rect r_temp = cv::Rect(0, 0, 0, 0);

        for (size_t i = 0; i < faces.size(); i++) {
            r_temp = faces[i];
            if (r_temp.width > draw_r.width)
                draw_r = r_temp;
        }
        draw_r.x *= scale;
        draw_r.y *= scale;
        draw_r.width *= scale;
        draw_r.height *= scale;
    }
}

cv::Mat FaceDetector::faceLocating(cv::Mat &matImage)
{
    assert(!matImage.empty());

    got_norm_face = false;

    origin_image = matImage.clone();

    /* flip the image, so the image to be processed is a natural case */
    cv::flip(origin_image, origin_image, 1);

    gray_image.create(matImage.rows, matImage.cols, CV_8UC1);

    /* ? I think here should be BGR, but I still not sure about how cv::Mat handle raw data */
    /* ... this should be checked later */
    /* here is BGR is according to a test made in function QImage mat2qimage(const cv::Mat &image) */
    cv::cvtColor(origin_image, gray_image, CV_BGR2GRAY);

    /* 1. face detection */
    /* 2. eye detection */
    /* 3. rotation */
    /* 4. locate face-feature points  */
    /* 5. cut and resize */
    /* 6. face picture normalization */
    cv::Mat image_reye;
    cv::Mat image_leye;
    cv::Mat image_rotated;
    cv::Mat image_roi;
    cv::Mat image_resized;

    cv::Rect face_r   = cv::Rect(0, 0, 0, 0);
    cv::Rect leye_r   = cv::Rect(0, 0, 0, 0);
    cv::Rect reye_r   = cv::Rect(0, 0, 0, 0);
    cv::Rect leye_r_g = cv::Rect(0, 0, 0, 0);
    cv::Rect reye_r_g = cv::Rect(0, 0, 0, 0);

    cv::Point center_leye   = cv::Point(0, 0);
    cv::Point center_reye   = cv::Point(0, 0);
    cv::Point center_leye_g = cv::Point(0, 0);
    cv::Point center_reye_g = cv::Point(0, 0);

    objectDetection(gray_image, face_r, scale_for_frame, frontal_face_cascade, cv::Size(30, 30));

    if (face_r.width > min_face) {
        drawRectangle(origin_image, face_r, first_color);

        cv::Size eye_size;
        eye_size.width = (int)(face_r.width / 3);
        eye_size.height = (int)(face_r.height / 4);

        cv::Point tlp_reye, tlp_leye;
        tlp_reye.x = (int)(face_r.width / 7);
        tlp_reye.y = (int)(face_r.height / 4);
        tlp_leye.x = (int)(face_r.width * 11 / 21);
        tlp_leye.y = tlp_reye.y;

        cv::Rect right_eye_window = cv::Rect(tlp_reye.x + face_r.x, tlp_reye.y + face_r.y,
                                             eye_size.width, eye_size.height);
        cv::Rect left_eye_window = cv::Rect(tlp_leye.x + face_r.x, tlp_leye.y + face_r.y,
                                            eye_size.width, eye_size.height);

        drawRectangle(origin_image, right_eye_window, second_color);
        drawRectangle(origin_image, left_eye_window, second_color);

        image_reye = get_image_roi(gray_image, right_eye_window);
        image_leye = get_image_roi(gray_image, left_eye_window);

        objectDetection(image_reye, reye_r, scale_for_eye, right_eye_cascade, cv::Size(16, 8));
        objectDetection(image_leye, leye_r, scale_for_eye, left_eye_cascade, cv::Size(16, 8));

        if (reye_r.width && leye_r.height) {
            center_reye = cv::Point(reye_r.x + (int)reye_r.width / 2,
                                    reye_r.y + (int)reye_r.height /2);
            center_leye = cv::Point(leye_r.x + (int)leye_r.width / 2,
                                    leye_r.y + (int)leye_r.height / 2);

            center_reye_g.x = center_reye.x + right_eye_window.x;
            center_reye_g.y = center_reye.y + right_eye_window.y;

            center_leye_g.x = center_leye.x + left_eye_window.x;
            center_leye_g.y = center_leye.y + left_eye_window.y;

            reye_r_g.x = reye_r.x + right_eye_window.x;
            reye_r_g.y = reye_r.y + right_eye_window.y;
            reye_r_g.width = reye_r.width;
            reye_r_g.height = reye_r.height;

            leye_r_g.x = leye_r.x + left_eye_window.x;
            leye_r_g.y = leye_r.y + left_eye_window.y;
            leye_r_g.width = leye_r.width;
            leye_r_g.height = leye_r.height;

            cv::circle(origin_image, center_reye_g, 3, cv::Scalar(255, 255, 255));
            drawRectangle(origin_image, reye_r_g, forth_color);
            cv::circle(origin_image, center_leye_g, 3, cv::Scalar(255, 255, 255));
            drawRectangle(origin_image, leye_r_g, forth_color);

            /* got_norm_face = false; */

            /* pre-processing of face recognition */
            if (abs(center_leye_g.y - center_reye_g.y) < 10 /*&&
                    abs(center_leye_g.x - center_reye_g.x) < face_r.width / 4*/) {
                /* flag to indicate that there is a new face image to be */
                /* pre-processed. */
                got_norm_face = true;

                cv::Point center_leye_g_rotated = cv::Point(center_leye_g);
                cv::Point center_reye_g_rotated = cv::Point(center_reye_g);

                /* 1. rotation */
                if (abs(center_leye_g.y - center_reye_g.y) >= 3) {
                    image_rotated = face_rotation(gray_image, center_leye_g_rotated,
                                                  center_reye_g_rotated);
                } else {
                    image_rotated = gray_image.clone();
                }

                /* checking rotation result */
                //write_image("old_image.jpg", gray_image);
                //write_image("rotated_image.jpg", image_rotated);
                /* end of checking rotation result */

                /* 2. cut and resize */
                cv::Rect face_seg_r;
                int eye_distance;

                eye_distance = abs(center_leye_g_rotated.x - center_reye_g_rotated.x);

                face_seg_r.width = 2 * eye_distance;
                face_seg_r.height = 2 * eye_distance;

                if ((center_reye_g_rotated.x - eye_distance / 2) > 0) {
                    face_seg_r.x = center_reye_g_rotated.x - eye_distance / 2;
                } else {
                    face_seg_r.x = 0;
                }

                if ((center_leye_g_rotated.y - eye_distance / 2) > 0) {
                    face_seg_r.y = center_leye_g_rotated.y - eye_distance / 2;
                } else {
                    face_seg_r.y = 0;
                }

                /* draw cutted rectangle from original image */
                /* this face_seg_r to draw on original image should be modified! */
                drawRectangle(origin_image, face_seg_r, first_color);

                image_roi.create(face_seg_r.size(), CV_8UC1);
                image_roi = get_image_roi(image_rotated, face_seg_r);

                /* resize the image to 128*128 */
                image_resized.create(norm_face_size, norm_face_size, CV_8UC1);
                cv::resize(image_roi, image_resized, image_resized.size());

                /* 3. histogram equalization */
                cv::equalizeHist(image_resized, image_resized);

                /* 4. add mask */
                cv::min(image_resized, mask_image, norm_face);
            }
            /* if code execuited here, which means face can be pre-processed, */
            /* then, start face recognition process */
        }
    }

    /* flip back the input image */
    cv::flip(origin_image, origin_image, 1);
    return origin_image;
}

/********************************************************************
 * function name:
 *     face_rotation
 * parameters:
 *     cv::Mat    &gray_image   - input gray face image
 *     cv::Point  &leye         - coordinates of left eye center
 *     cv::Point  &reye         - coordinates of right eye center
 * return:
 *     cv::Mat                  - rotated face image (returned is a clone)
 * description:
 *     this function rotates the input face image to keep the
 *     coordinates of two eyes are in horizontal line.
 *******************************************************************/
cv::Mat FaceDetector::face_rotation(cv::Mat &gray_image, cv::Point &leye, cv::Point &reye)
{
    cv::Mat M;
    cv::Mat rotated_image;
    cv::Point old_leye;
    cv::Point old_reye;

    cv::Point2f rotation_center(gray_image.rows / 2.0f, gray_image.cols / 2.0f);
    rotated_image.zeros(gray_image.size(), CV_8UC1);

    /* arc to degree */
    double angle_arc = atan2(leye.y - reye.y, leye.x - reye.x);
    double angle_degree = angle_arc * 180.0 / CV_PI;

    M = cv::getRotationMatrix2D(rotation_center, angle_degree, 1.0);

    cv::warpAffine(gray_image, rotated_image, M, rotated_image.size());

    /* calculate the leye and reye center point after rotation */
    /* dst(x, y) = src(M11x + M12y + M13, M21x + M22y + M23) */
    old_leye = leye;
    old_reye = reye;

    leye.x = M.at<double>(0, 0) * old_leye.x + M.at<double>(0, 1) * old_leye.y + M.at<double>(0, 2);
    leye.y = M.at<double>(1, 0) * old_leye.x + M.at<double>(1, 1) * old_leye.y + M.at<double>(1, 2);

    reye.x = M.at<double>(0, 0) * old_reye.x + M.at<double>(0, 1) * old_reye.y + M.at<double>(0, 2);
    reye.y = M.at<double>(1, 0) * old_reye.x + M.at<double>(1, 1) * old_reye.y + M.at<double>(1, 2);

    return rotated_image;
}

cv::Mat FaceDetector::get_norm_face()
{
    return norm_face;
}

bool FaceDetector::norm_face_flag()
{
    return got_norm_face;
}

/* set new cascade files */
void FaceDetector::set_face_cascade(const std::string &face_cascade_name)
{
    frontal_face_cascade_name = face_cascade_name;
    if (!frontal_face_cascade.load(frontal_face_cascade_name)) {
        fprintf(stderr, "--(!)Error loading: face_cascade\n");
        exit(EXIT_FAILURE);
    }
}

void FaceDetector::set_leye_cascade(const std::string &leye_cascade_name)
{
    left_eye_cascade_name = leye_cascade_name;
    if (!left_eye_cascade.load(left_eye_cascade_name)) {
        fprintf(stderr, "--(!)Error loading: left_eye_cascade\n");
        exit(EXIT_FAILURE);
    }
}

void FaceDetector::set_reye_cascade(const std::string &reye_cascade_name)
{
    right_eye_cascade_name = reye_cascade_name;
    if (!right_eye_cascade.load(right_eye_cascade_name)) {
        fprintf(stderr, "--(!)Error loading: right_eye_cascade\n");
        exit(EXIT_FAILURE);
    }
}
