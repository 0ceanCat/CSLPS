#include "../header/BitOutputStream.h"
#include "iostream"
using namespace std;

BitOutputStream::BitOutputStream(string outputFile) {
    this->position = 7;
    this->buffer = 0;
    this->output.open(outputFile, ios::binary);
}

BitOutputStream::BitOutputStream() {
    this->position = 7;
    this->buffer = 0;
}


void BitOutputStream::writeBit(int data) {
    buffer |= data << position--;
    if (position == -1) {
        output.put(buffer);
        clear();
    }
}


void BitOutputStream::writeNbits(int data, int n) {
    if (n <= 0) return;

    for (int i = n - 1; i >= 0; i--){
        int bit = data & (0X01 << i);
        if(bit > 0)
            writeBit(1);
        else
            writeBit(0);
    }
}

string BitOutputStream::bufferToString() {
    string res = "";
    if (position == 0) return res;

    unsigned char max = 1 << 7;

    for (int i = 0; i < 8; i++){
        unsigned char leftBit = this->buffer & max >> i;
        if (leftBit > 0)
            res.append("1");
        else
            res.append("0");
    }
    return res;
}

void BitOutputStream::clear() {
    position = 7;
    buffer = 0;
}

void BitOutputStream::open(string filename) {
    output.open(filename, ios::binary);
}

void BitOutputStream::flush() {
    if (position == 7) return;
    output.put(buffer);
    clear();
}

void BitOutputStream::close() {
    if (position != 7)
        flush();
    output.close();
}

void BitOutputStream::writeString(string data) {
    for (char i : data){
        writeNbits(i,8);
    }
}
