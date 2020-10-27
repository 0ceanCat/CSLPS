#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QObject>
#include <QThread>
#include "Player.h"

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);
    MyThread(Player* p);
    virtual void run();
public slots:
    void stopWork();
private:
    Player* p;
    bool stop;

};


#endif // MYTHREAD_H
