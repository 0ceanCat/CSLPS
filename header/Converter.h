#ifndef PROJECT4_CONVERTER_H
#define PROJECT4_CONVERTER_H
#include "opencv2/opencv.hpp"
using namespace cv;

class Converter{
public:
    virtual Mat encode(Mat &img) = 0;
    virtual Mat decode(Mat &img) = 0;
};

#endif
