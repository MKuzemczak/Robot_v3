#ifndef THREAD_H
#define THREAD_H


#include <QWidget>
#include <QThread>
#include <QDebug>

class Thread : public QThread
{
    Q_OBJECT
public:
    Thread();

    void run();

signals:
    void loop();
};

#endif // THREAD_H
