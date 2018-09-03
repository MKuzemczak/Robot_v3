#include "thread.h"


Thread::Thread()
{

}

void Thread::run()
{
    while(1)
    {
        emit loop();
        usleep(50000);
    }
}
