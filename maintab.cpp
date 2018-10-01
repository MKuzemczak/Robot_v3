#include "maintab.h"

MainTab::MainTab(QWidget *parent) : QWidget(parent)
{
    label0 = new QLabel("Hello");
    label1 = new QLabel("there");
    actionList = new PointListWidget();

    QHBoxLayout *layout = new QHBoxLayout;

    layout->addWidget(label0);
    layout->addWidget(label1);
    layout->addWidget(actionList);

    this->setLayout(layout);
}
