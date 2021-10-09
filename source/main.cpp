#include <iostream>
#include <sysinfoapi.h>
#include "../header/Predictor.h"
using namespace cv;
using namespace std;

void test(){
    String videos[] = {"in_to_tree_420_720p50", "ducks_take_off_420_720p50", "park_joy_420_720p50", "old_town_cross_420_720p50"};
    int types[] = {1, 2};
    for (const String& v : videos) {
        for (bool type : types) {
            for (int shift = 0; shift < 8; ++shift) {
                String path = "D:/Universidade/3ano/CLP/" + v + ".y4m";
                Predictor p("C:/Users/Predator/Desktop/write.bin", 3, 16, shift, type);
                long time_start = GetTickCount();
                p.encodeVideo(path, YUV420);
                p.close();
                long time_end = GetTickCount();
                long encode = time_end - time_start;

                ifstream in("C:/Users/Predator/Desktop/write.bin", std::ifstream::ate | std::ifstream::binary);
                int size = in.tellg();
                cout <<  "Video: " << v << endl;
                String mode = type == 1 ? "Intra" : "Inter";
                cout << "Mode: " << mode << endl;
                cout <<  "Shift: " << shift << endl;
                cout <<  "Size: " << size / 1024.0 / 1024.0 << " mb" << endl;
                cout << "encode: " << encode / 1000 << " s" << endl;
                cout << endl;
            }
        }
    }
}
int main() {
   /* Predictor p("C:/Users/Predator/Desktop/write.bin", 3, 16, 3, false);
    long time_start = GetTickCount();
    p.encodeVideo("D:/Universidade/3ano/CLP/ducks50.y4m");
    long time_end = GetTickCount();
    long encode = time_end - time_start;
    p.flip();
    time_start = GetTickCount();
    p.decodeVideo();
    time_end = GetTickCount();
    long decode = time_end - time_start;

    cout << "encode: " << encode / 1000 << " s, " << "decode: " << decode / 1000 << " s";
*/
   test();
   return 0;
}

