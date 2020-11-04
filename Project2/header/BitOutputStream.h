//
// Created by Predator on 2020/10/28.
//

#ifndef PROJECT2_BITOUTPUTSTREAM_H
#define PROJECT2_BITOUTPUTSTREAM_H

#include <fstream>
#include "string"
using namespace std;

class BitOutputStream {
private:
    /**
     * O ponteiro para a posicao do bit
     */
    int position;

    /**
     * 1 char/uchar ocupa 1 byte na memoria, ou seja, 8 bits.
     * Funcionar como se fosse um vector de bits
     */
    unsigned char buffer;

    /**
     * Ofstream
     */
    ofstream output;


public:
    /**
     * Se usar este construtor ja nao precisa de invocar open()
     * @param outputFile : o ficheiro de output
     */
    BitOutputStream(string outputFile);

    /**
     * Construtor
     */
    BitOutputStream();

    /**
     * Escrever um bit no buffer.
     * Quando o buffer estiver cheio, escreve todos os bits do buffer no ficheiro
     * @param data: O bit que quer escrever. data deve ser o nuemro 0 ou 1
     */
    void writeBit(int data);

    /**
     * Escrever n bits menos significativos do data no buffer
     * @param data: Um numero ou uma letra ou uma character
     * @param n: Numero de bits que quer ler
     */
    void writeNbits(int data, int n);

    /**
     * Escrever o string no ficheiro
     * @param data: String que quer escrever
     */
    void writeString(string data);

    /**
     * Mostrar a sequencia binaria do buffer em string
     * @return buffer: Em string
     */
    string bufferToString();

    /**
     * Limpar o buffer e reset o position
     */
    void clear();

    /**
     * Abrir o ficheiro
     * @param filename: caminho do ficheiro
     */
    void open(string filename);

    /**
     * Escrever todos os bits do buffer no ficheiro, mesmo que o buffer não esteja cheio.
     */
    void flush();

    /**
     * Fechar o ficheiro
     */
    void close();
};


#endif //PROJECT2_BITOUTPUTSTREAM_H
