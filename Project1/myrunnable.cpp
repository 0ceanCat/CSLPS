#include "myrunnable.h"

MyRunnable::MyRunnable():QRunnable()
{
}

MyRunnable::~MyRunnable()
{
}


void MyRunnable::run()
{
   cout<<"dosomething ...."<< endl;

}
