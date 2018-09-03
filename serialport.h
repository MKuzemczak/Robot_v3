#ifndef SERIALPORT_H
#define SERIALPORT_H

#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 255

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>

#include <QDebug>

class SerialPort
{
private:
    HANDLE handler;
    bool connected;
    COMSTAT status;
    DWORD errors;
public:
    SerialPort(char *portName, int baud);
    ~SerialPort();

    int readSerialPort(char *buffer, unsigned int buf_size);
    bool writeSerialPort(const char *buffer, unsigned int buf_size);
    bool isConnected();

    SerialPort & operator << (std::string & s);
    SerialPort & operator << (const char * s);
    SerialPort & operator << (int s);
    SerialPort & operator << (double s);
};

#endif // SERIALPORT_H
