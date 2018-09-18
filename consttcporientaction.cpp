#include "baseaction.h"

ConstTCPOrientAction::ConstTCPOrientAction(Eigen::Vector3d start,
                                           Eigen::Vector3d dest,
                                           SerialPort * port,
                                           Flags * flags)
{
    setType(CONST_STRAIGHT);

    starting = start;
    destination = dest;
    setArduinoPortPtr(port);
    setFlagsPtr(flags);
}

ConstTCPOrientAction::~ConstTCPOrientAction()
{

}

void ConstTCPOrientAction::calculate(Robot & robot)
{
    Lista<Eigen::Vector3d> path;
    Eigen::Vector3d orient;

    orient = robot.getTCPOrient();


    lerp(path);



    for (int i = 0; i < static_cast<int>(path.size()); i++)
    {
        Eigen::Vector3d jointLoc;

        jointLoc = path[i] + orient;

        robot.set(0, robot.getRegJointsAmount() - 1, robot.getDOF() - 1, jointLoc);

        robot.set(robot.getRegJointsAmount(), robot.getDOF() - 1, robot.getDOF(), path[i]);

        Lista<int> s;

        robot.mapThetasToServos(s);

        pathInServoDegs.push_back(s);

    }

    setCalculated();
    resetDone();
    emit calculationsFinished();
    moveToThread(getParentThreadPtr());
}

void ConstTCPOrientAction::execute()
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


void ConstTCPOrientAction::lerp(Lista<Eigen::Vector3d> & path)
{
    Eigen::Vector3d v = destination - starting;

    double vmod = sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));

    double number = vmod / 10;

    int loops = static_cast<int>(number);

    v.normalize();

    v *= 10;


    for (int i = 1; i < loops; i++)
    {
        path.push_back(starting + i*v);
    }

    path.push_back(destination);
}

int ConstTCPOrientAction::size()
{
    return static_cast<int>(pathInServoDegs.size());
}
