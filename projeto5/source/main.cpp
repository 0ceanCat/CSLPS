#include <iostream>
#include <sysinfoapi.h>
#include "../header/Predictor.h"
using namespace cv;
using namespace std;

int main() {
    Predictor p("C:/Users/Predator/Desktop/write.bin", 3, 16, 3);
    long time_start = GetTickCount();
    p.encodeVideo("D:/Universidade/3ano/CLP/akiyo_cif.y4m", YUV444);
    long time_end = GetTickCount();
    long encode = time_end - time_start;

    p.flip();
    time_start = GetTickCount();
    p.decodeVideo();
    time_end = GetTickCount();
    long decode = time_end - time_start;

    cout << "encode: " << encode / 1000 << " s, " << "decode: " << decode / 1000 << " s";
   return 0;
}
