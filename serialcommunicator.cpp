#include "serialcommunicator.h"

SerialCommunicator::SerialCommunicator(SerialPort * p, Flags * ptr)
{
    std::cout << "SerialCommunicator::SerialCommunicator(...) : poczatek" << std::endl;
    port = p;
    flags = ptr;

    thread = new Thread();
    connect(thread, SIGNAL(loop()), this, SLOT(checkForFlags()));
    //thread->start();
    std::cout << "SerialCommunicator::SerialCommunicator(...) : koniec" << std::endl;
}


void SerialCommunicator::checkForFlags()
{
    if(port->isConnected())
    {
        qDebug() << "SerialCommunicator::checkForFlags() : port connected";
        if(port->readSerialPort(&byte, MAX_DATA_LENGTH) != 0)
        {
            flags->set(byte);

            qDebug() << "Com thread read byte: " << byte;
        }
    }

    //qDebug() << "communicator thread check";
}
