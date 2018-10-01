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

    for(int i = 0; i < 1; i++)
    {
        table->setRowCount(i+1);
        for(int j = 0; j < 3; j++)
        {
            table->setItem(i, j, new QTableWidgetItem(QString("%1").arg(i*10+j)));
            if(i%2 == 0)
                table->item(i,j)->setBackgroundColor(QColor(0,0,0,10));
        }
        table->setRowHeight(i, 10);
        table->setVerticalHeaderItem(i, new QTableWidgetItem(QString("P%1").arg(i)));
        if(i%2 == 0)
            table->verticalHeaderItem(i)->setBackgroundColor(QColor(0,0,0,10));
    }

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

    connect(addButton, SIGNAL(clicked()), addDialog, SLOT(exec()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deletePoint()));
    connect(addDialog, SIGNAL(addValues(int, int, int)), this, SLOT(addPoint(int, int, int)));

    setMaximumWidth(183);
}

void PointListWidget::selectionChangedSlot(const QItemSelection &, const QItemSelection &)
{
    //const QModelIndex index = listView->selectionModel()->currentIndex();

    //qDebug() << index;
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
        //table->setRowCount(table->rowCount() - 1);
    }
}
