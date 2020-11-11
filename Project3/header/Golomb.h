//
// Created by Predator on 2020/11/4.
//

#ifndef PROJECT2_GOLOMB_H
#define PROJECT2_GOLOMB_H
#include "string"
#include "BitInputStream.h"
#include "BitOutputStream.h"
#include "math.h"

using namespace std;

class Golomb {
private:
    int m;
    BitInputStream input;
    BitOutputStream output;
    string filepath;
    bool isPowerOf2();

public:
    Golomb(string filePath, int m);
    ~Golomb();
    void encode(int val);
    int decode();
    void flip();
};


#endif //PROJECT2_GOLOMB_H
