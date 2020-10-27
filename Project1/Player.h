#ifndef PLAYER_H
#define PLAYER_H
#include <opencv2/highgui.hpp>
#include <iostream>
#include <QMainWindow>
#include <QLabel>

using namespace cv;
using namespace std;

class MyThread;
class Player: public QObject{
    Q_OBJECT
    friend MyThread;
    private:
        Mat img;
        Mat original;
        VideoCapture video;
        bool isVideo;
        char option;
        QLabel* label;
        int frame; // index of frame
        int threshold_type;
        int threshold_value;
        int morph_operator;
        int morph_size;
    public:
        Player();
        ~Player();
        void setLabel(QLabel *label);
        void setOption(char option);
        void setImage(Mat img);
        void setVideo(VideoCapture video);
        void displayVideo();
        void displayImg();
        bool getIsVideo();
        VideoCapture getVideo();
        Mat getImg();
        bool stop;
        Mat waterMark(String filepath="C:/Users/Predator/Desktop/logo.png", String text="Complementos Sobre Linguagens de Programacao");
        Mat doOptionWork();
        Mat changeSpaceAndshowChannels();
        Mat colorHistogram();
        Mat grayAndHistogram();
        Mat gaussianBlurFilters();
        Mat segmentation();
        char getOption();
        void setFrame(int frame);
        void setThresholdType(int type);
        void setThresholdValue(int value);
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
