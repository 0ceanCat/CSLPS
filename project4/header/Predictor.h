//
// Created by Predator on 2020/11/17.
//

#ifndef PROJECT4_PREDICTOR_H
#define PROJECT4_PREDICTOR_H
#include "Golomb.h"
#include "opencv2/opencv.hpp"
#include "BGRConverter.h"
#include "YUV420Converter.h"
#include "YUV422Converter.h"
#include "YUV444Converter.h"
#include "Format.h"
using namespace cv;


class Predictor {
public:
    Predictor(string filePath, int m, int type):golomb(filePath,m){
        if (type <= 0 or type > 8){
            throw invalid_argument("type should be >= 1 and <= 9");
        }
        this->m = m;
        this->type = type;
        this->convs.push_back(new BGRConverter);
        this->convs.push_back(new YUV420Converter);
        this->convs.push_back(new YUV422Converter);
        this->convs.push_back(new YUV444Converter);
    };
    void encodeVideo(string videoPath, Format format=BGR);
    void decodeVideo();
    void flip();

private:
    Golomb golomb;
    vector<Converter *> convs;
    int type;
    int m;
    void encodeFrame(const Mat& frame);
    void multipleChannelsDecoder(int rows, int cols, int frames, int fps, int format);
    void singleChannelDecoder(int rows, int cols, int frames, int fps, int format);
    int getPredictValor(const Mat& frame, int channel, int line, int col);
};

#endif //PROJECT4_PREDICTOR_H
