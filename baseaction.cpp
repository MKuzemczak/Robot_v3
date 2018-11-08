#include "baseaction.h"

BaseAction::BaseAction()
{
    calculated = false;
    done = false;
    parentThreadPtr = nullptr;
}

BaseAction::BaseAction(QObject * parent) :
    QObject(parent)
{
    calculated = false;
    done = false;
    parentThreadPtr = nullptr;
}

BaseAction::~BaseAction()
{

}

bool BaseAction::calculate(Robot & robot)
{
    robot.getDOF();

    return true;
}

void BaseAction::calcSlot(Robot * robot)
{
    calculate(*robot);
}

void BaseAction::execute()
{

}

int BaseAction::size()
{
    return 0;
}

void BaseAction::setDone()
{
    done = true;
}
void BaseAction::resetDone()
{
    done = false;
}
bool BaseAction::isDone()
{
    return done;
}

void BaseAction::setType(ActionType t)
{
    type = t;
}

void BaseAction::setCalculated()
{
    calculated = true;
}

bool BaseAction::isCalculated()
{
    return calculated;
}

void BaseAction::resetCalculated()
{
    calculated = false;
}

void BaseAction::setArduinoPortPtr(SerialPort * ptr)
{
    arduinoPortPtr = ptr;
}

SerialPort * BaseAction::arduinoPort()
{
    return arduinoPortPtr;
}

void BaseAction::setFlagsPtr(Flags * ptr)
{
    flagsPtr = ptr;
}

Flags * BaseAction::flags()
{
    return flagsPtr;
}

void BaseAction::setParentThreadPtr(QThread * ptr)
{
    parentThreadPtr = ptr;
}

QThread * BaseAction::getParentThreadPtr()
{
    return parentThreadPtr;
}

void BaseAction::clear()
{

}

void BaseAction::moveToParentThread()
{
    moveToThread(parentThreadPtr);
}
