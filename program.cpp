#include "program.h"

Program::Program() :
    arduinoPort(SerialPort("\\\\.\\COM4", 115200))
{

     if (arduinoPort.isConnected()) std::cout << "Connection Established" << std::endl;
     else std::cout << "ERROR, check port name";

     flags = new Flags(this);

     com = new SerialCommunicatorThread(&arduinoPort, flags);

     connect(com, SIGNAL(bufferReadyToRead(std::string)), this, SLOT(print(std::string)));

     com->start();

     manager = new ActionManager(flags, &arduinoPort, this);

     if(arduinoPort.isConnected())
     {
        arduinoPort << "A";

        QThread::msleep(3000);
     }

     std::cout << "Program::Program() : koniec" << std::endl;

     connect(flags, SIGNAL(movFinReceived()), manager, SLOT(nextStep()));



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

        arduinoPort << "B200\n382\n347\n355\n364\n300\nC";

        if(flags->isSet(ARDUINO_MOV_FIN))
            qDebug() << "Mov fin";
    }

    if(key == Qt::Key_S)
    {
        qDebug() << "S pressed";
        char buf[MAX_DATA_LENGTH];

        if(arduinoPort.readSerialPort(buf, MAX_DATA_LENGTH) != 0)
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

    arduinoPort << "B350\n382\n347\n355\n364\n362\n500\nC";

#ifdef PROGRAM_DEBUG
    qDebug() << "Program::testRobotInit() : end";
#endif

}

void Program::testRun()
{
#ifdef PROGRAM_DEBUG
    qDebug() << "Program::testRun() : start";
#endif

    //flags->set(LOOP);

    Eigen::Vector3d v0, v1, robotBase;

    robotBase = robot.getTCPlocation();

    v0 << 200, 50, 0;
    v1 << 150, 50, 0;

    manager->addConstTCPOrientAction(robotBase, v0);
    manager->addSetSingleJointAction(4, -150);
    manager->addConstTCPOrientAction(v0, v1);
    manager->addSetSingleJointAction(4, -179);
    manager->addGripperAction(300);
    v0 << 150, -30, 0;
    manager->addConstTCPOrientAction(v1, v0);
    manager->addGripperAction(520);
    manager->addConstTCPOrientAction(v0, v1);



    manager->setRobotPtr(&robot);

    manager->start();

#ifdef PROGRAM_DEBUG
    qDebug() << "Program::testRun() : end";
#endif
}

Program::~Program()
{
    com->quit();
    com->wait();
}
