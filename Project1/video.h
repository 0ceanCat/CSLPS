#ifndef VIDEO_H
#define VIDEO_H
#include "mainwindow.h"
using namespace cv;

class Video
{
public:
    Video(VideoCapture capt);
private:
    VideoCapture capt;
    Mat frame;
    Size size;
    int frames;
};

#endif // VIDEO_H
