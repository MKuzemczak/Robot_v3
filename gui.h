#ifndef GUI_H
#define GUI_H

#include <QWidget>
#include <QThread>
#include <QKeyEvent>
#include <QPushButton>

#include <iostream>

#include "program.h"
#include "serialcommunicator.h"
#include "actionmanager.h"

class GUI : public QWidget
{
    Q_OBJECT

    QThread programThread;

    Program * program;

/////////////////////////////////////// GUI
    QPushButton * robotInit,
                * start;

/////////////////////////////////////// !GUI

public:
    explicit GUI(QWidget *parent = nullptr);
    ~GUI();


protected:
    void keyPressEvent(QKeyEvent *e);


signals:

    void keyPressed(int key);

public slots:
};

#endif // GUI_H
