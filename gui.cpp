#include "gui.h"

GUI::GUI(QWidget *parent) :
    QWidget(parent)
{
    program = new Program();
    program->moveToThread(&programThread);
    connect(this, SIGNAL(keyPressed(int)), program, SLOT(keyPressed(int)));
    programThread.start();
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
