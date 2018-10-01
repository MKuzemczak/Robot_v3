#ifndef POINTLISTWIDGET_H
#define POINTLISTWIDGET_H

#include <QWidget>
#include <QListView>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QHeaderView>

#include "addpointdialog.h"

class PointListWidget : public QWidget
{
    Q_OBJECT

    //QListView * listView;
    //QStandardItemModel * standardModel;
    //QStandardItem *rootNode;
    QTableWidget * table;
    QLabel * label;
    QPushButton *addButton,
                *deleteButton;

    AddPointDialog * addDialog;

public:
    explicit PointListWidget(QWidget *parent = nullptr);

signals:

public slots:
    void selectionChangedSlot(const QItemSelection &, const QItemSelection &);
    void addPoint(int x, int y, int z);
    void deletePoint();
};

#endif // POINTLISTWIDGET_H
