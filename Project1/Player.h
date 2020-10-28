#ifndef PLAYER_H
#define PLAYER_H
#include <opencv2/highgui.hpp>
#include <iostream>
#include <QMainWindow>
#include <QLabel>

using namespace cv;
using namespace std;

class Player: public QObject{
    Q_OBJECT
    public:
        Mat img;        //image
        Mat original;   //original image
        VideoCapture video;
        bool isVideo;   //if the current resource is video
        int option;
        QLabel* label;  //where the video/image will be showed
        int frameIndex;      //index of the current frame
        int thresholdType;
        int thresholdValue;
        int morphOperator;
        int morphSize;
        int morphelem;
        int colorSpace;
        int kernelWidth;
        int kernelHeight;
        int gradient;
        int ksize;
        int scale;
        int delta;
        int lowThreshold = 0;
        int maxLowthreshold = 100;
        int ratio = 3;
        int cannyKsize = 3;
        int conversion;
        int lastConversion = -1;
        String watermarkPicPath;
        Mat watermarkPic;
        String watermarkText;
        Player();
        ~Player();
        void setLabel(QLabel *label);
        void setOption(int option);
        void setImage(Mat img);
        void setVideo(VideoCapture video);
        void setWaterMarkText(String text);
        void setWaterMarkPicPath(String path);
        void setColorSpace(int colorSpace);
        void setKernelWidth(int w);
        void setKernelHeight(int h);
        void setMorphOperator(int mp);
        void setMorphSize(int ms);
        void setMorhElem(int me);
        void setKsize(int ksize);
        void setScale(int scale);
        void setDelta(int delta);
        void setLowThreshold(int lt);
        void setRatio(int r);
        void setCannyKsize(int ck);
        void displayVideo();
        void displayImg();
        void setGradient(int g);
        void setConversion(int c);
        bool getIsVideo();
        int getOption();
        void setThresholdType(int type);
        void setThresholdValue(int value);
        VideoCapture getVideo();
        Mat getImg();
        bool stop;
        Mat waterMark();
        Mat doOptionWork();
        Mat changeSpaceAndshowChannels();
        Mat colorHistogram();
        Mat grayAndHistogram();
        Mat gaussianBlurFilters();
        Mat segmentation();
        Mat myConverter();
        Mat bgr2yuv420();
        Mat bgr2yuv444();
        Mat bgr2yuv422();
        Mat yuv420ToBgr();
        Mat yuv422ToBgr();
        Mat yuv444ToBgr();
        Mat morphologyTransformations();
        Mat gradients();
        Mat canny();
    signals:
        void changeResource();
}
;

#endif // PLAYER_H
