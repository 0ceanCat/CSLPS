#ifndef PROJECT2_BITINPUTSTREAM_H
#define PROJECT2_BITINPUTSTREAM_H
#include <fstream>
#include "string"
using namespace std;

class BitInputStream {
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
     * Ifstream
     */
    ifstream input;
public:
    /**
     * Construtor
     */
    BitInputStream();

    /**
     * Se usar este construtor ja nao precisa de invocar open()
     * @param inputFile : O ficheiro de input
     */
    BitInputStream(string inputFile);

    /**
     *  Ler um byte do ficheiro para o buffer se o buffer for vazio.
     *  Ler um bit do buffer para data
     * @param data: O "container" onde o bit lido vai ficar
     */
    void readBit(unsigned char& data);

    /**
     * Ler n bits para o data
     * @param data:  O "container" onde os bits lidos vao ficar
     * @param n: Numero de bits que quer ler
     */
    void readNbits(unsigned char& data, int n);

    /**
     * Se o ficheiro tiver > strLen bytes, lê strLen letras (chars) e forma um string,
     * caso contrario, lê o que o ficheiro tem.
     * 0 <= comprimento do string retornado <= strLen
     * @param strLen:  Comprimento do string que quer ler
     * @return String lido
     */
    string readString(int strLen);

    /**
     * Se ainda não estiver no fim do ficheiro, lê um byte (char)
     * @return Char lido
     */
    char readChar();

    /**
     * Abrir o ficheiro
     * @param filePath: Caminho do ficheiro
     */
    void open(string filePath);

    /**
     * Fechar o ficheiro
     */
    void close();

    /**
     * Limpar o buffer e reset o position
     */
    void clear();

    /**
     * @return Se o buffer for vazio, true, caso contrario, false
     */
    bool isEmpty();
};
#endif