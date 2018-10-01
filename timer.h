#ifndef TIMER_H
#define TIMER_H

#include <QThread>

class Timer : public QThread
{
    Q_OBJECT

    int i;

    void run() override;
public:
    Timer();

    int time()
    {
        return i;
    }
};

#endif // TIMER_H
