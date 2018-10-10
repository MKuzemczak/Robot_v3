#include "maintab.h"

MainTab::MainTab(Program * ptr, QWidget *parent) :
    QWidget(parent),
    program(ptr)
{
    /*showTerminalButton = new QPushButton("Pokaż", this);
    showTerminalButton->setMaximumSize(100, 20);
    QGridLayout * grid = new QGridLayout;
    grid->addWidget(showTerminalButton, 0, 0);
    QWidget * fillWidget = new QWidget;
    grid->addWidget(fillWidget, 0, 1);
    QWidget * gridWidget = new QWidget;
    gridWidget->setLayout(grid);
    gridWidget->setMaximumHeight(40);*/

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
    /*splitter0->addWidget(gridWidget);
    showTerminalButton->hide();
    fillWidget->hide();
    gridWidget->hide();*/

    splitter1 = new QSplitter(this);
    splitter1->setOrientation(Qt::Horizontal);
    splitter1->addWidget(splitter);
    splitter1->addWidget(splitter0);

    connect(mainControl, SIGNAL(runClicked()), this, SLOT(terminalTest()));
    connect(terminal, SIGNAL(hidden()), this, SLOT(resizeTerminal()));
    /*connect(terminal, SIGNAL(hidden()), showTerminalButton, SLOT(show()));
    connect(terminal, SIGNAL(hidden()), fillWidget, SLOT(show()));
    connect(terminal, SIGNAL(hidden()), gridWidget, SLOT(show()));
    connect(showTerminalButton, SIGNAL(clicked()), terminal, SLOT(show()));
    connect(showTerminalButton, SIGNAL(clicked()), showTerminalButton, SLOT(hide()));
    connect(showTerminalButton, SIGNAL(clicked()), fillWidget, SLOT(hide()));
    connect(showTerminalButton, SIGNAL(clicked()), gridWidget, SLOT(hide()));*/


    QGridLayout *layout = new QGridLayout;

    layout->addWidget(splitter1, 0, 0);

    this->setLayout(layout);

    if(program != nullptr)
    {
        connect(actionList, SIGNAL(actionAdded(ActionType, QString)), program, SLOT(addAction(ActionType, QString)));
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
