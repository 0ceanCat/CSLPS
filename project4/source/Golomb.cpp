//
// Created by Predator on 2020/11/4.
//

#include "../header/Golomb.h"
#include "iostream"
using namespace  std;

Golomb::Golomb(string filePath, int m) {
    if (m <= 0)
        throw invalid_argument("m should be > 0");
    this->filepath = filePath;
    this->output.open(filePath, false);
    this->m = m;
}

Golomb::~Golomb(){
    this->input.close();
    if (this->output.isOpen()){
        this->output.flush();
    }
    this->output.close();
}

/**
 * Codificar o numero dado, e escreve o resultado no ficheiro
 * @param val o numero que quer codificar
 */
void Golomb::encode(int val) {
    if (val < 0){
        val = -val * 2 - 1;
    }else{
        val = val * 2;
    }

    int q = val / m;
    int r = val - q * m;
    output.writeNbits(1, q);
    output.writeBit(0);


    int b = log2(m);
    int rBits = b;

    if (!isPowerOf2()){
        b = ceil(log2(m));
        rBits = b - 1;
        int t = (0x01 << b) - m;
        if (r >= t){
            r = r + t;
            rBits = b;
        }
    }

    int numberOfbitsForR;
    if (r == 0)
        numberOfbitsForR = 1;
    else
        numberOfbitsForR = (int) log2(r) + 1;

    output.writeNbits(0, rBits - numberOfbitsForR);

    for (int i = numberOfbitsForR - 1; i >= 0; i--){
        unsigned int bit = r & (0x01 << i);
        if (bit > 0){
            output.writeBit(1);
        } else{
            output.writeBit(0);
        }
    }
}

/**
 * Ler o ficheiro e faz o decodificacao
 * @return o numero descodificado
 */
int Golomb::decode() {
    int bit = 1;
    int q = -1;
    while (bit != 0){
        bit = input.readBit();
        q++;
    }

    int b = ceil(log2(m));

    int t = (0x01 << b) - m;

    int r = 0;
    input.readNbits(r, b - 1);

    int val;
    if (r < t){
        val = q * m + r;
    }else{
        r = (r << 1) + input.readBit();
        val = q * m + r - t;
    }

    if (val % 2 == 0){
        val = val / 2;
    }else{
        val = (val + 1) / -2;
    }
    return val;
}

/**
 * verificar se o valor m dado for potencia de 2
 * @return true se for potencia de 2, false, caso contrario
 */
bool Golomb::isPowerOf2() {
    return (m & (m - 1)) == 0;
}

/**
 * Mudar o modo de operacao sobre o ficheiro
 * Se for modo de escrita, fecha o output e abre o input.
 * Se for modo de leitura, fecha o input e abre o output.
 */
void Golomb::flip() {
    if (output.isOpen()){
        if (!input.isOpen()){
            input.open(filepath);
            output.close();
        }else{
            output.close();
        }
        return;
    }

    if (input.isOpen()){
        if (!output.isOpen()){  
            output.open(filepath, false);
            input.close();
        }else{
            input.close();
        }
    }
}

/**
 * Mudar o modo de operacao sobre o ficheiro
 * @param mode : 0 leitura, 1 escrita
 */
void Golomb::changeMode(int mode) {
    if (mode){
        this->input.close();
        this->output.open(filepath, true);
        return;
    }

    this->output.close();
    this->input.open(filepath);
}

void Golomb::setM(int _m) {
    this->m = _m;
}
