#include "maincontrolwidget.h"

MainControlWidget::MainControlWidget(QWidget *parent) : QWidget(parent)
{
    runningDiode = new Diode("W ruchu", this);
    serialStateDiode = new Diode("Połączenie", this);



    QGridLayout * layout = new QGridLayout();

    layout->addWidget(runningDiode, 0, 0);
    layout->addWidget(serialStateDiode, 0, 2);
    layout->addWidget(createPointMovBox(), 2, 0, 1, 5);
    layout->addWidget(createButtonMovBox(), 3, 0, 1, 5);
    layout->addWidget(createSequenceControlBox(), 4, 0, 1, 5);

    /*for(int i = 0; i < layout->columnCount(); i++)
    {
        layout->setColumnStretch(i, 20);
    }*/

    setLayout(layout);
}


QGroupBox * MainControlWidget::createPointMovBox()
{
    QGroupBox * box = new QGroupBox("Ruch do punktu");

    setButton = new QPushButton("Ustaw", this);
    toListButton = new QPushButton("Dodaj do listy", this);

    setButton->setMaximumWidth(100);
    toListButton->setMaximumWidth(100);

    connect(setButton, SIGNAL(clicked()), this, SLOT(emitSet()));
    connect(toListButton, SIGNAL(clicked()), this, SLOT(emitPointToList()));

    xEdit = new QLineEdit(this);
    yEdit = new QLineEdit(this);
    zEdit = new QLineEdit(this);

    xEdit->setMaximumWidth(50);
    yEdit->setMaximumWidth(50);
    zEdit->setMaximumWidth(50);


    xLabel = new QLabel("X", this);
    yLabel = new QLabel("Y", this);
    zLabel = new QLabel("Z", this);

    xLabel->setMaximumWidth(10);
    yLabel->setMaximumWidth(10);
    zLabel->setMaximumWidth(10);

    QHBoxLayout * hbox0 = new QHBoxLayout,
            * hbox1 = new QHBoxLayout,
            * hbox2 = new QHBoxLayout,
            * hbox3 = new QHBoxLayout,
            * hbox4 = new QHBoxLayout;


    hbox0->addWidget(xLabel, Qt::AlignRight);
    hbox0->addWidget(xEdit);
    hbox1->addWidget(yLabel, Qt::AlignRight);
    hbox1->addWidget(yEdit);
    hbox2->addWidget(zLabel, Qt::AlignRight);
    hbox2->addWidget(zEdit);

    hbox3->addItem(hbox0);
    hbox3->addItem(hbox1);
    hbox3->addItem(hbox2);

    hbox4->addWidget(setButton);
    hbox4->addWidget(toListButton);

    QVBoxLayout * vbox = new QVBoxLayout;

    vbox->addItem(hbox3);
    vbox->addItem(hbox4);

    box->setLayout(vbox);
    box->setMaximumSize(500, 200);

    return box;
}

QGroupBox * MainControlWidget::createButtonMovBox()
{
    QGroupBox * box = new QGroupBox("Joystick");

    upButton = new QPushButton("Góra", this);
    downButton = new QPushButton("Dół", this);
    leftButton = new QPushButton("Lewo", this);
    rightButton = new QPushButton("Prawo", this);
    frontButton = new QPushButton("Przód", this);
    rearButton = new QPushButton("Tył", this);
    aheadButton = new QPushButton("Pchaj", this);
    drawBackButton = new QPushButton("Cofaj", this);

    upButton->setMaximumWidth(100);
    downButton->setMaximumWidth(100);
    leftButton->setMaximumWidth(100);
    rightButton->setMaximumWidth(100);
    frontButton->setMaximumWidth(100);
    rearButton->setMaximumWidth(100);
    aheadButton->setMaximumWidth(100);
    drawBackButton->setMaximumWidth(100);

    connect(upButton, SIGNAL(pressed()), this, SIGNAL(upPressed()));
    connect(upButton, SIGNAL(released()), this, SIGNAL(upReleased()));
    connect(downButton, SIGNAL(pressed()), this, SIGNAL(downPressed()));
    connect(downButton, SIGNAL(released()), this, SIGNAL(downReleased()));
    connect(leftButton, SIGNAL(pressed()), this, SIGNAL(leftPressed()));
    connect(leftButton, SIGNAL(released()), this, SIGNAL(leftReleased()));
    connect(rightButton, SIGNAL(pressed()), this, SIGNAL(rightPressed()));
    connect(rightButton, SIGNAL(released()), this, SIGNAL(rightReleased()));
    connect(frontButton, SIGNAL(pressed()), this, SIGNAL(frontPressed()));
    connect(frontButton, SIGNAL(released()), this, SIGNAL(frontReleased()));
    connect(rearButton, SIGNAL(pressed()), this, SIGNAL(rearPressed()));
    connect(rearButton, SIGNAL(released()), this, SIGNAL(rearReleased()));
    connect(aheadButton, SIGNAL(pressed()), this, SIGNAL(aheadPressed()));
    connect(aheadButton, SIGNAL(released()), this, SIGNAL(aheadReleased()));
    connect(drawBackButton, SIGNAL(pressed()), this, SIGNAL(drawBackPressed()));
    connect(drawBackButton, SIGNAL(released()), this, SIGNAL(drawBackReleased()));

    QGridLayout * grid0 = new QGridLayout;

    grid0->addWidget(upButton, 0, 2, 1, 3);
    grid0->addWidget(leftButton, 1, 0, 1, 3);
    grid0->addWidget(rightButton, 1, 4, 1, 3);
    grid0->addWidget(downButton, 2, 2, 1, 3);

    QVBoxLayout * vbox0 = new QVBoxLayout,
            * vbox1 = new QVBoxLayout;

    vbox0->addWidget(frontButton);
    vbox0->addWidget(rearButton);
    vbox1->addWidget(aheadButton);
    vbox1->addWidget(drawBackButton);

    QHBoxLayout * hbox = new QHBoxLayout;

    hbox->addItem(grid0);
    hbox->addItem(vbox0);
    hbox->addItem(vbox1);

    box->setLayout(hbox);
    box->setMaximumWidth(500);

    return box;

}

QGroupBox * MainControlWidget::createSequenceControlBox()
{
     QGroupBox * box = new QGroupBox("Kontrola sekwencji");

    runButton = new QPushButton("Start", this);
    stopButton = new QPushButton("Stop", this);

    connect(runButton, SIGNAL(clicked()), this, SIGNAL(runClicked()));
    connect(stopButton, SIGNAL(clicked()), this, SIGNAL(stopClicked()));

    runButton->setMaximumWidth(100);
    stopButton->setMaximumWidth(100);

    QHBoxLayout * hbox = new QHBoxLayout;

    hbox->addWidget(runButton);
    hbox->addWidget(stopButton);

    box->setLayout(hbox);
    box->setMaximumSize(500, 100);

    return box;

}

void MainControlWidget::emitPointToList()
{
    emit addPointToList(xEdit->text().toInt(), zEdit->text().toInt(), yEdit->text().toInt());
}

void MainControlWidget::emitSet()

{
    emit set(xEdit->text().toInt(), zEdit->text().toInt(), yEdit->text().toInt());
}
