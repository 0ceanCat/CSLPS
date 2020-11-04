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
    bs.writeNbits(2020, 11);
    bs.flush();
    bs.close();
}

void writeStringTest(){
    BitOutputStream bs("C:/Users/Predator/Desktop/write");
    bs.writeString("Complementos sobre linguagens de programacao");
    bs.close();
}

void readBitTest(){
    BitInputStream is("C:/Users/Predator/Desktop/write");
    int bits;
    //conteudo do ficheiro: 2020 (em decimal)
    is.readBit(bits); // 1 0 0 0 0 0 0 0
    cout <<"readBit: " << bits << endl;
    is.readBit(bits); // 1 1 0 0 0 0 0 0
    cout <<"readBit: " << bits << endl;
    is.readBit(bits); // 1 1 1 0 0 0 0 0
    cout <<"readBit: " << bits << endl;
    is.readBit(bits); // 1 1 1 1 0 0 0 0
    cout <<"readBit: " <<  bits << endl;
    is.close();
}

void readNbitsTest(){
    BitInputStream is("C:/Users/Predator/Desktop/write");
    int bits;
    is.readNbits(bits, 11);
    cout <<"readNbits: " << bits << endl;
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
    //conteudo do ficheiro deve ser: 1 1 0 0 0 0 0 0  (192 em decimal)
    //writeBitTest();

    //escrever 11 bits mais significativos do numero 2020 no ficheiro
    //writeNbitsTest();

    //escrever o string "Complementos sobre linguagens de programacao" no ficheiro
    //writeStringTest();


    //conteudo do ficheiro: numero 2020
    //ler bit a bit, em total 4 bits do ficheiro
    //readBitTest();

    //ler 11 bits de uma vez so
    //readNbitsTest();

    //ler um string do ficheiro
    //Complementos sobre linguagens de programacao
    //readStringTest();


    return 0;
}
