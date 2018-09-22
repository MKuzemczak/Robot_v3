#include "testtab.h"

TestTab::TestTab(QWidget *parent) : QWidget(parent)
{

    robotInit = new QPushButton("Init", this);
    start = new QPushButton("Start", this);

    robotInit->setGeometry(100, 100, 200, 100);
    start->setGeometry(320, 100, 200, 100);

    connect(robotInit, SIGNAL(pressed()), this, SIGNAL(initPressed()));
    connect(start, SIGNAL(pressed()), this, SIGNAL(startPressed()));

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(robotInit);
    layout->addWidget(start);
    this->setLayout(layout);
}
