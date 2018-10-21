#include "maintab.h"

MainTab::MainTab(Program * ptr, QWidget *parent) :
    QWidget(parent),
    program(ptr)
{
    terminal = new Terminal(this);
    mainControl = new MainControlWidget(ptr);
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

    connect(mainControl, SIGNAL(upPressed()), this, SLOT(terminalTest()));
    connect(mainControl, SIGNAL(addPointToList(int, int, int)), pointList, SLOT(addPoint(int, int, int)));
    connect(terminal, SIGNAL(hidden()), this, SLOT(resizeTerminal()));

    QGridLayout *layout = new QGridLayout;

    layout->addWidget(splitter1, 0, 0);

    this->setLayout(layout);

    connect(actionList, SIGNAL(actionAdded(ActionType, QString)), program, SLOT(addAction(ActionType, QString)));
    connect(actionList, SIGNAL(actionDeleted(int)), program, SLOT(deleteAction(int)));
    if(program->isSerialConnected())
        mainControl->setPortDiodeOn();

    connect(program, SIGNAL(writeToTerminal(QString)), terminal, SLOT(write(QString)));
    connect(program, SIGNAL(writeToTerminal(int)), terminal, SLOT(write(int)));
    connect(program, SIGNAL(writeToTerminal(double)), terminal, SLOT(write(double)));
    connect(program, SIGNAL(writeToTerminal(char)), terminal, SLOT(write(char)));
    connect(program, SIGNAL(writeToTerminal(char const *)), terminal, SLOT(write(char const *)));
    connect(program, SIGNAL(writeToTerminal(std::string)), terminal, SLOT(write(std::string)));

}

PointListWidget * MainTab::getPointList()
{
    return pointList;
}

void MainTab::terminalTest()
{
    *terminal << "hehehehhehe" << "hohoho\n";
    if(program->isSerialConnected())
        mainControl->setPortDiodeOn();
}

void MainTab::resizeTerminal()
{
    //terminal->setMaximumHeight(40);
}

void MainTab::scanConfig()
{
    QFile data("pointAndActionConfig.dat");

    if(data.open(QFile::ReadOnly))
    {
        QTextStream strm(&data);

        QStringList slist0, slist1;

        QString line;

        while(!strm.atEnd())
        {
            line = strm.readLine();

            slist0 = line.split(",");

            slist1 = slist0[1].split(" ");

            if(slist0[0] == "p")
            {
                if(slist1.size() != 3)
                {
                    qDebug() << "Błąd odczytu config listy punktów";
                }
                else
                {
                    pointList->addPoint(slist1[0].toInt(),
                            slist1[1].toInt(),
                            slist1[2].toInt());
                }
            }
            else if(slist0[0] == "a")
            {
                QString info;

                for(int i = 1; i < slist1.size(); i++)
                {
                    info += slist1[i];
                    if(i < slist1.size() - 1)
                        info += ",";
                }

                actionList->addAction(static_cast<ActionType>(slist1[0].toInt()), info);
            }
        }
    }

}
