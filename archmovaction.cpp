#include "baseaction.h"

ArchMovAction::ArchMovAction(Eigen::Vector3d start,
                             Eigen::Vector3d inter,
                             Eigen::Vector3d dest,
                             SerialPort * port,
                             Flags * flags)
{
    starting = start;
    intermediate = inter;
    destination = dest;
    setArduinoPortPtr(port);
    setFlagsPtr(flags);
}

ArchMovAction::~ArchMovAction()
{

}

bool ArchMovAction::calculate(Robot & r)
{
    r.getDOF();

    return true;
}

void ArchMovAction::execute()
{

}

void ArchMovAction::clear()
{
    resetCalculated();
    resetDone();
}
