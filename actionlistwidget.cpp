#include "actionlistwidget.h"

ActionListWidget::ActionListWidget(QWidget *parent) : QWidget(parent)
{
    table = new QTableWidget(this);

    table->setColumnCount(2);

    QStringList horizontalHeaders = {"Akcja", "Info"};
    table->setHorizontalHeaderLabels(horizontalHeaders);

    table->setColumnWidth(0, 100);
    table->setColumnWidth(1, 100);

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
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteCurrentAction()));

    setMaximumWidth(300);
}

void ActionListWidget::addAction(ActionType type, QString info)
{
    QStringList slist = info.split(",");

    for(int i = 0; i < slist.size(); i++)
    {
        if(slist[i][0] == "P" && !pointList->containsName(slist[i]))
        {
            qDebug() << "error: ActionListWidget::addAction() : punkt " << slist[i] << " nie istnieje";
            return;
        }
    }

    emit actionAdded(type, info);

    int rowCount = table->rowCount();

    table->setRowCount(rowCount+1);

    table->setItem(rowCount, 0, new QTableWidgetItem(actionTypeToString(type)));
    table->setItem(rowCount, 1, new QTableWidgetItem(info));

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
    emit actionDeleted(table->currentRow());

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
    addDialog->setPointListPtr(pointList);
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

void ActionListWidget::deleteAction(int i)
{
    emit actionDeleted(i);

    if(table->currentRow() >= 0)
    {
        table->removeRow(i);
    }
}

void ActionListWidget::deleteCurrentAction()
{
    int i = table->currentRow();

    emit actionDeleted(i);

    if(table->currentRow() >= 0)
    {
        table->removeRow(i);
    }
}
