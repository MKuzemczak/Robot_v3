#include "maintab.h"

MainTab::MainTab(QWidget *parent) : QWidget(parent)
{
    pointList = new PointListWidget();
    actionList = new ActionListWidget();
    actionList->setPointListPtr(pointList);

    QSplitter * splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(pointList);
    splitter->addWidget(actionList);

    QGridLayout *layout = new QGridLayout;

    layout->addWidget(splitter, 0, 0);

    this->setLayout(layout);
}
