#include "addactiondialog.h"

AddActionDialog::AddActionDialog(QWidget * parent) :
    QDialog(parent)
{
    setFixedSize(300, 300);
    setWindowTitle("Dodaj akcję");

    typeList = new QComboBox(this);

    QStringList s = {"Linia prosta", "Wolny ruch", "Ruch po łuku", "Local change",
                    "Postój", "Pojedynczy przegub", "const TCP orient", "Chwytak"};

    typeList->addItems(s);

    connect(typeList, SIGNAL(currentIndexChanged(int)), this, SLOT(typeChanged(int)));

    startPointEdit = new QLineEdit(this);
    midPointEdit = new QLineEdit(this);
    endPointEdit = new QLineEdit(this);
    timeEdit = new QLineEdit(this);
    newAngleEdit = new QLineEdit(this);

    startPointLabel = new QLabel("Punkt startowy", this);
    midPointLabel = new QLabel("Punkt pośredni", this);
    endPointLabel = new QLabel("Punkt końcowy", this);
    timeLabel = new QLabel("Czas", this);
    newAngleLabel = new QLabel("Nowy kąt", this);

    addButton = new QPushButton("Dodaj", this);
    cancelButton = new QPushButton("Anuluj", this);

    connect(addButton, SIGNAL(clicked()), this, SLOT(addClicked()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QGridLayout * grid = new QGridLayout;

    grid->addWidget(typeList, 0, 0, 1, 3);
    grid->addWidget(startPointEdit, 1, 1, 1, 2);
    grid->addWidget(midPointEdit, 2, 1, 1, 2);
    grid->addWidget(endPointEdit, 3, 1, 1, 2);
    grid->addWidget(timeEdit, 4, 1, 1, 2);
    grid->addWidget(newAngleEdit, 5, 1, 1, 2);
    grid->addWidget(startPointLabel, 1, 0, Qt::AlignRight);
    grid->addWidget(midPointLabel, 2, 0, Qt::AlignRight);
    grid->addWidget(endPointLabel, 3, 0, Qt::AlignRight);
    grid->addWidget(timeLabel, 4, 0, Qt::AlignRight);
    grid->addWidget(newAngleLabel, 5, 0, Qt::AlignRight);
    grid->addWidget(addButton, 6, 1);
    grid->addWidget(cancelButton, 6, 2);

    setLayout(grid);

    typeChanged(0);
}

void AddActionDialog::typeChanged(int index)
{
    switch(index)
    {
    case STRAIGHT_LINE:
    case FREE:
    case CONST_STRAIGHT:
        disableAll();
        startPointEdit->setDisabled(false);
        endPointEdit->setDisabled(false);
        break;
    case ARCH:
        enableAll();
        timeEdit->setDisabled(true);
        newAngleEdit->setDisabled(true);
        break;
    case DELAY:
        disableAll();
        timeEdit->setDisabled(false);
        break;
    case GRIPPER:
    case SINGLE:
        disableAll();
        newAngleEdit->setDisabled(false);
        break;
    }
}

void AddActionDialog::enableAll()
{
    startPointEdit->setDisabled(false);
    midPointEdit->setDisabled(false);
    endPointEdit->setDisabled(false);
    timeEdit->setDisabled(false);
    newAngleEdit->setDisabled(false);
}

void AddActionDialog::disableAll()
{
    startPointEdit->setDisabled(true);
    midPointEdit->setDisabled(true);
    endPointEdit->setDisabled(true);
    timeEdit->setDisabled(true);
    newAngleEdit->setDisabled(true);
}

void AddActionDialog::setType(ActionType type)
{
    typeList->setCurrentIndex(type);
}

void AddActionDialog::setStart(QString s)
{
    startPointEdit->setText(s);
}

void AddActionDialog::setMid(QString s)
{
    midPointEdit->setText(s);
}

void AddActionDialog::setEnd(QString s)
{
    endPointEdit->setText(s);
}

void AddActionDialog::setTime(QString s)
{
    timeEdit->setText(s);
}

void AddActionDialog::setAngle(QString s)
{
    newAngleEdit->setText(s);
}

void AddActionDialog::setInfo(QStringList s)
{
    switch(typeList->currentIndex())
    {
    case STRAIGHT_LINE:
    case FREE:
    case CONST_STRAIGHT:
        startPointEdit->setText(s.at(0));
        endPointEdit->setText(s.at(1));
        break;
    case ARCH:
        startPointEdit->setText(s.at(0));
        midPointEdit->setText(s.at(1));
        endPointEdit->setText(s.at(2));
        break;
    case DELAY:
        timeEdit->setText(s.at(0));
        break;
    case GRIPPER:
    case SINGLE:
        newAngleEdit->setText(s.at(0));
        break;

    }
}

void AddActionDialog::addClicked()
{
    QString info;

    switch(typeList->currentIndex())
    {
    case STRAIGHT_LINE:
    case FREE:
    case CONST_STRAIGHT:
        info += startPointEdit->text();
        info += ",";
        info += endPointEdit->text();
        break;
    case ARCH:
        info += startPointEdit->text();
        info += ",";
        info += midPointEdit->text();
        info += ",";
        info += endPointEdit->text();
        break;
    case DELAY:
        info += timeEdit->text();
        break;
    case GRIPPER:
    case SINGLE:
        info += newAngleEdit->text();
        break;
    }

    emit addValues(stringToActionType(typeList->currentText()), info);
    accept();
}
