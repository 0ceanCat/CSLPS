#include <iostream>
#include "../header/BitOutputStream.h"
#include "../header/BitInputStream.h"
using namespace std;

void writeBitTest(){
    BitOutputStream bs;
    bs.open("C:/Users/Predator/Desktop/write");
    bs.writeBit(1);
    bs.writeBit(1);
    bs.flush();
    bs.close();
}

void writeNbitsTest(){
    BitOutputStream bs;
    bs.open("C:/Users/Predator/Desktop/write");
    bs.writeNbits(11, 4);
    bs.flush();
    bs.close();
}

void writeStringTest(){
    BitOutputStream bs("C:/Users/Predator/Desktop/write");
    bs.writeString("Complementos sobre linguagens de programacao");
    bs.close();
}

void readBitTest(){
    BitInputStream is("C:/Users/Predator/Desktop/read");
    unsigned char bits;
    //conteudo: 0 0 0 0 1 0 1 1
    is.readBit(bits); // 0 0 0 0 0 0 0 1
    cout <<"readBit: " << (int) bits << endl;
    is.readBit(bits); // 0 0 0 0 0 0 1 1
    cout <<"readBit: " << (int) bits << endl;
    is.readBit(bits); // 0 0 0 0 0 0 1 1
    cout <<"readBit: " << (int) bits << endl;
    is.readBit(bits); // 0 0 0 0 1 0 1 1
    cout <<"readBit: " << (int) bits << endl;
    is.close();
}

void readNbitsTest(){
    BitInputStream is("C:/Users/Predator/Desktop/read");
    unsigned char bits;
    // 0 0 0 0 1 1 1 1
    is.readNbits(bits, 4);
    cout <<"readNbits: " << (int) bits << endl;
    is.close();
}

void readStringTest(){
    BitInputStream is("C:/Users/Predator/Desktop/write");
    string str = is.readString(50);
    cout << str << endl;
    is.close();
}

int main() {
    //escrever 2 bits (1 e 1) no ficheiro
    // 1 1 000
    // 00 11
   // writeBitTest();

    //escrever 4 bits menos significativos do numero 11 no ficheiro
    //0 0 0 0 1 0 1 1
    //writeNbitsTest();

    //escrever o string "Complementos sobre linguagens de programacao" no ficheiro
    //writeStringTest();


    //conteudo do ficheiro: numero 11
    //0 0 0 0 1 0 1 1
    //ler bit a bit, em total 4 bits do ficheiro
    //os resultados devem ser: 1, 3, 3, 11
    //readBitTest();

    //ler 4 bits de uma vez so
    //readNbitsTest();

    //ler um string do ficheiro
    //Complementos sobre linguagens de programacao
    readStringTest();

    return 0;
}
