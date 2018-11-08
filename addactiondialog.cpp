#include "addactiondialog.h"

AddActionDialog::AddActionDialog(QWidget * parent) :
    QDialog(parent)
{
    setFixedSize(300, 300);
    setWindowTitle("Dodaj akcję");

    typeList = new QComboBox(this);

    QStringList s = {"Linia prosta", "Wolny ruch", "Ruch po łuku", "Local change",
                    "Postój", "Pojedynczy przegub", "const TCP orient", "Chwytak",
                    "Wszystkie kąty"};

    typeList->addItems(s);

    connect(typeList, SIGNAL(currentIndexChanged(int)), this, SLOT(typeChanged(int)));

    startPointEdit = new QLineEdit(this);
    midPointEdit = new QLineEdit(this);
    endPointEdit = new QLineEdit(this);
    timeEdit = new QLineEdit(this);
    newAngleEdit = new QLineEdit(this);
    jointIndexEdit = new QLineEdit(this);
    allAnglesEdit = new QLineEdit(this);

    startPointEdit->setPlaceholderText("np.: P1, p2");
    midPointEdit->setPlaceholderText("np.: P1, p2");
    endPointEdit->setPlaceholderText("np.: P1, p2");
    timeEdit->setPlaceholderText("np.: 1000");
    newAngleEdit->setPlaceholderText("np.: 45");
    jointIndexEdit->setPlaceholderText("np.: 0, 1 itd.");
    allAnglesEdit->setPlaceholderText("Kąty oddzielone przecinkami");

    startPointEdit->installEventFilter(this);
    midPointEdit->installEventFilter(this);
    endPointEdit->installEventFilter(this);
    timeEdit->installEventFilter(this);
    newAngleEdit->installEventFilter(this);
    jointIndexEdit->installEventFilter(this);
    allAnglesEdit->installEventFilter(this);

    QRegExpValidator * v = new QRegExpValidator(QRegExp("[pP][0-9]{1,4}"), this);

    startPointEdit->setValidator(v);
    midPointEdit->setValidator(v);
    endPointEdit->setValidator(v);
    timeEdit->setValidator(new QIntValidator(1, 1000000, this));
    newAngleEdit->setValidator(new QIntValidator(-180, 600, this));
    jointIndexEdit->setValidator(new QIntValidator(0, 10, this));

    startPointLabel = new QLabel("Punkt startowy", this);
    midPointLabel = new QLabel("Punkt pośredni", this);
    endPointLabel = new QLabel("Punkt końcowy", this);
    timeLabel = new QLabel("Czas [ms]", this);
    newAngleLabel = new QLabel("Nowy kąt", this);
    jointIndexLabel = new QLabel("Przegub", this);
    allAnglesLabel = new QLabel("Kąty", this);

    constTCPlocationCheckBox = new QCheckBox("Stałe położenie TCP", this);

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
    grid->addWidget(jointIndexEdit, 6, 1, 1, 2);
    grid->addWidget(allAnglesEdit, 7, 1, 1, 2);
    grid->addWidget(startPointLabel, 1, 0, Qt::AlignRight);
    grid->addWidget(midPointLabel, 2, 0, Qt::AlignRight);
    grid->addWidget(endPointLabel, 3, 0, Qt::AlignRight);
    grid->addWidget(timeLabel, 4, 0, Qt::AlignRight);
    grid->addWidget(newAngleLabel, 5, 0, Qt::AlignRight);
    grid->addWidget(jointIndexLabel, 6, 0, Qt::AlignRight);
    grid->addWidget(allAnglesLabel, 7, 0, Qt::AlignRight);
    grid->addWidget(constTCPlocationCheckBox, 8, 0);
    grid->addWidget(addButton, 9, 1);
    grid->addWidget(cancelButton, 9, 2);

    setLayout(grid);

    typeChanged(0);
}

void AddActionDialog::typeChanged(int index)
{
    enabledEdits.clear();

    switch(index)
    {
    case STRAIGHT_LINE:
    case FREE:
    case CONST_STRAIGHT:
        enabledEdits.push_back(startPointEdit);
        enabledEdits.push_back(endPointEdit);
        break;
    case ARCH:
        enabledEdits.push_back(startPointEdit);
        enabledEdits.push_back(midPointEdit);
        enabledEdits.push_back(endPointEdit);
        break;
    case DELAY:
        enabledEdits.push_back(timeEdit);
        break;
    case SINGLE:
        enabledEdits.push_back(jointIndexEdit);
        break;
    case GRIPPER:
        enabledEdits.push_back(jointIndexEdit);
        enabledEdits.push_back(newAngleEdit);
        break;
    case ALL_ANGLES:
        enabledEdits.push_back(allAnglesEdit);
        break;
    }

    disableAll();
    for(QLineEdit * l : enabledEdits)
    {
        l->show();
        qDebug() << "show";
    }
}

void AddActionDialog::enableAll()
{
    startPointEdit->show();
    midPointEdit->show();
    endPointEdit->show();
    timeEdit->show();
    newAngleEdit->show();
    jointIndexEdit->show();
    allAnglesEdit->show();
    startPointLabel->show();
    midPointLabel->show();
    endPointLabel->show();
    timeLabel->show();
    newAngleLabel->show();
    jointIndexLabel->show();
    allAnglesLabel->show();
    constTCPlocationCheckBox->show();
}

void AddActionDialog::disableAll()
{
    startPointEdit->hide();
    midPointEdit->hide();
    endPointEdit->hide();
    timeEdit->hide();
    newAngleEdit->hide();
    jointIndexEdit->hide();
    allAnglesEdit->hide();
    startPointLabel->hide();
    midPointLabel->hide();
    endPointLabel->hide();
    timeLabel->hide();
    newAngleLabel->hide();
    jointIndexLabel->hide();
    allAnglesLabel->hide();
    constTCPlocationCheckBox->hide();
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

void AddActionDialog::setJoint(QString s)
{
    jointIndexEdit->setText(s);
}

void AddActionDialog::setInfo(QStringList s)
{
    if(s.size() != static_cast<int>(enabledEdits.size()))
        qDebug() << "error: AddActionDialog::setInfo(QStringList) : QStringList::size() != Lista::size()";

    switch(typeList->currentIndex())
    {
    case SINGLE:
        enabledEdits[0]->setText(s.at(0));
        break;
    case ALL_ANGLES:
    {
        QString s0;

        for(int i = 0; i < s.size(); i++)
        {
            s0 += s[i];
            if(i < s.size() - 1)
                s0 += ",";
        }

        enabledEdits[0]->setText(s0);
    }
        break;
    default:
        enabledEdits[0]->setText(s.at(s.size() - 1));
    }
}

void AddActionDialog::addClicked()
{
    QString info;

    for(int i = 0; i < static_cast<int>(enabledEdits.size()); i++)
    {
        QString txt = enabledEdits[i]->text();
        txt = txt.toUpper();
        info += enabledEdits[i]->text();

        if(i < static_cast<int>(enabledEdits.size()) - 1)
            info += ",";
    }

    info = info.toUpper();

    ActionType type = stringToActionType(typeList->currentText());
    if(type == SINGLE)
    {
        if(constTCPlocationCheckBox->isChecked())
            info += ",1";
        else
            info += ",0";
    }

    emit addValues(stringToActionType(typeList->currentText()), info);
    accept();
}

bool AddActionDialog::eventFilter(QObject *object, QEvent *event)
{
    if(event->type() == QEvent::Show)
    {
        if(object == startPointEdit)
        {
            startPointLabel->show();
        }
        else if(object == midPointEdit)
        {
            midPointLabel->show();
        }
        else if(object == endPointEdit)
        {
            endPointLabel->show();
        }
        else if(object == timeEdit)
        {
            timeLabel->show();
        }
        else if(object == newAngleEdit)
        {
            newAngleLabel->show();
            constTCPlocationCheckBox->show();
        }
        else if(object == jointIndexEdit)
        {
            jointIndexLabel->show();
        }
        else if(object == allAnglesEdit)
        {
            allAnglesLabel->show();
        }
    }
    else if(event->type() == QEvent::Hide)
    {
        if(object == startPointEdit)
        {
            startPointLabel->hide();
        }
        else if(object == midPointEdit)
        {
            midPointLabel->hide();
        }
        else if(object == endPointEdit)
        {
            endPointLabel->hide();
        }
        else if(object == timeEdit)
        {
            timeLabel->hide();
        }
        else if(object == newAngleEdit)
        {
            newAngleLabel->hide();
            constTCPlocationCheckBox->hide();
        }
        else if(object == jointIndexEdit)
        {
            jointIndexLabel->hide();
        }
        else if(object == allAnglesEdit)
        {
            allAnglesLabel->hide();
        }
    }

    return false;
}
