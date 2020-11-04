/********************************************************************
 * Copyright © Lin Guannan. 2011-2012. All rights reserved.
 *
 * File name:     v4l2_driver.cpp
 * First Creator: Lin Guannan
 * Data:          2012-5-19
 * Description:   source file for operating USB-camera devices using
 *                v4l2 driver
 *                v4l2 driver program uses mmap to co-ordinate memories
 *                in kernel and user spaces
 * Others:        this file may be independent from other part of the
 *                system, for it could be also used in other apps
 * History:
 *     1. Data:            2012-5-19
 *        Author:          Lin Guannan
 *        Modification:    First Creation of the file
 *******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <sys/mman.h>
//#include <stddef.h>

#include "v4l2_driver.h"

/* local macro definitions */
#define MAX_DEVICE_NUM  10
#define BUFFER_NUM       5       /* maximum number is 32 in my dell xps pc */
#define VIDEO_WIDTH     640
#define VIDEO_HEIGHT    480

static void errno_exit(const char *s)
{
    fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
    exit(EXIT_FAILURE);
}

void V4l2_Device::set_device_name(const char *device)
{
    strcpy(device_name, device);
}

V4l2_Device::V4l2_Device() : video_width(VIDEO_WIDTH), video_height(VIDEO_HEIGHT)
{
    int i;
    char device[13];

    device_name = new char[13];

    for (i = 0; i < MAX_DEVICE_NUM; i++) {
        sprintf(device, "%s%d", "/dev/video", i);
        if (!access(device, 0)) {
            strcpy(device_name, device);
            break;
        }
    }

    if (i == MAX_DEVICE_NUM) device_name = NULL;

    /* buffer to store image extracted from kernel space */
    frame = new unsigned char[video_width * video_height * 3];
}

V4l2_Device::~V4l2_Device()
{
    delete []device_name;
    delete []frame;
}

void V4l2_Device::video_init()
{
    video_open();
    video_set_format();
    buffer_init();
    video_streamon();
}

void V4l2_Device::video_quit()
{
    video_streamoff();
    buffer_free();
    video_close();
}

void V4l2_Device::buffer_enqueue(int index)
{
    memset(&query, 0, sizeof(struct v4l2_buffer));
    query.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    query.memory = V4L2_MEMORY_MMAP;
    query.index = index;

    /* add EAGAIN latter ? */
    if (ioctl(fd, VIDIOC_QBUF, &query) == -1) {
        errno_exit("VIDIOC_DQBUF");
    }
}

void V4l2_Device::buffer_dequeue(int index)
{
    memset(&query, 0, sizeof(struct v4l2_buffer));
    query.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    query.memory = V4L2_MEMORY_MMAP;
    query.index = index;

    if (ioctl(fd, VIDIOC_DQBUF, &query) == -1) {
        errno_exit("VIDIOC_DQBUF");
    }
}

/*--------local functions---------*/
void V4l2_Device::video_open()
{
    fd = open(device_name, O_RDONLY);
    if (fd == -1) {
        errno_exit("cannot open v4l2 device");
    }
}

void V4l2_Device::video_close()
{
    close(fd);
}

void V4l2_Device::video_set_format()
{
    memset(&format, 0, sizeof(struct v4l2_format));
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.width = video_width;
    format.fmt.pix.height = video_height;
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;

    if (ioctl(fd, VIDIOC_S_FMT, &format) == -1) {
        errno_exit("VIDIOC_S_FMT");
    }
}

void V4l2_Device::video_streamon()
{
    v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_STREAMON, &type) == -1) {
        if (errno == EINVAL) {
            errno_exit("stream i/o not supported");
        } else {
            errno_exit("VIDIOC_STREAMON");
        }
    }
}

void V4l2_Device::video_streamoff()
{
    v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_STREAMOFF, &type) == -1) {
        if (errno == EINVAL) {
            errno_exit("stream i/o not supported");
        } else {
            errno_exit("VIDIOC_STREAMOFF");
        }
    }
}

void V4l2_Device::buffer_request()
{
    memset(&req, 0, sizeof(struct v4l2_requestbuffers));
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    req.count = BUFFER_NUM;

    if (ioctl(fd, VIDIOC_REQBUFS, &req) == -1) {
        if (errno == EINVAL) {
            errno_exit("video capturing or "
                       "mmap-streaming is not supported");
        } else {
            errno_exit("VIDIOC_REQBUFS");
        }
    }

    if (req.count < BUFFER_NUM) {
        perror("no enough buffers\n");
        exit(EXIT_FAILURE);
    }

    buf = new swap_buffer[req.count];

    assert(buf != NULL);
}

void V4l2_Device::buffer_mmap(int index) 
{
    memset(&query, 0, sizeof(struct v4l2_buffer));
    query.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    query.memory = V4L2_MEMORY_MMAP;
    query.index = index;

    if (ioctl(fd, VIDIOC_QUERYBUF, &query) == -1) {
        errno_exit("VIDIOC_QUERYBUF");
    }

    buf[index].length = query.length;
    buf[index].start = mmap(NULL,
                            query.length,
                            PROT_READ,
                            MAP_SHARED,
                            fd,
                            query.m.offset);

    if (buf[index].start == MAP_FAILED) {
        errno_exit("mmap");
    }
}

void V4l2_Device::buffer_init()
{
    unsigned int i;

    buffer_request();
    for (i = 0; i < req.count; i++) {
        buffer_mmap(i);
        buffer_enqueue(i);
    }
}

void V4l2_Device::buffer_free()
{
    unsigned int i;

    for (i = 0; i < req.count; i++)
        munmap(buf[i].start, buf[i].length);

    delete buf;
}

/* keep yuv value within range 0 - 255 */
unsigned char V4l2_Device::clamp(double yuv)
{
    int r = (int)yuv;
    if (r < 0)        return 0;
    else if (r > 255) return 255;
    else              return r;
}

/* be aware of the differences between YUV and YCbCr */
void V4l2_Device::yuyv2rgb(unsigned char Y, unsigned char U, unsigned char V,
                           unsigned char *pR, unsigned char *pG, unsigned char *pB)
{
    double y1, pb, pr, r, g, b;

    y1 = (255 / 219.0) * (Y - 16);
    pb = (255 / 224.0) * (U - 128);
    pr = (255 / 224.0) * (V - 128);
    r = y1 + 1.402 * pr;
    g = y1 + 0.344 * pb - 0.714 * pr;
    b = y1 + 1.722 * pb;

    *pR = clamp(r);
    *pG = clamp(g);
    *pB = clamp(b);
}

void V4l2_Device::update_rgb_pixels(int index)
{
    unsigned char *data = (unsigned char *)buf[index].start;
    unsigned char Y, U, V;
    unsigned char r, g, b;
    int x, y;
    int p1, p2, p3, p4;

    for (y = 0; y < video_height; y++) {
        for (x = 0; x < video_width; x++) {

            p1 = y * video_width * 2 + x * 2;
            Y = data[p1];
            if (x % 2 == 0) {
                p2 = y * video_width * 2 + (x * 2 + 1);
                p3 = y * video_width * 2 + (x * 2 + 3);
            } else {
                p2 = y * video_width * 2 + (x * 2 - 1);
                p3 = y * video_width * 2 + (x * 2 + 1);
            }
            U = data[p2]; V = data[p3];
            yuyv2rgb(Y, U, V, &r, &g, &b);
            p4 = y * video_width * 3 + x * 3;
            frame[p4 + 0] = b;
            frame[p4 + 1] = g;
            frame[p4 + 2] = r;
            /* data in this sequence is BGR mode in cv::Mat, Why? */
        }
    }
}

int V4l2_Device::width()
{
    return video_width;
}

int V4l2_Device::height()
{
    return video_height;
}

int V4l2_Device::nBuffers()
{
    return req.count;
}

unsigned char *V4l2_Device::get_frame()
{
    return frame;
}

