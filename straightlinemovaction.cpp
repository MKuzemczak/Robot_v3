#include "baseaction.h"

//#define DEBUG_STRAIGHT_LINE_ACTION

StraightLineMovAction::StraightLineMovAction(Eigen::Vector3d start,
                                             Eigen::Vector3d dest,
                                             int spd,
                                             SerialPort * port,
                                             Flags * flags)
{
    setType(STRAIGHT_LINE);

    starting = start;
    destination = dest;
    speed = spd;
    setArduinoPortPtr(port);
    setFlagsPtr(flags);
}

bool StraightLineMovAction::calculate(Robot & robot)
{
    if(speed != robot.getSpeed())
    {
        robot.setSpeed(speed);
        speedChanged = true;
    }

    Lista<Eigen::Vector3d, Eigen::aligned_allocator<Eigen::Vector3d>> path;

    lerp(path);

#ifdef DEBUG_STRAIGHT_LINE_ACTION
    qDebug("Straight Line Action, calculate(), wykonano lerp\n");
#endif // DEBUG


    for (int i = 0; i < static_cast<int>(path.size()); i++)
    {
#ifdef DEBUG_STRAIGHT_LINE_ACTION
        qDebug("Straight Line Action, calculate(), poczatek petli\n");
#endif // DEBUG

        if(!robot.set(0, robot.getRegJointsAmount() - 1, robot.getDOF(), path[i]))
        {
            emit calculationsFailed();
            moveToThread(getParentThreadPtr());
            return false;
        }
        //robot.setRegional(path[i]);
        //robot.set(0, robot.getDOF() - 1, robot.getDOF(), path[i]); // temporal

#ifdef DEBUG_STRAIGHT_LINE_ACTION
        qDebug("Straight Line Action, calculate(), ustawiono czesc regionalna robota\n");
#endif // DEBUG

        robot.setTCPOrient(robot.getJointLocation(robot.getDOF() - 1) - robot.getTCPlocation());

        Lista<int> s;

        robot.mapThetasToServos(s);

#ifdef DEBUG_STRAIGHT_LINE_ACTION
        qDebug("straightLineMovAction::calculate() : zmapowano katy na serwa:\n");

        for (int i = 0; i < s.size(); i++)
        {
            qDebug("%d ", s[i]);
        }

        qDebug("\n\n");
#endif // DEBUG

        pathInServoDegs.push_back(s);

#ifdef DEBUG_STRAIGHT_LINE_ACTION
        qDebug("Straight Line Action, calculate(), koniec petli\n");
#endif // DEBUG

    }

    setCalculated();
    resetDone();
    emit calculationsFinished();
    moveToThread(getParentThreadPtr());

    return true;
}

void StraightLineMovAction::execute()
{
    if(speedChanged)
    {
        std::string s;

        s = "F";
        s += std::to_string(10-speed);
        s += "\n";

        while (!flags()->isSet(ARDUINO_MOV_FIN)) ;
        *arduinoPort() << s;
        speedChanged = false;
    }
    else
    {
        std::string s;

        s = "B";

        for (int j = 0; j < static_cast<int>(pathInServoDegs[0].size()); j++)
        {
            s += std::to_string(pathInServoDegs[0][j]) + "\n";

#ifdef DEBUG_STRAIGHT_LINE_ACTION
            qDebug("%d ", pathInServoDegs[0][j]);
#endif // DEBUG_STRAIGHT_LINE_ACTION

        }

        s += 'C';

#ifdef DEBUG_STRAIGHT_LINE_ACTION
        qDebug("\n");
#endif


        while (!flags()->isSet(ARDUINO_MOV_FIN)) ;

        *arduinoPort() << s;
        flags()->reset(ARDUINO_MOV_FIN);
        pathInServoDegs.erase(0);
    }

    if (pathInServoDegs.size() == 0)
    {
        setDone();
        resetCalculated();
    }

}

void StraightLineMovAction::lerp(Lista<Eigen::Vector3d, Eigen::aligned_allocator<Eigen::Vector3d>> & path)
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

int StraightLineMovAction::size()
{
    return static_cast<int>(pathInServoDegs.size());
}

void StraightLineMovAction::clear()
{
    for(int i = 0; i < static_cast<int>(pathInServoDegs.size()); i++)
    {
        pathInServoDegs[i].clear();
    }

    pathInServoDegs.clear();
    resetCalculated();
    resetDone();
}
