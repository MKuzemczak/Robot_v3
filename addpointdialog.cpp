#include "addpointdialog.h"

AddPointDialog::AddPointDialog(QWidget * parent) :
    QDialog(parent)
{
    setFixedSize(300, 150);

    addButton = new QPushButton("Dodaj", this);
    cancelButton = new QPushButton("Anuluj", this);

    xLabel = new QLabel("X", this);
    yLabel = new QLabel("Y", this);
    zLabel = new QLabel("Z", this);

    xLabel->setMaximumWidth(10);
    yLabel->setMaximumWidth(10);
    zLabel->setMaximumWidth(10);

    xEdit = new QLineEdit("0", this);
    yEdit = new QLineEdit("0", this);
    zEdit = new QLineEdit("0", this);

    xLabel->setBuddy(xEdit);
    yLabel->setBuddy(yEdit);
    zLabel->setBuddy(zEdit);

    QGridLayout * layout = new QGridLayout;

    layout->addWidget(xLabel, 0, 0, Qt::AlignRight);
    layout->addWidget(xEdit, 0, 1);
    layout->addWidget(yLabel, 0, 2, Qt::AlignRight);
    layout->addWidget(yEdit, 0, 3);
    layout->addWidget(zLabel, 0, 4, Qt::AlignRight);
    layout->addWidget(zEdit, 0, 5);
    layout->addWidget(addButton, 1, 2);
    layout->addWidget(cancelButton, 1, 4);

    setLayout(layout);

    connect(addButton, SIGNAL(clicked()), this, SLOT(addPressed()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    setModal(false);
}

AddPointDialog::~AddPointDialog()
{

}

void AddPointDialog::addPressed()
{
    emit addValues(xEdit->text().toInt(), yEdit->text().toInt(), zEdit->text().toInt());

    reject();
}

void AddPointDialog::setDefault(int x, int y, int z)
{
    xEdit->setText(QString("%1").arg(x));
    yEdit->setText(QString("%1").arg(y));
    zEdit->setText(QString("%1").arg(z));
}
