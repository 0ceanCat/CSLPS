#ifndef PROJECT2_GOLOMB_H
#define PROJECT2_GOLOMB_H
#include "string"
#include "BitInputStream.h"
#include "BitOutputStream.h"
#include "vector"
#include "math.h"

using namespace std;

class Golomb {
private:
    int m;
    int fakeM = 8;
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
    void changeMode(int mode);
    void setM(int m);
    int countNecessaryBit(int val);
    const int changeToBestM(int bits);

    void close();
};


#endif
