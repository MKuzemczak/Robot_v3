#include "gui.h"

GUI::GUI(QWidget *parent) :
    QWidget(parent),
    arduinoPort(SerialPort("\\\\.\\COM4", 115200))
{
    setFixedSize(200, 100);

     if (arduinoPort.isConnected()) std::cout << "Connection Established" << std::endl;
     else std::cout << "ERROR, check port name";

     com = new SerialCommunicator(&arduinoPort, &flags);

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

     std::cout << "GUI::GUI() : koniec" << std::endl;

}


void GUI::keyPressEvent(QKeyEvent *e)
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
