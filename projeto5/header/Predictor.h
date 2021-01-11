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
    Predictor(string filePath, int ray, int blockSize, int shift = 0):golomb(filePath, 8){
        this->m = 8;
        this->type = 8;
        this->ray = ray;
        this->shift = shift;
        this->blockSize = blockSize;
        this->convs.push_back(new BGRConverter);
        this->convs.push_back(new YUV420Converter);
        this->convs.push_back(new YUV422Converter);
        this->convs.push_back(new YUV444Converter);
    };
    void encodeVideo(const string& videoPath, Format format);
    void decodeVideo();
    void flip();
private:
    Golomb golomb;
    vector<Converter *> convs;
    int type;
    int m;
    int ray;
    int shift;
    Mat lastFrame;
    int bitsLastFrame = 0;
    int blockSize;
    int shifts[9][2] = {{0, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}, {0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    int mediaFrame(const Mat& frame);
    void encode(Mat& frame);
    void encodeInter(Mat& frame);
    void encodeIntra(Mat& frame);
    void encodeFrame(Mat &frame);
    Mat findBestBlockAndEncode(const Mat& frame, int x, int y);
    void encodeBlock(Mat& block, int x, int y, bool sameBlock);
    Mat decodeInter(int rows, int cols, int channels);
    Mat decodeIntra(int rows, int cols, int channels);
    void decodeBlock(Mat& frame, int x, int y, int size);
    int getPredictValor(const Mat& frame, int channel, int line, int col) const;
};

#endif //PROJECT4_PREDICTOR_H
