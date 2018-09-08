#ifndef PROGRAM_H
#define PROGRAM_H

#include <QWidget>
#include <QThread>
#include <QKeyEvent>

#include <iostream>

#include "serialcommunicator.h"
#include "actionmanager.h"

Q_DECLARE_METATYPE(std::string)

class Program : public QObject
{
    Q_OBJECT

    Flags flags;

    SerialPort arduinoPort;

    SerialCommunicatorThread * com;

    ActionManager * manager;

    Robot * robot;

public:
    Program();
    ~Program();


protected:
    void keyPressEvent(QKeyEvent *e);


signals:

public slots:
    void print(std::string s);
    void keyPressed(int);
};

#endif // PROGRAM_H
