#include "program.h"

Program::Program() :
    arduinoPort(SerialPort("\\\\.\\COM4", 115200))
{

     if (arduinoPort.isConnected()) std::cout << "Connection Established" << std::endl;
     else std::cout << "ERROR, check port name";

     com = new SerialCommunicatorThread(&arduinoPort, &flags);

     connect(com, SIGNAL(bufferReadyToRead(std::string)), this, SLOT(print(std::string)));

     com->start();

     if(arduinoPort.isConnected())
     {
        arduinoPort << "A";

        QThread::msleep(3000);

        arduinoPort << "B350\n382\n347\n355\n364\n362\nC";

        QThread::msleep(3000);

        arduinoPort << "B200\n382\n347\n355\n364\n362\nC";


        std::cout << "after second sleep" << std::endl;

        if(flags.isSet(ARDUINO_MOV_FIN))
            std::cout << "flag set\n";
     }

     std::cout << "Program::Program() : koniec" << std::endl;

}


void Program::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_W && !(e->isAutoRepeat()))
    {
        qDebug() << "W pressed";

        arduinoPort << "B200\n382\n347\n355\n364\n300\nC";

        if(flags.isSet(ARDUINO_MOV_FIN))
            qDebug() << "Mov fin";
    }

    if(e->key() == Qt::Key_S)
    {
        qDebug() << "S pressed";
        char buf[MAX_DATA_LENGTH];

        if(arduinoPort.readSerialPort(buf, MAX_DATA_LENGTH) != 0)
        {
            qDebug() << "hehe";
            qDebug() << buf;
        }
    }
}

void Program::print(std::string s)
{
    qDebug() << s.c_str();
}


void Program::keyPressed(int key)
{
    if(key == Qt::Key_W)
    {
        qDebug() << "W pressed";

        arduinoPort << "B200\n382\n347\n355\n364\n300\nC";

        if(flags.isSet(ARDUINO_MOV_FIN))
            qDebug() << "Mov fin";
    }

    if(key == Qt::Key_S)
    {
        qDebug() << "S pressed";
        char buf[MAX_DATA_LENGTH];

        if(arduinoPort.readSerialPort(buf, MAX_DATA_LENGTH) != 0)
        {
            qDebug() << "hehe";
            qDebug() << buf;
        }
    }
}

Program::~Program()
{
    com->quit();
    com->wait();
}
