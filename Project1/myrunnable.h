#ifndef MYRUNNABLE_H
#define MYRUNNABLE_H
#include <QRunnable>
#include <iostream>
using namespace std;

class MyRunnable:public QRunnable
{
public:
       MyRunnable();
       ~MyRunnable();
       void run();
};


#endif // MYRUNNABLE_H
