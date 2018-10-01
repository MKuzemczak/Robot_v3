#include "actionlistwidget.h"

ActionListWidget::ActionListWidget(QWidget *parent) : QWidget(parent)
{
    table = new QTableWidget(this);

    table->setColumnCount(2);

    QStringList horizontalHeaders = {"Akcja", "Info"};
    table->setHorizontalHeaderLabels(horizontalHeaders);

    table->setColumnWidth(0, 50);
    table->setColumnWidth(1, 50);

    for(int i = 0; i < 100; i++)
    {
        table->setRowCount(i+1);
        for(int j = 0; j < 2; j++)
        {
            table->setItem(i, j, new QTableWidgetItem(QString("%1").arg(i*10+j)));
            if(i%2 == 0)
                table->item(i,j)->setBackgroundColor(QColor(0,0,0,10));
        }
        table->setRowHeight(i, 10);
        table->setVerticalHeaderItem(i, new QTableWidgetItem(QString("%1").arg(i)));
        if(i%2 == 0)
            table->verticalHeaderItem(i)->setBackgroundColor(QColor(0,0,0,10));
    }

    label = new QLabel("Lista akcji", this);
    label->setMaximumHeight(13);
    addButton = new QPushButton("Dodaj", this);
    deleteButton = new QPushButton("Usuń", this);

    QGridLayout * layout = new QGridLayout;

    layout->addWidget(label, 0, 0);
    layout->addWidget(table, 1, 0, 2, 4);
    layout->addWidget(addButton, 5, 0);
    layout->addWidget(deleteButton, 5, 1);

    setLayout(layout);
}
