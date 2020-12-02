#include <iostream>
#include <sysinfoapi.h>
#include "../header/Predictor.h"
using namespace cv;
using namespace std;

int main() {
    Predictor p("C:/Users/Predator/Desktop/write", 8, 8);
    long time_start = GetTickCount();
    p.encodeVideo("D:/Universidade/3ano/CLP/ducks_take_off_420_720p50.y4m", YUV420);
    p.flip();
    p.decodeVideo();
    long time_end = GetTickCount();
    cout << (time_end - time_start) / 1000 << " s";
    return 0;
}

