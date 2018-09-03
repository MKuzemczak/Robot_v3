#include "serialcommunicator.h"


SerialCommunicator::SerialCommunicator(SerialPort * p, Flags * ptr)
{
#ifdef DEBUG_SERIAL_COMMUNICATOR
    qDebug() << "SerialCommunicator::SerialCommunicator(...) : poczatek";
#endif



    port = p;
    flags = ptr;

    buffer = "";

    thread = new Thread();
    connect(thread, SIGNAL(loop()), this, SLOT(checkForFlags()));
    thread->start();



#ifdef DEBUG_SERIAL_COMMUNICATOR
    qDebug() << "SerialCommunicator::SerialCommunicator(...) : koniec";
#endif
}


void SerialCommunicator::checkForFlags()
{
    if(port->isConnected())
    {
        if(port->isDataToRead())
        {
            port->readSerialPort(&byte, 1);

            if(byte >= FLAG_OFFSET && byte < FLAG_OFFSET + NUM_OF_FLAGS)
            {
                flags->set(byte);

#ifdef DEBUG_SERIAL_COMMUNICATOR
                qDebug() << "SerialCommunicator::checkComFlags() : set flag: " << byte;
#endif
            }
            else
            {
                if(byte == '\n')
                    emit bufferReadyToRead();
                else
                {
                    buffer += byte;

#ifdef DEBUG_SERIAL_COMMUNICATOR
                    qDebug() << "SerialCommunicator::checkComFlags() : byte added to buffer: " << byte;
#endif
                }
            }
        }
    }

}


void SerialCommunicator::clearBuffer()
{
    buffer = "";
}

std::string SerialCommunicator::getBuffer()
{
    return buffer;
}
