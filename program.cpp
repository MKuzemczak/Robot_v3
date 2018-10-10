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
    }

    std::cout << "Program::Program() : koniec" << std::endl;

    connect(flags, SIGNAL(movFinReceived()), manager, SLOT(nextStep()));

    pointList = nullptr;

}



void Program::print(std::string s)
{
    qDebug() << s.c_str();
}


void Program::keyPressed(int key)
{
    if(key == Qt::Key_W)
    {
        qDebug() << "W pressed";

        *arduinoPort << "B200\n382\n347\n355\n364\n300\nC";

        if(flags->isSet(ARDUINO_MOV_FIN))
            qDebug() << "Mov fin";
    }

    if(key == Qt::Key_S)
    {
        qDebug() << "S pressed";
        char buf[MAX_DATA_LENGTH];

        if(arduinoPort->readSerialPort(buf, MAX_DATA_LENGTH) != 0)
        {
            qDebug() << "hehe";
            qDebug() << buf;
        }

        qDebug() << "Threads:\nMain: " << QApplication::instance()->thread()
                 << "\nProgram: " << this->thread()
                 << "\nmanager: " << manager->thread();
    }
}

void Program::testRobotInit()
{
#ifdef PROGRAM_DEBUG
    qDebug() << "Program::testRObotInit() : start";
#endif

    //robot = Robot();

    robot.addRegJoint(-90, 0, 0);
    robot.addRegJoint(90, 0, 0);
    robot.addRegJoint(0, 150, 0);
    robot.addLocJoint(90, 0, 0);
    robot.addLocJoint(-90, 0, 93);
    robot.setTCPaLength(90);

    robot.setThetaDeg(1, 90);
    robot.setThetaDeg(4, -90);

    robot.addJointServoMinMax(0, 555, 150);
    robot.addJointServoMinMax(1, 178, 593);
    robot.addJointServoMinMax(1, 545, 154);
    robot.addJointServoMinMax(2, 560, 160);
    robot.addJointServoMinMax(3, 570, 165);
    robot.addJointServoMinMax(4, 140, 590);

    robot.setJointConstructionMinMax(0, -90, 90);
    robot.setJointConstructionMinMax(1, 20, 150);
    robot.setJointConstructionMinMax(2, -45, 45);
    robot.setJointConstructionMinMax(3, -90, 90);
    robot.setJointConstructionMinMax(4, -180, 0);

    robot.setJointConversionMinMax(0, -90, 90);
    robot.setJointConversionMinMax(1, 0, 180);
    robot.setJointConversionMinMax(2, -90, 90);
    robot.setJointConversionMinMax(3, -90, 90);
    robot.setJointConversionMinMax(4, -180, 0);

    robot.setTCPOrient(robot.getJointLocation(robot.getDOF() - 1) - robot.getTCPlocation());

    *arduinoPort << "B350\n382\n347\n355\n364\n362\n500\nC";

    manager->setRobotPtr(&robot);

    robotBase = robot.getTCPlocation();
    emit robotSet(static_cast<int>(robotBase(0)),
                  static_cast<int>(robotBase(1)),
                  static_cast<int>(robotBase(2)));

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
    manager->addSetSingleJointAction(4, -150);
    manager->addConstTCPOrientAction(v0, v1);
    manager->addSetSingleJointAction(4, -179);
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
    manager->addSetSingleJointAction(4, -90);
    manager->addStraightLineMovAction(v1, robotBase);





    manager->start();

#ifdef PROGRAM_DEBUG
    qDebug() << "Program::testRun() : end";
#endif
}

void Program::addAction(ActionType type, QString info)
{
    qDebug() << "Program::addAction(..) : type == " << type << ", info == " << info;

    if(pointList == nullptr)
    {
        qDebug() << "error: Program::addAction(ActionType, QString) :\n"
                    "nullptr wskaźnik na listę punktów";
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
            qDebug() << "adding constTCPaction";
            break;
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

        manager->addSetSingleJointAction(joint, angle);
    }
    else if (type == GRIPPER)
    {
        manager->addGripperAction(s.at(0).toInt());
    }
}

void Program::startSequence()
{
    manager->start();
}

void Program::stop()
{

}

Program::~Program()
{
    com->quit();
    com->wait();
}
