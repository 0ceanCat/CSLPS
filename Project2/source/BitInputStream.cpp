//
// Created by Predator on 2020/10/28.
//

#include "../header/BitInputStream.h"
#include "string"

BitInputStream::BitInputStream(string inputFile) {
    this->position = 0;
    this->buffer = 0;
    this->input.open(inputFile, ios::binary);
}

BitInputStream::BitInputStream() {
    this->position = 0;
    this->buffer = 0;
}

void BitInputStream::readBit(unsigned char& data) {
    if (isEmpty()){
        if (input.eof())
            return;
        input.read((char*)&buffer, 1);
    }
    data |= buffer & (0x01 << position++);
    if(position == 8)
        clear();
}

void BitInputStream::readNbits(unsigned char& data, int n) {
    for (int i = 0; i < n && !input.eof(); i++){
        readBit(data);
    }
}

string BitInputStream::readString(int strLen) {
   string res="";

    for (int i = 0; i < strLen; i++){
        char c = readChar();
        if (c == '\0')
            return res;
        res.append(&c);
    }
    return res;
}

char BitInputStream::readChar(){
    if (!input.eof()){
        unsigned char c;
        readNbits(c, 8);
        return c;
    }
    return '\0';
}

void BitInputStream::open(string filePath) {
    input.open(filePath, ios::binary);
}

void BitInputStream::close() {
    input.close();
}

bool BitInputStream::isEmpty() {
    return buffer == 0;
}

void BitInputStream::clear() {
    position = 0;
    buffer = 0;
}


