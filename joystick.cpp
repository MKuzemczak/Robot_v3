#include "joystick.h"

Joystick::Joystick(QObject *parent) :
    QThread(parent)
{
    robot = nullptr;
    flags = nullptr;
    port = nullptr;
    sent = true;
    move = false;

}

Joystick::Joystick(Robot * rptr, Flags * fptr, SerialPort * pptr, QObject *parent) :
    QThread(parent)
{
    robot = rptr;
    flags = fptr;
    port = pptr;
    sent = true;
    move = false;
}


void Joystick::run()
{
    if(!checkPtrs())
    {
        stop();
        return;
    }

    while(true)
    {
        if(move && !flags->isSet(STOP))
        {
            ConstTCPOrientAction action(robot->getTCPlocation(),
                                        robot->getTCPlocation() + 5*direction,
                                        9,
                                        port,
                                        flags);

            if(!flags->isSet(STOP))
            {
                if(!action.calculate(*robot))
                {
                    qDebug() << "error: Joystick::run() : błąd obliczeń";
                    emit writeToTerminal("error: Joystick::run() : błąd obliczeń");
                    stop();
                }
            }
            else
            {
                stop();
            }

            while(!flags->isSet(STOP) && move && !action.isDone())
            {
                action.execute();
                emit robotSet(static_cast<int>(robot->getTCPlocation()[0]),
                        static_cast<int>(robot->getTCPlocation()[1]),
                        static_cast<int>(robot->getTCPlocation()[2]));
            }
        }
        else
        {
            msleep(100);
        }
    }

    flags->set(STOP);
}


void Joystick::startUp()
{
    if(!checkPtrs())
        return;

    flags->reset(STOP);

    direction << 0, 1, 0;

    move = true;
}

void Joystick::startDown()
{
    if(!checkPtrs())
        return;

    flags->reset(STOP);

    direction << 0, -1, 0;

    move = true;
}

void Joystick::startLeft()
{
    if(!checkPtrs())
        return;

    flags->reset(STOP);

    direction << 0, 0, -1;

    move = true;
}

void Joystick::startRight()
{
    if(!checkPtrs())
        return;

    flags->reset(STOP);

    direction << 0, 0, 1;

    move = true;
}

void Joystick::startFront()
{
    if(!checkPtrs())
        return;

    flags->reset(STOP);

    direction << 1, 0, 0;

    move = true;
}

void Joystick::startRear()
{
    if(!checkPtrs())
        return;

    flags->reset(STOP);

    direction << -1, 0, 0;

    move = true;
}

void Joystick::startAhead()
{
    if(!checkPtrs())
        return;

    flags->reset(STOP);

    direction = -1 * robot->getTCPOrient().normalized();

    move = true;
}

void Joystick::startDrawBack()
{
    if(!checkPtrs())
        return;

    flags->reset(STOP);

    direction = robot->getTCPOrient().normalized();

    move = true;
}

void Joystick::stop()
{
    flags->set(STOP);
    move = false;
    //this->exit();
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
