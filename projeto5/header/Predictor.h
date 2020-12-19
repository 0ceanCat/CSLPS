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
    Predictor(string filePath, int type, int ray, int blockSize):golomb(filePath, 8){
        if (type <= 0 or type > 8){
            throw invalid_argument("type should be >= 1 and <= 9");
        }
        this->m = 8;
        this->type = type;
        this->ray = ray;
        this->blockSize = blockSize;
        this->convs.push_back(new BGRConverter);
        this->convs.push_back(new YUV420Converter);
        this->convs.push_back(new YUV422Converter);
        this->convs.push_back(new YUV444Converter);
    };
    void encodeVideo(const string& videoPath, Format format=BGR);
    void decodeVideo();
    void flip();
private:
    Golomb golomb;
    vector<Converter *> convs;
    int type;
    int m;
    int ray;
    Mat lastFrame;
    int bitsLastFrame = 0;
    int blockSize;
    int shifts[9][2] = {{0, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}, {0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    int mediaFrame(const Mat& frame);
    int getBestMifUsePredictor(const Mat& frame);
    void encode(const Mat& frame);
    void encodeInter(const Mat& frame);
    void encodeIntra(const Mat& frame);
    void encodeFrame(const Mat &frame);
    void findBestBlockAndEncode(const Mat& frame, int x, int y);
    void encodeBlock(const Mat& frame, int x, int y, bool sameBlock);
    Mat decodeInter(int rows, int cols, int channels);
    Mat decodeIntra(int rows, int cols, int channels);
    void decodeBlock(Mat& frame, int x, int y, int size);
    int getPredictValor(const Mat& frame, int channel, int line, int col) const;
    int sum(const Mat &block);
    int getValidPixelValue(int val);
};

#endif //PROJECT4_PREDICTOR_H
