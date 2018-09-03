#ifndef GUI_H
#define GUI_H

#include <QWidget>
#include <QThread>
#include <QKeyEvent>

#include <iostream>

#include "serialport.h"
#include "lista.h"
#include "comflags.h"
#include "serialcommunicator.h"

class GUI : public QWidget
{
    Q_OBJECT

    Flags flags;

    SerialPort arduinoPort;

    SerialCommunicator * com;

public:
    explicit GUI(QWidget *parent = nullptr);


protected:
    void keyPressEvent(QKeyEvent *e);


signals:

public slots:
};

#endif // GUI_H
