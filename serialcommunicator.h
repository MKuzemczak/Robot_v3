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

    SerialPort * port;

    std::string buffer;

    char byte;

    bool connected;

    void run() override;

public:
    SerialCommunicatorThread(SerialPort * p, Flags * ptr);

    void clearBuffer();
    std::string getBuffer();
    bool isConnected();

public slots:


signals:
    void bufferReadyToRead(std::string);
    void portConnected();
    void portDisconnected();
    void writeToTerminal(QString);
    void writeToTerminal(int);
    void writeToTerminal(double);
    void writeToTerminal(char const *);
    void writeToTerminal(char);
    void writeToTerminal(std::string);
};

#endif // SERIALCOMMUNICATOR_H
