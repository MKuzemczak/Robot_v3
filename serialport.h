#ifndef SERIALPORT_H
#define SERIALPORT_H

#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 1000

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>

#include <QObject>
#include <QDebug>
#include <QString>

//#define DEBUG_SERIALPORT

class SerialPort : public QObject
{
    Q_OBJECT

private:
    HANDLE handler;
    bool connected;
    COMSTAT status;
    DWORD errors;

    std::string portName;
    int baud;
public:
    SerialPort(std::string portName, int baud);
    SerialPort(std::string portName, int baud, QObject * parent);
    ~SerialPort();

    int readSerialPort(char *buffer, unsigned int buf_size);
    bool writeSerialPort(const char *buffer, unsigned int buf_size);
    bool isConnected();
    bool isDataToRead();
    bool init();

    SerialPort & operator << (std::string & s);
    SerialPort & operator << (QString s);
    SerialPort & operator << (const char * s);
    SerialPort & operator << (int s);
    SerialPort & operator << (double s);
    SerialPort & operator << (const char s);
};

#endif // SERIALPORT_H
