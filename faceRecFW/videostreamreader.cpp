/********************************************************************
 Copyright © Lin Guannan. 2011-2012. All rights reserved.

 File name:     videostreamreader.cpp
 First Creator: Lin Guannan
 Data:          2012-5-19
 Description:   class VideoStreamReader extracts frames from v4l2
                devices and coordinately displays those frames
                on the main GUI
 Others:        this class is esstential, since I am not quite
                familiar with multi-threads programming and
                a little improvement in this class may do a lot
                contrbution to the processing time of the algorithm
 History:
     1. Data:            2012-5-19
        Author:          Lin Guannan
        Modification:    First Creation of the file
********************************************************************/
#include "videostreamreader.h"

VideoStreamReader::VideoStreamReader(QObject *parent) :
    QThread(parent)
{
    QMutexLocker locker(&mutex);
    running = false;
    norm_face_flag = false;
}

VideoStreamReader::~VideoStreamReader()
{
    stopRecord();
    wait();
}

void VideoStreamReader::setFaceCascade(const std::string &cascade_filename)
{
    faceDetector.set_face_cascade(cascade_filename);
}

void VideoStreamReader::setLeftEyeCascade(const std::string &cascade_filename)
{
    faceDetector.set_leye_cascade(cascade_filename);
}

void VideoStreamReader::setRightEyeCascade(const std::string &cascade_filename)
{
    faceDetector.set_reye_cascade(cascade_filename);
}

void VideoStreamReader::setWebCamDevice(const std::string &device)
{
    webcam_device.set_device_name(device.c_str());
}

/* run function wimageNormNewithin thread, extract image from V4L2 device */
void VideoStreamReader::run()
{
    webcam_device.video_init();
    int i = 0;
    int count = webcam_device.nBuffers();

    while (running) {
        {
            QMutexLocker locker(&mutex);
            webcam_device.buffer_dequeue(i);
            webcam_device.update_rgb_pixels(i);
            webcam_device.buffer_enqueue(i);

            imageColor = rawdata2mat(webcam_device.get_frame(),
                                     webcam_device.height(), webcam_device.width(),
                                     CV_8UC3);

            imageColor = faceDetector.faceLocating(imageColor);

            /* the incoming new norm face from faceDetector always overwirte */
            /* the old one got from the last loop.                           */
            /* this logical check here make sure we don't copy imageNorm     */
            /* when norm face data in faceDetector does not change.          */
            if (faceDetector.norm_face_flag()) {
                norm_face_flag = true;
                imageNorm = faceDetector.get_norm_face();
            }

            emit transferStart();
            transferCondition.wait(&mutex);
            norm_face_flag = false;
        }

        i++;
        if (i == count) i = 0;
    }

    webcam_device.video_quit();
}

bool VideoStreamReader::getHasNewNormFace()
{
    QMutexLocker locker(&mutex);
    return norm_face_flag;
}

void VideoStreamReader::transferCompleted()
{
    transferCondition.wakeOne();
}

/* get current image stored in VideoStreamReader Object */
cv::Mat VideoStreamReader::getFrame()
{
    QMutexLocker locker(&mutex);
    return imageColor;
}

cv::Mat VideoStreamReader::getNormFace()
{
    QMutexLocker locker(&mutex);
    return imageNorm;
}

bool VideoStreamReader::running_flag()
{
    QMutexLocker locker(&mutex);
    return running;
}

/* control VideoStreamReader thread running state */
void VideoStreamReader::startRecord()
{
    QMutexLocker locker(&mutex);
    running = true;
    start();
}

void VideoStreamReader::stopRecord()
{
    QMutexLocker locker(&mutex);
    running = false;
}
