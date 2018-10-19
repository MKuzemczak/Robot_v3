#include "joystick.h"

Joystick::Joystick(QObject *parent) :
    QThread(parent)
{
    robot = nullptr;
    flags = nullptr;
    port = nullptr;
    sent = true;

}

Joystick::Joystick(Robot * rptr, Flags * fptr, SerialPort * pptr, QObject *parent) :
    QThread(parent)
{
    robot = rptr;
    flags = fptr;
    port = pptr;
    sent = true;
}

void Joystick::run()
{
    if(!checkPtrs())
    {
        stop();
        return;
    }

    while(!flags->isSet(STOP))
    {
        qDebug() << "Joystick::run() : poczatek petli";

        ConstTCPOrientAction action(robot->getTCPlocation(), robot->getTCPlocation() + 5*direction, port, flags);

        qDebug() << "Joystick::run() : po definicji";

        if(!action.calculate(*robot))
        {
            qDebug() << "error: Joystick::run() : błąd obliczeń";
            emit writeToTerminal("error: Joystick::run() : błąd obliczeń");
            stop();
            return;
        }

        while(!action.isDone())
            action.execute();

        qDebug() << "Joystick::run() : koniec petli";
    }

    stop();
}


void Joystick::startUp()
{
    if(!checkPtrs())
        return;

    flags->reset(STOP);

    direction << 0, 1, 0;

    start();
}

void Joystick::startDown()
{
    if(!checkPtrs())
        return;

    flags->reset(STOP);

    direction << 0, -1, 0;

    start();
}

void Joystick::startLeft()
{
    if(!checkPtrs())
        return;

    flags->reset(STOP);

    direction << 0, 0, -1;

    start();
}

void Joystick::startRight()
{
    if(!checkPtrs())
        return;

    flags->reset(STOP);

    direction << 0, 0, 1;

    start();
}

void Joystick::startFront()
{
    if(!checkPtrs())
        return;

    flags->reset(STOP);

    direction << 1, 0, 0;

    start();
}

void Joystick::startRear()
{
    if(!checkPtrs())
        return;

    flags->reset(STOP);

    direction << -1, 0, 0;

    start();
}

void Joystick::startAhead()
{
    if(!checkPtrs())
        return;

    flags->reset(STOP);

    direction = -1 * robot->getTCPOrient().normalized();

    start();
}

void Joystick::startDrawBack()
{
    if(!checkPtrs())
        return;

    flags->reset(STOP);

    direction = robot->getTCPOrient().normalized();

    start();
}

void Joystick::stop()
{
    flags->set(STOP);
    this->exit();
}

bool Joystick::checkPtrs()
{
    if(robot == nullptr || flags == nullptr || port == nullptr)
    {
        qDebug() << "error: Joystick : wskaźniki nie ustawione";
        return false;
    }

    return true;
}

////////////////////////////////////////////////// setters & getters & adders
void Joystick::setRobotPtr(Robot * ptr)
{
    robot = ptr;
}

void Joystick::setFlagsPtr(Flags * ptr)
{
    flags = ptr;
}

void Joystick::setSerialPortPtr(SerialPort * ptr)
{
    port = ptr;
}
