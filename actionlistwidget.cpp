#include "actionlistwidget.h"

ActionListWidget::ActionListWidget(QWidget *parent) : QWidget(parent)
{
    listView = new QListView(this);
    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(listView);
    setLayout(layout);

    standardModel = new QStandardItemModel;
    rootNode = standardModel->invisibleRootItem();

    for(int i = 0; i < 100; i++)
    {
        rootNode->appendRow(new QStandardItem(QString("item%1").arg(i)));
    }

    listView->setModel(standardModel);

    QItemSelectionModel *selectionModel = listView->selectionModel();
    connect(selectionModel, SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
            this, SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));

    setMaximumWidth(300);
}

void ActionListWidget::selectionChangedSlot(const QItemSelection &, const QItemSelection &)
{
    const QModelIndex index = listView->selectionModel()->currentIndex();

    qDebug() << index;
}
