#include "actionlistwidget.h"

ActionListWidget::ActionListWidget(QWidget *parent) : QWidget(parent)
{
    table = new QTableWidget(this);

    table->setColumnCount(2);

    QStringList horizontalHeaders = {"Akcja", "Info"};
    table->setHorizontalHeaderLabels(horizontalHeaders);

    table->setColumnWidth(0, 100);
    table->setColumnWidth(1, 100);

    /*for(int i = 0; i < 100; i++)
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
    }*/

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

    addDialog = nullptr;

    connect(addButton, SIGNAL(clicked()), this, SLOT(openAddDialog()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteAction()));
}

void ActionListWidget::addAction(ActionType type, QString info)
{
    emit actionAdded(type, info);

    int rowCount = table->rowCount();

    table->setRowCount(rowCount+1);

    qDebug() << "ActionListWidget::addAction(ActionType type, QString info):\n"
             << "table->rowCount() == " << table->rowCount();

    table->setItem(rowCount, 0, new QTableWidgetItem(actionTypeToString(type)));
    table->setItem(rowCount, 1, new QTableWidgetItem(info));

    qDebug() << "ActionListWidget::addAction(ActionType type, QString info):\n"
             << table->item(0,0) << "\n" << table->item(0,1);

    if(rowCount%2 == 0)
    {
        table->item(rowCount,0)->setBackgroundColor(QColor(0,0,0,10));
        table->item(rowCount,1)->setBackgroundColor(QColor(0,0,0,10));
    }

    table->setRowHeight(rowCount, 10);
    table->setVerticalHeaderItem(rowCount, new QTableWidgetItem(QString("%1").arg(rowCount)));

    if(rowCount%2 == 0)
        table->verticalHeaderItem(rowCount)->setBackgroundColor(QColor(0,0,0,10));
}

void ActionListWidget::deleteAction()
{
    if(table->currentRow() >= 0)
    {
        table->removeRow(table->currentRow());
    }
}

void ActionListWidget::openAddDialog()
{
    if(addDialog != nullptr)
        delete addDialog;

    addDialog = new AddActionDialog(this);

    connect(addDialog, SIGNAL(addValues(ActionType, QString)), this, SLOT(addAction(ActionType, QString)));

    if(table->rowCount() > 0)
    {
        addDialog->setType(stringToActionType(table->item(table->rowCount() - 1, 0)->text()));
        addDialog->setInfo(getInfo(table->item(table->rowCount() - 1, 1)->text()));
    }

    addDialog->show();
    addDialog->raise();
    addDialog->activateWindow();
}

QStringList ActionListWidget::getInfo(QString s)
{
    return s.split(",");
}
