//
// Created by Predator on 2020/11/17.
//

#ifndef PROJECT4_PREDICTOR_H
#define PROJECT4_PREDICTOR_H
#include "Golomb.h"
#include "opencv2/opencv.hpp"
#include "YUV420Converter.h"
#include "YUV422Converter.h"
#include "YUV444Converter.h"
#include "BGRConverter.h"
#include "Format.h"
using namespace cv;


class Predictor {
public:
    Predictor(string filePath, int ray, int blockSize, int shift = 0, int mode = 0):golomb(filePath, 8){
        this->m = 8;
        this->type = 8;
        this->ray = ray;
        this->shift = shift;
        this->blockSize = blockSize;
        this->mode = mode;
        this->convs.push_back(new YUV420Converter);
        this->convs.push_back(new YUV422Converter);
        this->convs.push_back(new YUV444Converter);
        this->convs.push_back(new BGRConverter);
    };
    void encodeVideo(const string& videoPath, Format format=YUV420);
    void decodeVideo();
    void flip();
    void close();
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
    int mode; // 0 mistura, 1 intra, 2 inter
    int shifts[9][2] = {{0, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}, {0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    int mediaFrame(const Mat& frame);
    int getBestMifUsePredictor(const Mat& frame);
    void encode(Mat& frame);
    void encodeIntra(Mat& frame);
    void encodeInter(Mat& frame);
    void encodeFrame(Mat &frame);
    Mat findBestBlockAndEncode(const Mat& frame, int x, int y);
    void encodeBlock(Mat& frame, int x, int y, bool sameBlock);
    Mat decodeIntra(int rows, int cols, int channels);
    Mat decodeInter(int rows, int cols, int channels);
    void decodeBlock(Mat& frame, int x, int y, int size);
    int getPredictValor(const Mat& frame, int channel, int line, int col) const;
};

#endif //PROJECT4_PREDICTOR_H
