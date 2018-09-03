#ifndef SERIALCOMMUNICATOR_H
#define SERIALCOMMUNICATOR_H

#include <iostream>
#include <QWidget>
#include <QDebug>

#include "serialport.h"
#include "comflags.h"
#include "thread.h"

//#define DEBUG_SERIAL_COMMUNICATOR

class SerialCommunicator : public QWidget
{
    Q_OBJECT

    Flags * flags;

    Thread * thread;

    SerialPort * port;

    std::string buffer;

    char byte;

public:
    SerialCommunicator(SerialPort * p, Flags * ptr);

    void clearBuffer();
    std::string getBuffer();

public slots:
    void checkForFlags();

signals:
    void bufferReadyToRead();
};

#endif // SERIALCOMMUNICATOR_H
