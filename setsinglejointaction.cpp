#include "baseaction.h"

SetSingleJointAction::SetSingleJointAction(int j, int deg,
                                           SerialPort * port,
                                           Flags * flags)
{
    setType(SINGLE);

    joint = j;
    angleDeg = deg;
    setArduinoPortPtr(port);
    setFlagsPtr(flags);
}

SetSingleJointAction::~SetSingleJointAction()
{

}

void SetSingleJointAction::calculate(Robot & robot)
{
    robot.setThetaDeg(joint, angleDeg);

    //robot.mapThetasToServos(servoSignals);

    int currentThetaDeg = static_cast<int>(robot.getJointThetaRad(joint) / DEG_TO_RAD);

    for(int i = currentThetaDeg;
        i != angleDeg;
        i += (angleDeg - currentThetaDeg)/abs(angleDeg - currentThetaDeg))
    {
        robot.setThetaDeg(joint, i);

        // tutaj trzeba wstawić robot.setExcluding(),
        // czyli ustawienie TCP robota bez ruszania wybranym przegubem.
    }

    robot.setTCPOrient(robot.getJointLocation(robot.getDOF() - 1) - robot.getTCPlocation());

    setCalculated();
    resetDone();
    emit calculationsFinished();
    moveToThread(getParentThreadPtr());
}

void SetSingleJointAction::execute()
{
    std::string s;


    s = "B";

    for (int j = 0; j < static_cast<int>(pathInServoDegs[0].size()); j++)
    {
        s += std::to_string(pathInServoDegs[0][j]) + "\n";


    }

    s += 'C';

    while (!flags()->isSet(ARDUINO_MOV_FIN)) ;

    *arduinoPort() << s;

    flags()->reset(ARDUINO_MOV_FIN);
    pathInServoDegs.erase(0);

    if (pathInServoDegs.size() == 0)
    {
        setDone();
        resetCalculated();
    }

}
