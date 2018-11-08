#include "program.h"

Program::Program()
{
    arduinoPort = new SerialPort("\\\\.\\COM4", 115200, this);

    if (arduinoPort->isConnected()) std::cout << "Connection Established" << std::endl;
    else std::cout << "ERROR, check port name";

    flags = new Flags(this);
    com = new SerialCommunicatorThread(arduinoPort, flags);
    connect(com, SIGNAL(bufferReadyToRead(std::string)), this, SLOT(print(std::string)));
    com->start();
    manager = new ActionManager(flags, arduinoPort, this);

    if(arduinoPort->isConnected())
    {
        *arduinoPort << "A";
        QThread::msleep(3000);
        emit portConnected();
    }

    std::cout << "Program::Program() : koniec" << std::endl;
    joystick = new Joystick(&robot, flags, arduinoPort, this);

    connect(flags, SIGNAL(movFinReceived()), manager, SLOT(nextStep()));
    connect(com, SIGNAL(portDisconnected()), this, SIGNAL(portDisconnected()));
    connect(com, SIGNAL(portConnected()), this, SIGNAL(portConnected()));

    connect(manager, SIGNAL(finished()), this, SIGNAL(stopped()));
    connect(manager, SIGNAL(finished()), this, SLOT(stop()));
    connect(manager, SIGNAL(stepped()), this, SIGNAL(anglesChanged()));
    connect(manager, SIGNAL(writeToTerminal(QString)), this, SIGNAL(writeToTerminal(QString)));
    connect(manager, SIGNAL(writeToTerminal(int)), this, SIGNAL(writeToTerminal(int)));
    connect(manager, SIGNAL(writeToTerminal(double)), this, SIGNAL(writeToTerminal(double)));
    connect(manager, SIGNAL(writeToTerminal(char)), this, SIGNAL(writeToTerminal(char)));
    connect(manager, SIGNAL(writeToTerminal(char const *)), this, SIGNAL(writeToTerminal(char const *)));
    connect(manager, SIGNAL(writeToTerminal(std::string)), this, SIGNAL(writeToTerminal(std::string)));

    connect(com, SIGNAL(writeToTerminal(QString)), this, SIGNAL(writeToTerminal(QString)));
    connect(com, SIGNAL(writeToTerminal(int)), this, SIGNAL(writeToTerminal(int)));
    connect(com, SIGNAL(writeToTerminal(double)), this, SIGNAL(writeToTerminal(double)));
    connect(com, SIGNAL(writeToTerminal(char)), this, SIGNAL(writeToTerminal(char)));
    connect(com, SIGNAL(writeToTerminal(char const *)), this, SIGNAL(writeToTerminal(char const *)));
    connect(com, SIGNAL(writeToTerminal(std::string)), this, SIGNAL(writeToTerminal(std::string)));

    connect(joystick, SIGNAL(robotSet(int, int, int)), this, SIGNAL(robotSet(int, int, int)));
    connect(joystick, SIGNAL(robotSet(int, int, int)), this, SIGNAL(anglesChanged()));
    joystick->start();

    pointList = nullptr;

    scanConfig();
}



void Program::print(std::string s)
{
    qDebug() << s.c_str();
}


void Program::keyPressed(int)
{
}

void Program::testRobotInit()
{
#ifdef PROGRAM_DEBUG
    qDebug() << "Program::testRObotInit() : start";
#endif

    *arduinoPort << robot.getBasePos();

    robot.setTCPOrient(robot.getJointLocation(robot.getDOF() - 1) - robot.getTCPlocation());

    manager->setRobotPtr(&robot);

    robotBase = robot.getTCPlocation();
    emit robotSet(static_cast<int>(robotBase(0)),
                  static_cast<int>(robotBase(1)),
                  static_cast<int>(robotBase(2)));

    emit writeToTerminal("testowa inicjalizacja zmiennej robot");

#ifdef PROGRAM_DEBUG
    qDebug() << "Program::testRobotInit() : end";

    qDebug() << "program thread(): " << thread() << "\nSerialPort thread(): " << arduinoPort->thread();
#endif

}

void Program::testRun()
{
#ifdef PROGRAM_DEBUG
    qDebug() << "Program::testRun() : start";
#endif

    //flags->set(LOOP);

    //manager->clear();

    Eigen::Vector3d v0, v1;

    v0 << 200, 50, 0;
    v1 << 150, 50, 0;

    manager->addConstTCPOrientAction(robotBase, v0);
    manager->addSetSingleJointAction(4, -150, true);
    manager->addConstTCPOrientAction(v0, v1);
    manager->addSetSingleJointAction(4, -179, true);
    manager->addGripperAction(400);
    v0 << 150, -30, 0;
    manager->addConstTCPOrientAction(v1, v0);
    manager->addGripperAction(550);
    manager->addConstTCPOrientAction(v0, v1);
    v0 << 150, 50, 50;
    manager->addConstTCPOrientAction(v1, v0);
    v1 << 150, -30, 50;
    manager->addConstTCPOrientAction(v0, v1);
    manager->addGripperAction(400);
    manager->addConstTCPOrientAction(v1, v0);
    v1 << 150, 50, 0;
    manager->addConstTCPOrientAction(v0, v1);
    manager->addSetSingleJointAction(4, -90, true);
    manager->addStraightLineMovAction(v1, robotBase);





    manager->start();

#ifdef PROGRAM_DEBUG
    qDebug() << "Program::testRun() : end";
#endif
}

void Program::addAction(ActionType type, QString info)
{
    if(pointList == nullptr)
    {
        emit writeToTerminal("error: Program::addAction(ActionType, QString) :\n"
                     "nullptr wskaźnik na listę punktów");

        return;
    }

    QStringList s = info.split(",");

    if(type == STRAIGHT_LINE || type == FREE || type == CONST_STRAIGHT)
    {
        int p1 = s.at(0).mid(1).toInt(),
                p2 = s.at(1).mid(1).toInt();

        Eigen::Vector3d start, end;
        start << (*pointList)(p1, 0), (*pointList)(p1, 1), (*pointList)(p1, 2);
        end << (*pointList)(p2, 0), (*pointList)(p2, 1), (*pointList)(p2, 2);

        switch(type)
        {
        case STRAIGHT_LINE:
            manager->addStraightLineMovAction(start, end);
            break;
        case FREE:
            manager->addFreeMovAction(end);
            break;
        case CONST_STRAIGHT:
            manager->addConstTCPOrientAction(start, end);
            break;
        default:
            ;
        }
    }
    else if (type == ARCH)
    {
        int p1 = s.at(0).mid(1).toInt(),
                p2 = s.at(1).mid(1).toInt(),
                p3 = s.at(2).mid(1).toInt();

        Eigen::Vector3d start, mid, end;
        start << (*pointList)(p1, 0), (*pointList)(p1, 1), (*pointList)(p1, 2);
        mid << (*pointList)(p2, 0), (*pointList)(p2, 1), (*pointList)(p2, 2);
        mid << (*pointList)(p3, 0), (*pointList)(p3, 1), (*pointList)(p3, 2);
    }
    else if (type == DELAY)
    {
        //int time = s.at(0).toInt();
    }
    else if(type == SINGLE)
    {
        int joint = s.at(0).toInt(),
                angle = s.at(1).toInt();
        bool constTCPlocation = s.at(2).toInt();

        qDebug() << "Angle: " << angle;

        manager->addSetSingleJointAction(joint, angle, constTCPlocation);
    }
    else if (type == GRIPPER)
    {
        manager->addGripperAction(s.at(0).toInt());
    }
    else if (type == ALL_ANGLES)
    {
        Lista<int> angles;

        for(int i = 0; i < static_cast<int>(s.size()); i++)
        {
            angles.push_back(s.at(i).toInt());
        }

        manager->addSetAllAnglesAction(angles);
    }
}

void Program::startSequence()
{
    flags->reset(STOP);
    manager->start();
    emit writeToTerminal("Program - start sekwencji");
    emit started();
}

void Program::stop()
{
    flags->set(STOP);
    manager->stop();
    emit writeToTerminal("Program - stop");
    emit stopped();
    emit robotSet(static_cast<int>(robot.getTCPlocation()[0]),
            static_cast<int>(robot.getTCPlocation()[1]),
            static_cast<int>(robot.getTCPlocation()[2]));
    emit anglesChanged();
}

bool Program::isSerialConnected()
{
    return arduinoPort->isConnected();
}

void Program::scanConfig()
{
    QFile data("save/robotConfig.dat");

    if(data.open(QFile::ReadOnly))
    {
        QTextStream strm(&data);
        QStringList slist0, slist1;
        QString line;

        while(!strm.atEnd())
        {
            line = strm.readLine();
            slist0 = line.split(",");

            for(int i = 1; i < slist0.size(); i++)
            {
                slist1 = slist0[i].split(" ");

                if(slist0[0] == "jr")
                {
                    if(slist1.size() != 3)
                    {
                        emit writeToTerminal(QString("Błąd odczytu config przegubów regionalnych: %1").arg(i));
                    }
                    else
                    {
                        qDebug() << "dodawanko reg";
                        robot.addRegJoint(slist1[0].toDouble(),
                                slist1[1].toDouble(),
                                slist1[2].toDouble());
                    }
                }
                else if (slist0[0] == "jl")
                {
                    if(slist1.size() != 3)
                    {
                        emit writeToTerminal(QString("Błąd odczytu config przegubów lokalnych: %1").arg(i));
                    }
                    else
                    {
                        qDebug() << "dodawanko loc";
                        robot.addLocJoint(slist1[0].toDouble(),
                                slist1[1].toDouble(),
                                slist1[2].toDouble());
                    }

                }
                else if (slist0[0] == "TCP")
                {
                    robot.setTCPaLength(slist0[1].toDouble());
                }
                else if (slist0[0] == "TH")
                {
                    if(slist1.size() != 2)
                    {
                        emit writeToTerminal(QString("Błąd odczytu config kątów Theta: %1").arg(i));
                    }
                    else
                    {
                        robot.setThetaDeg(slist1[0].toInt(),
                                slist1[1].toDouble());
                        robot.setJointBaseThetaDeg(slist1[0].toInt(),
                                slist1[1].toDouble());
                    }
                }
                else if (slist0[0] == "servo")
                {
                    if(slist1.size() != 3)
                    {
                        emit writeToTerminal(QString("Błąd odczytu config serwomechanizmów: %1").arg(i));
                    }
                    else
                    {
                        robot.addJointServoMinMax(slist1[0].toInt(),
                                slist1[1].toInt(),
                                slist1[2].toInt());
                    }

                }
                else if (slist0[0] == "constr")
                {
                    if(slist1.size() != 3)
                    {
                        emit writeToTerminal(QString("Błąd odczytu config kątów konstrukcyjnych: %1").arg(i));
                    }
                    else
                    {
                        robot.setJointConstructionMinMax(slist1[0].toInt(),
                                slist1[1].toInt(),
                                slist1[2].toInt());
                    }
                }
                else if (slist0[0] == "conv")
                {
                    if(slist1.size() != 3)
                    {
                        emit writeToTerminal(QString("Błąd odczytu config kątów konwersji: %1").arg(i));
                    }
                    else
                    {
                        robot.setJointConversionMinMax(slist1[0].toInt(),
                                slist1[1].toInt(),
                                slist1[2].toInt());
                    }
                }

            }

            if(slist0[0] == "gripper")
            {
                robot.setGripperMinMax(slist0[1].toInt(), slist0[2].toInt());
                robot.setGripper(slist0[3].toInt());
            }
            if (slist0[0] == "BASE")
            {
                QString s;
                if(slist0.size() != robot.getServoAmount() + 2)
                {
                    qDebug() << "Błąd odczytu config ustawienia startowego robota: " << slist0.size() << robot.getServoAmount();
                    emit writeToTerminal("Błąd odczytu config ustawienia startowego robota");
                }
                else
                {
                    for(int i = 1; i < slist0.size(); i++)
                    {
                        s += slist0[i];
                        if(i < slist0.size() - 1)
                            s += '\n';
                    }

                    robot.setBasePos(s);
                }

            }
        }
    }
}

void Program::deleteAction(int i)
{
    manager->deleteAction(i);
}

void Program::setRobot(int x, int y , int z)
{
    Eigen::Vector3d d;
    d << x, y, z;

    ConstTCPOrientAction action(robot.getTCPlocation(), d, arduinoPort, flags);

    if(!action.calculate(robot))
    {
        qDebug() << "error: Program::setRobot() : błąd obliczeń";
        emit writeToTerminal("error: Program::setRobot() : błąd obliczeń");
        return;
    }

    emit started();
    while(!action.isDone())
    {
        action.execute();
    }

    emit stopped();
    emit robotSet(x, y, z);
    emit anglesChanged();
}

int Program::getRobotDOF()
{
    return robot.getDOF();
}


void Program::setJointAngleDeg(int joint, int theta)
{
    Lista<int> l;
    std::string s;

    robot.setThetaDeg(joint, theta);
    robot.setTCPOrient(robot.getJointLocation(robot.getDOF() - 1) - robot.getTCPlocation());

    robot.mapThetasToServos(l);

    s = "B";

    for (int j = 0; j < static_cast<int>(l.size()); j++)
    {
        s += std::to_string(l[j]) + "\n";
    }

    s += 'C';

    while (!flags->isSet(ARDUINO_MOV_FIN)) ;

    *arduinoPort << s;
    flags->reset(ARDUINO_MOV_FIN);
    emit robotSet(static_cast<int>(robot.getTCPlocation()[0]),
            static_cast<int>(robot.getTCPlocation()[1]),
            static_cast<int>(robot.getTCPlocation()[2]));
}

void Program::setGripper(int s)
{
    GripperAction action(s, arduinoPort, flags);

    action.calculate(robot);
    action.execute();
}

void Program::setRobotToBase()
{
    Lista<int> l;
    std::string s;

    for(int i = 0; i < robot.getDOF(); i++)
    {
        robot.setThetaRad(i, robot.getJointBaseThetaRad(i));
    }

    robot.setTCPOrient(robot.getJointLocation(robot.getDOF() - 1) - robot.getTCPlocation());

    robot.mapThetasToServos(l);

    s = "B";

    for (int j = 0; j < static_cast<int>(l.size()); j++)
    {
        s += std::to_string(l[j]) + "\n";
    }

    s += 'C';

    while (!flags->isSet(ARDUINO_MOV_FIN)) ;

    *arduinoPort << s;
    flags->reset(ARDUINO_MOV_FIN);
    emit robotSet(static_cast<int>(robot.getTCPlocation()[0]),
            static_cast<int>(robot.getTCPlocation()[1]),
            static_cast<int>(robot.getTCPlocation()[2]));
    emit anglesChanged();
}

void Program::moveAction(int /*i*/, int oldI, int newI)
{
    manager->moveAction(oldI, newI);
    qDebug() << "Program: kolejność zmieniona";
    emit writeToTerminal("Kolejność akcji zmieniona");
}

void Program::toggleLoopFlag(bool b)
{
    if(b)
        flags->set(LOOP);
    else
        flags->reset(LOOP);
}

Program::~Program()
{


    com->quit();
    com->wait();
}
