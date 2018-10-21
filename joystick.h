#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <QObject>
#include <QThread>
#include <QThreadPool>
#include <QDebug>

#include "eigenaddons.h"
#include "robot.h"
#include "comflags.h"
#include "baseaction.h"

class Joystick : public QThread
{
    Q_OBJECT

    Robot * robot;

    Flags * flags;

    SerialPort * port;

    Eigen::Vector3d direction;

    bool sent,
        move;

    void run() override;
    bool checkPtrs();

public:
    explicit Joystick(QObject *parent = nullptr);
    explicit Joystick(Robot * rptr, Flags * fptr, SerialPort * pptr, QObject *parent = nullptr);

    void setRobotPtr(Robot * ptr);
    void setFlagsPtr(Flags * ptr);
    void setSerialPortPtr(SerialPort * ptr);

signals:
    void writeToTerminal(QString);
    void writeToTerminal(int);
    void writeToTerminal(double);
    void writeToTerminal(char const *);
    void writeToTerminal(char);
    void writeToTerminal(std::string);
    void robotSet(int, int, int);


public slots:
    void startUp();
    void startDown();
    void startLeft();
    void startRight();
    void startFront();
    void startRear();
    void startAhead();
    void startDrawBack();
    void stop();

};

#endif // JOYSTICK_H
