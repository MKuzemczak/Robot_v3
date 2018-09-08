#ifndef SERIALCOMMUNICATOR_H
#define SERIALCOMMUNICATOR_H

#include <iostream>
#include <QWidget>
#include <QDebug>
#include <QThread>

#include "serialport.h"
#include "comflags.h"
#include "thread.h"

//#define DEBUG_SERIAL_COMMUNICATOR

class SerialCommunicatorThread : public QThread
{
    Q_OBJECT

    Flags * flags;

    //Thread * thread;

    SerialPort * port;

    std::string buffer;

    char byte;

    void run() override;

public:
    SerialCommunicatorThread(SerialPort * p, Flags * ptr);

    void clearBuffer();
    std::string getBuffer();

public slots:


signals:
    void bufferReadyToRead(std::string);
    void portDisconnected();
};

#endif // SERIALCOMMUNICATOR_H
