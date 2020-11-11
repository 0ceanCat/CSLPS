#include <iostream>
#include "../header/Golomb.h"

using namespace std;

int main() {
   Golomb gb("C:/Users/Predator/Desktop/write", 5);

    for (int i = 0; i < 10; ++i) {
        int v = rand() % 255;
        gb.encode(v);
    }

    cout << endl;
    gb.flip();
    cout << endl;

    for (int i = 0; i < 10; ++i) {
        cout << "decoded value: " << gb.decode() << endl;
    }

    return 0;
}
