#include "maintab.h"

MainTab::MainTab(Program * ptr, QWidget *parent) :
    QWidget(parent),
    program(ptr)
{
    mainControl = new MainControlWidget();
    pointList = new PointListWidget();
    actionList = new ActionListWidget();
    actionList->setPointListPtr(pointList);

    QSplitter * splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(pointList);
    splitter->addWidget(actionList);

    QSplitter * splitter0 = new QSplitter(this);
    splitter0->setOrientation(Qt::Horizontal);
    splitter0->addWidget(splitter);
    splitter0->addWidget(mainControl);

    QGridLayout *layout = new QGridLayout;

    layout->addWidget(splitter0, 0, 0);

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
