#include "pointlistwidget.h"

PointListWidget::PointListWidget(QWidget *parent) : QWidget(parent)
{
    table = new QTableWidget(this);

    table->setColumnCount(3);

    QStringList horizontalHeaders = {"X", "Y", "Z"};
    table->setHorizontalHeaderLabels(horizontalHeaders);

    table->setColumnWidth(0, 20);
    table->setColumnWidth(1, 20);
    table->setColumnWidth(2, 20);

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    label = new QLabel("Lista punktów", this);
    label->setMaximumHeight(13);
    addButton = new QPushButton("Dodaj", this);
    deleteButton = new QPushButton("Usuń", this);

    QGridLayout * layout = new QGridLayout;

    layout->addWidget(label, 0, 0);
    layout->addWidget(table, 1, 0, 2, 4);
    layout->addWidget(addButton, 5, 0);
    layout->addWidget(deleteButton, 5, 1);

    setLayout(layout);

    addDialog = new AddPointDialog(this);

    connect(addButton, SIGNAL(clicked()), this, SLOT(openAddDialog()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deletePoint()));
    connect(addDialog, SIGNAL(addValues(int, int, int)), this, SLOT(addPoint(int, int, int)));

}

void PointListWidget::selectionChangedSlot(const QItemSelection &, const QItemSelection &)
{
}


void PointListWidget::addPoint(int x, int y, int z)
{

    table->setRowCount(table->rowCount() + 1);

    int count = table->rowCount();

    table->setItem(count - 1, 0, new QTableWidgetItem(QString("%1").arg(x)));
    table->setItem(count - 1, 1, new QTableWidgetItem(QString("%1").arg(y)));
    table->setItem(count - 1, 2, new QTableWidgetItem(QString("%1").arg(z)));

    if((count - 1)%2 == 0)
        for(int i = 0; i < 3; i++)
            table->item(count - 1, i)->setBackgroundColor(QColor(0,0,0,10));

    table->setRowHeight(count - 1, 10);

    if(count > 1)
    {
        int number = table->verticalHeaderItem(count - 2)->text().mid(1).toInt() + 1;

        table->setVerticalHeaderItem(count - 1, new QTableWidgetItem(QString("P%1")
                                                                     .arg(number)));
    }
    else
        table->setVerticalHeaderItem(count - 1, new QTableWidgetItem(QString("P0")));

    if((count - 1)%2 == 0)
        table->verticalHeaderItem(count - 1)->setBackgroundColor(QColor(0,0,0,10));

}

void PointListWidget::deletePoint()
{
    if(table->currentRow() >= 0)
    {
        table->removeRow(table->currentRow());
    }
}

void PointListWidget::openAddDialog(int x, int y, int z)
{
    if(table->rowCount() > 0)
    {
        x = table->item(table->rowCount() - 1, 0)->text().toInt();
        y = table->item(table->rowCount() - 1, 1)->text().toInt();
        z = table->item(table->rowCount() - 1, 2)->text().toInt();
    }
    addDialog->setDefault(x, y, z);
    addDialog->show();
    addDialog->raise();
    addDialog->activateWindow();
}

int PointListWidget::operator () (int point, int index)
{
    return table->item(point, index)->text().toInt();
}

bool PointListWidget::containsName(QString n)
{
    for(int i = 0; i < table->rowCount(); i++)
    {
        if(n == table->verticalHeaderItem(i)->text())
            return true;
    }

    return false;
}
