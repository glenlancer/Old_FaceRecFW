/********************************************************************
 Copyright © Lin Guannan. 2011-2012. All rights reserved.

 File name:     videostreamreader.h
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
#ifndef VIDEOSTREAMREADER_H
#define VIDEOSTREAMREADER_H

#include <QImage>
#include <QMutex>
#include <QWaitCondition>
#include <QThread>
#include <QTimer>
#include <QDir>

#include "v4l2_driver.h"
#include "facedetector.h"

class VideoStreamReader : public QThread
{
    Q_OBJECT
public:
    explicit VideoStreamReader(QObject *parent = 0);
    ~VideoStreamReader();

    void startRecord();
    void stopRecord();

    void transferCompleted();
    void setWebCamDevice(const std::string &device);
    void setFaceCascade(const std::string &cascade_filename);
    void setLeftEyeCascade(const std::string &cascade_filename);
    void setRightEyeCascade(const std::string &cascade_filename);

    cv::Mat getFrame();
    cv::Mat getNormFace();

    bool getHasNewNormFace();

    bool running_flag();

signals:
    void transferStart();

protected:
    void run();

private:
    bool     running;
    bool     norm_face_flag;

    cv::Mat  imageColor;
    cv::Mat  imageNorm;

    QMutex         mutex;
    QWaitCondition transferCondition;

    FaceDetector faceDetector;
    V4l2_Device  webcam_device;
};

#endif // VIDEOSTREAMREADER_H
