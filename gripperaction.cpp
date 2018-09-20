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

void GripperAction::calculate(Robot & robot)
{
    robot.setGripper(newSetting);

    for(int i = 0; i< robot.getDOF(); i++)
    {
        index += robot.getJointServoAmount(i);
    }

    setCalculated();
    resetDone();
    emit calculationsFinished();
    moveToThread(getParentThreadPtr());
}

void GripperAction::execute()
{
    std::string s;

    s = "D";
    s += std::to_string(index);
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
