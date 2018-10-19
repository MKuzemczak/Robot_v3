#include "serialcommunicator.h"


SerialCommunicatorThread::SerialCommunicatorThread(SerialPort * p, Flags * ptr)
{
#ifdef DEBUG_SERIAL_COMMUNICATOR
    qDebug() << "SerialCommunicator::SerialCommunicator(...) : poczatek";
#endif



    port = p;
    flags = ptr;

    buffer = "";

    //thread = new Thread();
    //connect(thread, SIGNAL(loop()), this, SLOT(checkForFlags()));
    //thread->start();



#ifdef DEBUG_SERIAL_COMMUNICATOR
    qDebug() << "SerialCommunicator::SerialCommunicator(...) : koniec";
#endif
}




void SerialCommunicatorThread::run()
{
    while(true)
    {
        if(port->isConnected())
        {
            if(!connected)
            {
                connected = true;

                emit portConnected();
            }

            if(port->isDataToRead())
            {
                port->readSerialPort(&byte, 1);

                if(byte >= FLAG_OFFSET && byte < FLAG_OFFSET + NUM_OF_FLAGS)
                {
                    flags->set(byte);

                    if(byte == ARDUINO_MOV_FIN)
                        qDebug() << "Mov fin received";

#ifdef DEBUG_SERIAL_COMMUNICATOR
                    qDebug() << "SerialCommunicator::checkComFlags() : set flag: " << byte;
#endif
                }
                else
                {
                    if(byte == '\n')
                    {
                        emit bufferReadyToRead(buffer);
                        clearBuffer();
                    }
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
        else
        {
            if(connected)
            {
                connected = false;
                emit portDisconnected();
            }
            msleep(1000);
            port->init();
        }
    }

}


void SerialCommunicatorThread::clearBuffer()
{
    buffer = "";
}

std::string SerialCommunicatorThread::getBuffer()
{
    return buffer;
}

bool SerialCommunicatorThread::isConnected()
{
    return port->isConnected();
}
