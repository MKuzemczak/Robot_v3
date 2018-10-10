#include "maintab.h"

MainTab::MainTab(Program * ptr, QWidget *parent) :
    QWidget(parent),
    program(ptr)
{
    terminal = new Terminal(this);
    mainControl = new MainControlWidget();
    pointList = new PointListWidget();
    actionList = new ActionListWidget();
    actionList->setPointListPtr(pointList);

    splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(pointList);
    splitter->addWidget(actionList);

    splitter0 = new QSplitter(this);
    splitter0->setOrientation(Qt::Vertical);
    splitter0->addWidget(mainControl);
    splitter0->addWidget(terminal);

    splitter1 = new QSplitter(this);
    splitter1->setOrientation(Qt::Horizontal);
    splitter1->addWidget(splitter);
    splitter1->addWidget(splitter0);

    connect(mainControl, SIGNAL(runClicked()), this, SLOT(terminalTest()));
    connect(mainControl, SIGNAL(addPointToList(int, int, int)), pointList, SLOT(addPoint(int, int, int)));
    connect(terminal, SIGNAL(hidden()), this, SLOT(resizeTerminal()));

    QGridLayout *layout = new QGridLayout;

    layout->addWidget(splitter1, 0, 0);

    this->setLayout(layout);

    if(program != nullptr)
    {
        connect(actionList, SIGNAL(actionAdded(ActionType, QString)), program, SLOT(addAction(ActionType, QString)));
        connect(mainControl, SIGNAL(runClicked()), program, SLOT(startSequence()));
        connect(program, SIGNAL(robotSet(int, int, int)), mainControl, SLOT(displayPoint(int, int, int)));

    }

}

PointListWidget * MainTab::getPointList()
{
    return pointList;
}

void MainTab::terminalTest()
{
    *terminal << "hehehehhehe" << "hohoho\n";
}

void MainTab::resizeTerminal()
{
    //terminal->setMaximumHeight(40);
}
