#include "mythread.h"
#include <iostream>
#include <opencv2/imgproc.hpp>
#include "opencv2/opencv.hpp"
#include "mainwindow.h"

using namespace std;

MyThread::MyThread(QObject *parent) : QThread(parent)
{

}

MyThread::MyThread(Player* p)
{
    this->p = p;
    this->stop = false;
}


void MyThread:: run(){
    Mat img;
    int fps = p->video.get(CAP_PROP_FPS);
    int delay = 1000 / fps;
    for(;;){
        cout << "run " << QThread::currentThreadId() << endl;
       p->video.read(img);
       // check if we succeeded
       if (img.empty()) {
           cerr << "ERROR! blank frame grabbed\n";
           break;
       }
       Mat rgbImg;
       cvtColor(img, rgbImg, COLOR_BGR2RGB);
       p->setImage(rgbImg);
       p->displayImg();
       msleep(delay);
       if (stop)
           break;
    }
}

void MyThread::stopWork(){
    this->stop = true;
}
