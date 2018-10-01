#include "timer.h"

Timer::Timer()
{
    i = 0;
}

void Timer::run()
{
    while(1)
    {
        QThread::msleep(1000);
        i++;
    }
}
