#include "mthread.h"
#include <QDebug>
#include <iostream>


MyThread::MyThread(Player* p)
{
    this->p = p;
}

void MyThread::run()
{

  /*  qDebug()<<"current thread ID:"<<QThread::currentThreadId()<<'\n';
     int count = 0;
        for(int i = 0;i!=10;++i)
        {
         ++count;
        }
        //发送结束信号
        emit myThreadSignal(count);
        exec();
        qDebug()<<"myThread run() start to execute";*/
    for(int i = 0;i!=10;++i)
    {
      std::cout << i << std::endl;

    }
}
