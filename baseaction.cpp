#include "baseaction.h"

BaseAction::BaseAction()
{
    calculated = false;
    done = false;
}

BaseAction::~BaseAction()
{

}

void BaseAction::calculate(Robot & robot)
{

}

void BaseAction::execute()
{

}

int BaseAction::size()
{

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
