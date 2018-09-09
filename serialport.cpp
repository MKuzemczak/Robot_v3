#include "serialport.h"

SerialPort::SerialPort(std::string portName, int baud)
{
    this->connected = false;

    this->handler = CreateFileA(static_cast<LPCSTR>(portName.c_str()),
                                GENERIC_READ | GENERIC_WRITE,
                                0,
                                nullptr,
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL,
                                nullptr);
    if (this->handler == INVALID_HANDLE_VALUE){
        if (GetLastError() == ERROR_FILE_NOT_FOUND){
            printf("ERROR: Handle was not attached. Reason: %s not available\n", portName.c_str());
        }
    else
        {
            printf("ERROR!!!");
        }
    }
    else {
        DCB dcbSerialParameters = {};

        if (!GetCommState(this->handler, &dcbSerialParameters)) {
            printf("failed to get current serial parameters");
        }
        else
        {
            switch(baud)
            {
            case 9600:
                dcbSerialParameters.BaudRate = CBR_115200;
                break;
            case 115200:
                dcbSerialParameters.BaudRate = CBR_115200;
                break;
            }
            dcbSerialParameters.ByteSize = 8;
            dcbSerialParameters.StopBits = ONESTOPBIT;
            dcbSerialParameters.Parity = NOPARITY;
            dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

            if (!SetCommState(handler, &dcbSerialParameters))
            {
                printf("ALERT: could not set Serial port parameters\n");
            }
            else
            {
                this->connected = true;
                PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
                Sleep(ARDUINO_WAIT_TIME);
            }
        }
    }
}

SerialPort::~SerialPort()
{
    if (this->connected){
        this->connected = false;
        CloseHandle(this->handler);
    }
}

int SerialPort::readSerialPort(char *buffer, unsigned int buf_size)
{
    DWORD bytesRead;
    unsigned int toRead = 0;

    ClearCommError(this->handler, &this->errors, &this->status);

    if (this->status.cbInQue > 0){
        if (this->status.cbInQue > buf_size){
            toRead = buf_size;
        }
        else toRead = this->status.cbInQue;
    }

    memset(buffer, 0, buf_size);

    if (ReadFile(this->handler, buffer, toRead, &bytesRead, nullptr)) return static_cast<int>(bytesRead);

    return 0;
}

bool SerialPort::writeSerialPort(const char *buffer, unsigned int buf_size)
{
    DWORD bytesSend;

    if (!WriteFile(this->handler, (void*) buffer, buf_size, &bytesSend, nullptr)){
        ClearCommError(this->handler, &this->errors, &this->status);
        return false;
    }
    else return true;
}

bool SerialPort::isConnected()
{
    return this->connected;
}

bool SerialPort::isDataToRead()
{
    ClearCommError(this->handler, &this->errors, &this->status);

    if (this->status.cbInQue > 0)
        return true;

    return false;
}


SerialPort & SerialPort::operator << (std::string & s)
{
    writeSerialPort(s.c_str(), s.size());

    return *this;
}

SerialPort & SerialPort::operator << (const char * s)
{
    qDebug() << s;

    unsigned int i = 0;

    while(s[i] != '\0')
    {
        i++;
    }


    writeSerialPort(s, i);

    return *this;
}

SerialPort & SerialPort::operator << (int s)
{
    std::string a;

    a = std::to_string(s);

    writeSerialPort(a.c_str(), a.size());

    return *this;
}
SerialPort & SerialPort::operator << (double s)
{
    std::string a;

    a = std::to_string(s);

    writeSerialPort(a.c_str(), a.size());

    return *this;
}

SerialPort & SerialPort::operator << (const char s)
{
    writeSerialPort(&s, 1);

    return *this;
}
