#include "baseaction.h"

#define DEBUG_CONSTTCP

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

bool ConstTCPOrientAction::calculate(Robot & robot)
{
    Lista<Eigen::Vector3d, Eigen::aligned_allocator<Eigen::Vector3d>> path;
    Eigen::Vector3d orient;

    orient = robot.getTCPOrient();


    lerp(path);

#ifdef DEBUG_CONSTTCP
    qDebug() << "ConstTCPOrientAction::calculate(Robot&) : po lerp";
#endif

    for (int i = 0; i < static_cast<int>(path.size()); i++)
    {
        Eigen::Vector3d jointLoc;

        jointLoc = path[i] + orient;

        if(!robot.set(0, robot.getRegJointsAmount() - 1, robot.getDOF() - 1, jointLoc))
        {
            flags()->set(STOP);
            emit calculationsFailed();
            moveToThread(getParentThreadPtr());
            return false;
        }

        bool ret = true;

        Eigen::Vector3d v0, v1;
        v0 << path[i][0], 0, path[i][2];
        v1 << -orient[0], 0, -orient[2];
        if(asin(v0.cross(v1).norm())/(v0.norm()*v1.norm()) < DEG_TO_RAD)
        {
            qDebug() << "smol norm";
            ret = robot.set(robot.getRegJointsAmount() + 1, robot.getDOF() - 1, robot.getDOF(), path[i]);
        }
        else
        {
            ret = robot.set(robot.getRegJointsAmount(), robot.getDOF() - 1, robot.getDOF(), path[i]);
        }

        if(!ret)
        {
            flags()->set(STOP);
            emit calculationsFailed();
            moveToThread(getParentThreadPtr());
            return false;
        }

        Lista<int> s;

        robot.mapThetasToServos(s);

        pathInServoDegs.push_back(s);

    }

#ifdef DEBUG_CONSTTCP
    qDebug() << "ConstTCPOrientAction::calculate(Robot&) : end";
#endif

    setCalculated();
    resetDone();
    emit calculationsFinished();
    if(getParentThreadPtr() != nullptr)
        moveToThread(getParentThreadPtr());

    return true;
}

void ConstTCPOrientAction::execute()
{
#ifdef DEBUG_CONSTTCP
    qDebug() << "ConstTCPOrientAction::execute() : start";
#endif

    if(pathInServoDegs.size() != 0)
    {
        std::string s;


        s = "B";

        for (int j = 0; j < static_cast<int>(pathInServoDegs[0].size()); j++)
        {
            s += std::to_string(pathInServoDegs[0][j]) + "\n";


        }

        s += 'C';

#ifdef DEBUG_CONSTTCP
        qDebug() << "ConstTCPOrientAction::execute() : po petli";
#endif

        while (!flags()->isSet(ARDUINO_MOV_FIN)) ;

        *arduinoPort() << s;

#ifdef DEBUG_CONSTTCP
        qDebug() << s.c_str();
#endif


        flags()->reset(ARDUINO_MOV_FIN);
        pathInServoDegs.erase(0);
    }

    if (pathInServoDegs.size() == 0)
    {
        setDone();
        resetCalculated();
    }

}


void ConstTCPOrientAction::lerp(Lista<Eigen::Vector3d, Eigen::aligned_allocator<Eigen::Vector3d>> & path)
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


void ConstTCPOrientAction::clear()
{
    for(int i = 0; i < static_cast<int>(pathInServoDegs.size()); i++)
    {
        pathInServoDegs[i].clear();
    }

    pathInServoDegs.clear();
    resetCalculated();
    resetDone();
}
