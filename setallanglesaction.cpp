#include "baseaction.h"


setAllAnglesAction::setAllAnglesAction(Lista<int> l,
                   SerialPort* sptr,
                   Flags* fptr)
{
    angles = l;
    setArduinoPortPtr(sptr);
    setFlagsPtr(fptr);
}

setAllAnglesAction::~setAllAnglesAction()
{

}

bool setAllAnglesAction::calculate(Robot & robot)
{
    for(int i = 0; i < static_cast<int>(angles.size()); i++)
    {
        robot.setThetaDeg(i, angles[i]);
    }

    robot.mapThetasToServos(servoDegs);

    robot.setTCPOrient(robot.getJointLocation(robot.getDOF() - 1) - robot.getTCPlocation());

#ifdef DEBUG_SETSINGLE
    qDebug() << "SetSingleJointAction::calculate(), koniec";
#endif

    setCalculated();
    resetDone();
    emit calculationsFinished();
    moveToThread(getParentThreadPtr());

    return true;
}

void setAllAnglesAction::execute()
{
    std::string s;

    s = "B";

    for (int j = 0; j < static_cast<int>(servoDegs.size()); j++)
    {
        s += std::to_string(servoDegs[j]) + "\n";
    }

    s += 'C';

    qDebug() << s.c_str();

    while (!flags()->isSet(ARDUINO_MOV_FIN)) ;

    *arduinoPort() << s;
    flags()->reset(ARDUINO_MOV_FIN);
    servoDegs.clear();
    setDone();
    resetCalculated();
}

int setAllAnglesAction::size()
{
    return 0;
}

void setAllAnglesAction::clear()
{
    servoDegs.clear();
    resetCalculated();
    resetDone();
}
