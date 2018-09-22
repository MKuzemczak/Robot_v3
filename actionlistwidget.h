#ifndef ACTIONLISTWIDGET_H
#define ACTIONLISTWIDGET_H

#include <QWidget>
#include <QListView>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QDebug>
#include <QPushButton>
#include <QLabel>

class ActionListWidget : public QWidget
{
    Q_OBJECT

    QListView * listView;
    QStandardItemModel * standardModel;
    QStandardItem *rootNode;
    QLabel * label;
    QPushButton *addButton,
                *deleteButton;

public:
    explicit ActionListWidget(QWidget *parent = nullptr);

signals:

public slots:
    void selectionChangedSlot(const QItemSelection &, const QItemSelection &);

};

#endif // ACTIONLISTWIDGET_H
