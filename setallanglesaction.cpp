#include "baseaction.h"


setAllAnglesAction::setAllAnglesAction(Lista<int> l,
                                       int spd,
                                       SerialPort* sptr,
                                       Flags* fptr)
{
    angles = l;
    speed = spd;
    setArduinoPortPtr(sptr);
    setFlagsPtr(fptr);
}

setAllAnglesAction::~setAllAnglesAction()
{

}

bool setAllAnglesAction::calculate(Robot & robot)
{
    if(speed != robot.getSpeed())
    {
        robot.setSpeed(speed);
        speedChanged = true;
    }

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

    if(speedChanged)
    {
        s = "F";
        s += std::to_string(10-speed);
        s += "\n";

        while (!flags()->isSet(ARDUINO_MOV_FIN)) ;
        *arduinoPort() << s;
        speedChanged = false;
    }

    s = "B";

    for (int j = 0; j < static_cast<int>(servoDegs.size()); j++)
    {
        s += std::to_string(servoDegs[j]) + "\n";
    }

    s += 'C';

    while (!flags()->isSet(ARDUINO_MOV_FIN)) ;

    //qDebug() << endl << s.c_str() << endl;
    QThread::msleep(10);
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
