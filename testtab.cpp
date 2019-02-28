#include "testtab.h"

TestTab::TestTab(QWidget *parent) : QWidget(parent)
{

    robotInit = new QPushButton("Init", this);
    start = new QPushButton("Start", this);
    debugPauseButton = new QPushButton("Debug pause", this);

    robotInit->setGeometry(100, 100, 200, 100);
    start->setGeometry(320, 100, 200, 100);

    connect(robotInit, SIGNAL(pressed()), this, SIGNAL(initPressed()));
    connect(start, SIGNAL(pressed()), this, SIGNAL(startPressed()));
    connect(debugPauseButton, SIGNAL(clicked()), this, SLOT(debugPause()));

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(robotInit);
    layout->addWidget(start);
    layout->addWidget(debugPauseButton);
    this->setLayout(layout);
}

void TestTab::debugPause()
{
    qDebug() << "Debug pause";
}
