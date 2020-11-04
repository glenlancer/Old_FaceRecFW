/********************************************************************
 * Copyright © Lin Guannan. 2011-2012. All rights reserved.
 *
 * File name:     v4l2_driver.h
 * First Creator: Lin Guannan
 * Data:          2012-5-19
 * Description:   header file for operating USB-camera devices using
 *                v4l2 driver
 * Others:        this file may be independent from other part of the
 *                system, for it could be also used in other apps
 * History:
 *     1. Data:            2012-5-19
 *        Author:          Lin Guannan
 *        Modification:    First Creation of the file
 *******************************************************************/
#ifndef V4L2_DRIVER_H
#define V4L2_DRIVER_H

#include <linux/videodev2.h>

/* local structures */
/* store one video frame in memory */
struct _swap_buffer {
    void *start;
    size_t length;
};

typedef _swap_buffer swap_buffer;

class V4l2_Device {
public:
    explicit V4l2_Device();
    ~V4l2_Device();

    void video_init();
    void video_quit();
    void buffer_enqueue(int index);
    void buffer_dequeue(int index);
    
    void update_rgb_pixels(int index);
    void set_device_name(const char *device);

    int width();
    int height();
    int nBuffers();

    unsigned char *get_frame();

private:
    void video_open();
    void video_close();
    void video_set_format();
    void video_streamon();
    void video_streamoff();
    void buffer_init();
    void buffer_free();
    void buffer_request();
    void buffer_mmap(int index);

    void yuyv2rgb(unsigned char Y, unsigned char U, unsigned char V,
                  unsigned char *pR, unsigned char *pG, unsigned char *pB);
    unsigned char clamp(double yuv);

    int fd;
    char *device_name;
    struct v4l2_format format;
    struct v4l2_requestbuffers req;
    struct v4l2_buffer query;

    swap_buffer *buf;
    unsigned char *frame;
    int video_width;
    int video_height;
};

#endif /* V4L2_DRIVER_H */
