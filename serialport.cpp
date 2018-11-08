#include "serialport.h"

SerialPort::SerialPort(std::string n, int b)
{
    this->connected = false;
    portName = n;
    baud = b;

    init();

}

SerialPort::SerialPort(std::string n, int b, QObject * parent) :
    QObject(parent)
{
    this->connected = false;
    portName = n;
    baud = b;

    init();
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
    LPDWORD errors = new DWORD;
    LPCOMSTAT comstat = new COMSTAT;

    if(ClearCommError(handler, errors, comstat) == 0)
    {
        connected = false;
    }
    else
    {
        connected = true;
    }

    delete errors;
    delete comstat;
    return connected;
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

#ifdef DEBUG_SERIALPORT
    qDebug() << "SerialPort::operator << : \n" << s.c_str() << "\n";
#endif

    return *this;
}

SerialPort & SerialPort::operator << (const char * s)
{
#ifdef DEBUG_SERIALPORT
    qDebug() << "SerialPort::operator << : \n" << s << "\n";
#endif

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
#ifdef DEBUG_SERIALPORT
    qDebug() << "SerialPort::operator << : \n" << s << "\n";
#endif

    std::string a;

    a = std::to_string(s);

    writeSerialPort(a.c_str(), a.size());

    return *this;
}
SerialPort & SerialPort::operator << (double s)
{
#ifdef DEBUG_SERIALPORT
    qDebug() << "SerialPort::operator << : \n" << s << "\n";
#endif

    std::string a;

    a = std::to_string(s);

    writeSerialPort(a.c_str(), a.size());

    return *this;
}

SerialPort & SerialPort::operator << (const char s)
{
#ifdef DEBUG_SERIALPORT
    qDebug() << "SerialPort::operator << : \n" << s << "\n";
#endif

    writeSerialPort(&s, 1);

    return *this;
}

SerialPort & SerialPort::operator << (QString s)
{
    writeSerialPort(s.toStdString().c_str(), static_cast<unsigned int>(s.size()));

    return *this;
}

bool SerialPort::init()
{
    this->handler = CreateFileA(static_cast<LPCSTR>(portName.c_str()),
                                GENERIC_READ | GENERIC_WRITE,
                                0,
                                nullptr,
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL,
                                nullptr);
    if (this->handler == INVALID_HANDLE_VALUE){
        if (GetLastError() == ERROR_FILE_NOT_FOUND){
            qDebug("ERROR: Handle was not attached. Reason: %s not available", portName.c_str());
            connected = false;
            return false;
        }
    else
        {
            qDebug("ERROR!!!");
            connected = false;
            return false;
        }
    }

    DCB dcbSerialParameters = {};

    if (!GetCommState(this->handler, &dcbSerialParameters)) {
        qDebug("failed to get current serial parameters");
        connected = false;
        return false;
    }

    switch(baud)
    {
    case 9600:
        dcbSerialParameters.BaudRate = CBR_9600;
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
        qDebug("ALERT: could not set Serial port parameters\n");
        connected = false;
        return false;
    }

    this->connected = true;
    PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
    Sleep(ARDUINO_WAIT_TIME);

    return true;
}
