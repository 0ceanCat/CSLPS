
#ifndef PROJECT4_BGRCONVERTER_H
#define PROJECT4_BGRCONVERTER_H
#include "Converter.h"
class BGRConverter : public Converter {
public:
    Mat encode(Mat &img) {
        return img;
    }
    Mat decode(Mat &img){
        return img;
    }
};
#endif //PROJECT4_BGRCONVERTER_H
