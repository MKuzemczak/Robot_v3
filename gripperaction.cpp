#include "baseaction.h"

#define DEBUG_GRIPPER

GripperAction::GripperAction(int set,
                             SerialPort * port,
                             Flags * flags)
{
    setType(GRIPPER);

    newSetting = set;
    setArduinoPortPtr(port);
    setFlagsPtr(flags);
}


GripperAction::~GripperAction()
{

}

bool GripperAction::calculate(Robot & robot)
{
    robot.setGripper(newSetting);

    servoIndex = 0;

    for(int i = 0; i< robot.getDOF(); i++)
    {
        servoIndex += robot.getJointServoAmount(i);
    }

    setCalculated();
    resetDone();
    emit calculationsFinished();
    moveToThread(getParentThreadPtr());

    return true;
}

void GripperAction::execute()
{
    std::string s;

    s = "D";
    s += std::to_string(servoIndex);
    s += "\nE";
    s += std::to_string(newSetting);
    s += '\n';

    *arduinoPort() << s;

    while (!flags()->isSet(ARDUINO_MOV_FIN)) ;

    *arduinoPort() << s;

#ifdef DEBUG_GRIPPER
    qDebug() << s.c_str();
#endif


    flags()->reset(ARDUINO_MOV_FIN);

    setDone();
    resetCalculated();
}

int GripperAction::size()
{
    return 0;
}

void GripperAction::clear()
{
    resetCalculated();
    resetDone();
}
