//
// Created by Predator on 2020/11/25.
//

#include "Converter.h"

class BGRConverter : public Converter {
public:
    Mat encode(Mat &img) override{
        return img;
    }

    Mat decode(Mat &img) override{
        return img;
    }
};
