#include "baseaction.h"

FreeMovAction::FreeMovAction(Eigen::Vector3d dest,
                             SerialPort * port,
                             Flags * flags)
{
    destination = dest;
    setArduinoPortPtr(port);
    setFlagsPtr(flags);

    setType(FREE);
}

FreeMovAction::~FreeMovAction()
{

}

bool FreeMovAction::calculate(Robot & robot)
{
    robot.setRegional(destination);

    robot.mapThetasToServos(destInServoDegs);

    emit calculationsFinished();
    moveToThread(getParentThreadPtr());

    return true;
}

void FreeMovAction::execute()
{

}


