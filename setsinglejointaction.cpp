#include "baseaction.h"

///#define DEBUG_SETSINGLE

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

bool SetSingleJointAction::calculate(Robot & robot)
{
    int currentThetaDeg = static_cast<int>(robot.getJointThetaRad(joint) / DEG_TO_RAD);
    Eigen::Vector3d constTCPlocation = robot.getTCPlocation();

    for(int i = currentThetaDeg;
        i != angleDeg;
        i += (angleDeg - currentThetaDeg)/abs(angleDeg - currentThetaDeg))
    {
#ifdef DEBUG_SETSINGLE
        qDebug() << "SetSingleJointAction::calculate(), loop, i == " << i;
#endif

        robot.setThetaDeg(joint, i);

        if(!robot.setExcluding(joint, robot.getDOF(), constTCPlocation))
        {
            emit calculationsFailed();
            moveToThread(getParentThreadPtr());
            return false;
        }

        Lista<int> s;

        robot.mapThetasToServos(s);

        pathInServoDegs.push_back(s);
    }

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

#ifdef DEBUG_SETSINGLE
    qDebug() << s.c_str();
    qDebug() << "SetSingleJointAction::pathInServoDegs.size() == " << static_cast<int>(pathInServoDegs.size());
#endif

    flags()->reset(ARDUINO_MOV_FIN);
    pathInServoDegs.erase(0);

    if (pathInServoDegs.size() == 0)
    {
        setDone();
        resetCalculated();
    }

}

int SetSingleJointAction::size()
{
    return static_cast<int>(pathInServoDegs.size());
}
