#include "maincontrolwidget.h"

MainControlWidget::MainControlWidget(Program * pr,
                                     QWidget *parent) :
    QWidget(parent),
    program(pr)
{
    runningDiode = new Diode("W ruchu", this);
    serialStateDiode = new Diode("Połączenie", this);



    QGridLayout * layout = new QGridLayout();

    layout->addWidget(runningDiode, 0, 0);
    layout->addWidget(serialStateDiode, 0, 2);
    layout->addWidget(createPointMovBox(), 2, 0, 1, 5);
    layout->addWidget(createButtonMovBox(), 3, 0, 1, 5);
    layout->addWidget(createSequenceControlBox(), 4, 0, 1, 5);
    layout->addWidget(createSliderBox(), 2, 5, 2, 1);
    layout->addWidget(createGripperBox(), 4, 5);

    setLayout(layout);

    connect(program, SIGNAL(robotSet(int, int, int)), this, SLOT(displayPoint(int, int, int)));
    connect(program, SIGNAL(portDisconnected()), serialStateDiode, SLOT(setOn()));
    connect(program, SIGNAL(portConnected()), serialStateDiode, SLOT(setOff()));
    connect(program, SIGNAL(stopped()), runningDiode, SLOT(setOff()));
    connect(program, SIGNAL(started()), runningDiode, SLOT(setOn()));
}


QGroupBox * MainControlWidget::createPointMovBox()
{
    QGroupBox * box = new QGroupBox("Ruch do punktu");

    setButton = new QPushButton("Ustaw", this);
    toListButton = new QPushButton("Dodaj do listy", this);
    setToBaseButton = new QPushButton("Do bazy", this);

    setButton->setMaximumSize(100, 20);
    toListButton->setMaximumSize(100, 20);
    setToBaseButton->setMaximumSize(100, 20);

    connect(setButton, SIGNAL(clicked()), this, SLOT(emitSet()));
    connect(this, SIGNAL(set(int, int, int)), program, SLOT(setRobot(int, int, int)));
    connect(toListButton, SIGNAL(clicked()), this, SLOT(emitPointToList()));
    connect(setToBaseButton, SIGNAL(clicked()), program, SLOT(setRobotToBase()));

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
    hbox4->addWidget(setToBaseButton);

    QVBoxLayout * vbox = new QVBoxLayout;

    vbox->addItem(hbox3);
    vbox->addItem(hbox4);

    box->setLayout(vbox);
    box->setMaximumSize(500, 100);

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

    upButton->setMaximumSize(100, 20);
    downButton->setMaximumSize(100, 20);
    leftButton->setMaximumSize(100, 20);
    rightButton->setMaximumSize(100, 20);
    frontButton->setMaximumSize(100, 20);
    rearButton->setMaximumSize(100, 20);
    aheadButton->setMaximumSize(100, 20);
    drawBackButton->setMaximumSize(100, 20);

    connect(upButton, SIGNAL(pressed()), program->joystick, SLOT(startUp()));
    connect(upButton, SIGNAL(released()), program, SLOT(stop()));
    connect(downButton, SIGNAL(pressed()), program->joystick, SLOT(startDown()));
    connect(downButton, SIGNAL(released()), program, SLOT(stop()));
    connect(leftButton, SIGNAL(pressed()), program->joystick, SLOT(startLeft()));
    connect(leftButton, SIGNAL(released()), program, SLOT(stop()));
    connect(rightButton, SIGNAL(pressed()), program->joystick, SLOT(startRight()));
    connect(rightButton, SIGNAL(released()), program, SLOT(stop()));
    connect(frontButton, SIGNAL(pressed()), program->joystick, SLOT(startFront()));
    connect(frontButton, SIGNAL(released()), program, SLOT(stop()));
    connect(rearButton, SIGNAL(pressed()), program->joystick, SLOT(startRear()));
    connect(rearButton, SIGNAL(released()), program, SLOT(stop()));
    connect(aheadButton, SIGNAL(pressed()), program->joystick, SLOT(startAhead()));
    connect(aheadButton, SIGNAL(released()), program, SLOT(stop()));
    connect(drawBackButton, SIGNAL(pressed()), program->joystick, SLOT(startDrawBack()));
    connect(drawBackButton, SIGNAL(released()), program, SLOT(stop()));

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
    box->setMaximumSize(500, 150);

    return box;

}

QGroupBox * MainControlWidget::createSequenceControlBox()
{
     QGroupBox * box = new QGroupBox("Kontrola sekwencji");

    runButton = new QPushButton("Start", this);
    stopButton = new QPushButton("Stop", this);
    loopCheckBox = new QCheckBox("Powtarzaj", this);

    connect(runButton, SIGNAL(clicked()), program, SLOT(startSequence()));
    connect(stopButton, SIGNAL(clicked()), program, SLOT(stop()));
    connect(loopCheckBox, SIGNAL(clicked(bool)), program, SLOT(toggleLoopFlag(bool)));

    runButton->setMaximumSize(100, 20);
    stopButton->setMaximumSize(100, 20);

    QHBoxLayout * hbox = new QHBoxLayout;

    hbox->addWidget(runButton);
    hbox->addWidget(stopButton);

    QVBoxLayout * vbox = new QVBoxLayout;

    vbox->addWidget(loopCheckBox);
    vbox->addItem(hbox);

    box->setLayout(vbox);
    box->setMaximumSize(500, 200);

    return box;

}

QGroupBox * MainControlWidget::createSliderBox()
{
    int dof = program->getRobotDOF();
    QGridLayout * grid = new QGridLayout;
    QHBoxLayout *hbox = new QHBoxLayout;

    for(int i = 0; i < dof; i++)
    {
        sliders.push_back(new QSlider(Qt::Horizontal, this));
        sliders[i]->setMinimum(program->robot.getJointConstructionMin(i));
        sliders[i]->setMaximum(program->robot.getJointConstructionMax(i));
        sliders[i]->setValue(static_cast<int>(program->robot.getJointThetaRad(i)/DEG_TO_RAD));
        connect(sliders[i], SIGNAL(sliderMoved(int)), this, SLOT(handleSliderAction()));

        sliderLabels.push_back(new QLabel(QString("Przegub %1:").arg(i), this));
        sliderLabels.push_back(new QLabel(QString("%1").arg(sliders[i]->minimum()), this));
        sliderLabels.push_back(new QLabel(QString("%1").arg(sliders[i]->maximum()), this));

        sliderLineEdits.push_back(new QLineEdit(this));
        sliderLineEdits[i]->setText(QString("%1").arg(sliders[i]->value()));
        sliderLineEdits[i]->setMaximumWidth(30);

        grid->addWidget(sliderLabels[i*3], i, 0);
        grid->addWidget(sliderLabels[i*3 + 1], i, 1);
        grid->addWidget(sliders[i], i, 2);
        grid->addWidget(sliderLabels[i*3 + 2], i, 3);
        grid->addWidget(sliderLineEdits[i], i, 4);
    }


    anglesToActionsButton = new QPushButton("Do akcji", this);
    anglesToActionsButton->setFixedSize(100, 20);
    hbox->addWidget(anglesToActionsButton);
    grid->addItem(hbox, dof, 0, 1, 5);

    connect(this, SIGNAL(sliderChanged(int, int)), program, SLOT(setJointAngleDeg(int, int)));
    connect(program, SIGNAL(anglesChanged()), this, SLOT(updateSliders()));
    connect(anglesToActionsButton, SIGNAL(clicked()), this, SLOT(emitAnglesToActions()));

    QGroupBox * box = new QGroupBox("Przeguby");

    box->setLayout(grid);
    box->setMinimumSize(100, dof*40);
    box->setMaximumSize(400, (dof+1)*40);

    return box;
}

QGroupBox * MainControlWidget::createGripperBox()
{
    QGroupBox * box = new QGroupBox("Chwytak");

    gripperSlider = new QSlider(Qt::Horizontal);
    gripperSlider->setMinimum(program->robot.getGripperMin());
    gripperSlider->setMaximum(program->robot.getGripperMax());
    gripperSlider->setValue(program->robot.getGripper());

    connect(gripperSlider, SIGNAL(sliderMoved(int)), program, SLOT(setGripper(int)));

    gripperToActionsButton = new QPushButton("Dodaj akcję", this);
    gripperToActionsButton->setMaximumSize(100, 20);
    connect(gripperToActionsButton, SIGNAL(clicked()), this, SLOT(emitGripperToActions()));

    QHBoxLayout * hbox = new QHBoxLayout;
    hbox->addWidget(gripperSlider);

    QHBoxLayout * hbox1 = new QHBoxLayout;
    hbox1->addWidget(gripperToActionsButton, Qt::AlignCenter);

    QGridLayout * grid = new QGridLayout;
    grid->addItem(hbox, 0, 0);
    grid->addItem(hbox1, 1, 0);

    box->setLayout(grid);
    box->setMaximumSize(400, 150);

    return box;
}

void MainControlWidget::emitPointToList()
{
    emit addPointToList(xEdit->text().toInt(), yEdit->text().toInt(), zEdit->text().toInt());
}

void MainControlWidget::emitSet()

{
    emit set(xEdit->text().toInt(), yEdit->text().toInt(), zEdit->text().toInt());
}

void MainControlWidget::displayPoint(int x, int y, int z)
{
    xEdit->setText(QString("%1").arg(x));
    yEdit->setText(QString("%1").arg(y));
    zEdit->setText(QString("%1").arg(z));

    gripperSlider->setValue(program->robot.getGripper());
}

void MainControlWidget::setPortDiodeOn()
{
    serialStateDiode->setOn();
}

void MainControlWidget::setPortDiodeOff()
{
    serialStateDiode->setOff();
}

void MainControlWidget::setRunningDiodeOn()
{
    runningDiode->setOn();
}

void MainControlWidget::setRunningDiodeOff()
{
    runningDiode->setOff();
}

void MainControlWidget::handleSliderAction()
{
    for(int i = 0; i < program->robot.getDOF(); i++)
    {
        QString s0(QString("%1").arg(sliders[i]->value()));

        if(s0 != sliderLineEdits[i]->text())
        {
            sliderLineEdits[i]->setText(s0);
            emit sliderChanged(i, sliders[i]->value());
        }
    }
}

void MainControlWidget::updateSliders()
{
    for(int i = 0; i < program->robot.getDOF(); i++)
    {
        sliders[i]->setValue(static_cast<int>(program->robot.getJointThetaRad(i)/DEG_TO_RAD));
        sliderLineEdits[i]->setText(QString("%1").arg(sliders[i]->value()));
    }

    gripperSlider->setValue(program->robot.getGripper());
}

void MainControlWidget::emitGripperToActions()
{
    emit gripperToActions(GRIPPER, QString("%1").arg(gripperSlider->value()));
}

void MainControlWidget::emitAnglesToActions()
{
    QString s;

    for(int i = 0; i < static_cast<int>(sliders.size()); i++)
    {
        s += QString("%1").arg(sliders[i]->value());
        s += ",";
    }

    s += QString::number(program->robot.getSpeed());

    emit anglesToActions(ALL_ANGLES, s);
}
