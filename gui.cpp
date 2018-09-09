#include "gui.h"

GUI::GUI(QWidget *parent) :
    QWidget(parent)
{
    setFixedSize(620, 300);

    program = new Program();
    program->moveToThread(&programThread);
    connect(this, SIGNAL(keyPressed(int)), program, SLOT(keyPressed(int)));
    programThread.start();

    robotInit = new QPushButton("Init", this);
    start = new QPushButton("Start", this);

    robotInit->setGeometry(100, 100, 200, 100);
    start->setGeometry(320, 100, 200, 100);

    connect(robotInit, SIGNAL(pressed()), program, SLOT(testRobotInit()));
    connect(start, SIGNAL(pressed()), program, SLOT(testRun()));
}


void GUI::keyPressEvent(QKeyEvent *e)
{
    emit keyPressed(e->key());

}

GUI::~GUI()
{
    programThread.quit();
    programThread.wait();
}
