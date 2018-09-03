#ifndef SERIALCOMMUNICATOR_H
#define SERIALCOMMUNICATOR_H

#include <iostream>
#include <QWidget>
#include <QDebug>

#include "serialport.h"
#include "comflags.h"
#include "thread.h"

class SerialCommunicator : public QWidget
{
    Q_OBJECT

    Flags * flags;

    Thread * thread;

    SerialPort * port;

    char byte;

public:
    SerialCommunicator(SerialPort * p, Flags * ptr);

public slots:
    void checkForFlags();
};

#endif // SERIALCOMMUNICATOR_H
