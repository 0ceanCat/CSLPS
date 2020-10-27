#ifndef MYCLASS_H
#define MYCLASS_H
#include <QObject>
#include <QThread>

class Player;
class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);
    MyThread(Player* p);
    virtual void run();
private:
    Player* p;
};


#endif // MYCLASS_H
