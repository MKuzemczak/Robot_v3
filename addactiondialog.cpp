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

    startPointEdit->setPlaceholderText("np.: P1, p2");
    midPointEdit->setPlaceholderText("np.: P1, p2");
    endPointEdit->setPlaceholderText("np.: P1, p2");
    timeEdit->setPlaceholderText("np.: 1000");
    newAngleEdit->setPlaceholderText("np.: 45");

    QRegExpValidator * v = new QRegExpValidator(QRegExp("[pP][0-9]{1,4}"), this);

    startPointEdit->setValidator(v);
    midPointEdit->setValidator(v);
    endPointEdit->setValidator(v);
    timeEdit->setValidator(new QIntValidator(1, 1000000, this));
    newAngleEdit->setValidator(new QIntValidator(-180, 600, this));

    startPointLabel = new QLabel("Punkt startowy", this);
    midPointLabel = new QLabel("Punkt pośredni", this);
    endPointLabel = new QLabel("Punkt końcowy", this);
    timeLabel = new QLabel("Czas [ms]", this);
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
    case GRIPPER:
    case SINGLE:
        enabledEdits.push_back(newAngleEdit);
        break;
    }

    disableAll();
    for(QLineEdit * l : enabledEdits)
        l->setEnabled(true);
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
    if(s.size() != static_cast<int>(enabledEdits.size()))
        qDebug() << "error: AddActionDialog::setInfo(QStringList) : QStringList::size() != Lista::size()";

    enabledEdits[0]->setText(s.at(s.size() - 1));
}

void AddActionDialog::addClicked()
{
    QString info;

    for(int i = 0; i < static_cast<int>(enabledEdits.size()); i++)
    {
        QString txt = enabledEdits[i]->text();
        txt = txt.toUpper();

        if(txt.at(0) == "P" && !pointList->containsName(txt))
        {
            qDebug() << "error: AddActionDialog::addClicked() : punkt " << txt << " nie istnieje";
            return;
        }

        info += enabledEdits[i]->text();

        if(i < static_cast<int>(enabledEdits.size()) - 1)
            info += ",";
    }

    info = info.toUpper();

    emit addValues(stringToActionType(typeList->currentText()), info);
    accept();
}
