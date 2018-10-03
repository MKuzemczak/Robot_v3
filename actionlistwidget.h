#ifndef ACTIONLISTWIDGET_H
#define ACTIONLISTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>

#include "actiontype.h"
#include "addactiondialog.h"
#include "pointlistwidget.h"

class ActionListWidget : public QWidget
{
    Q_OBJECT

    QTableWidget * table;
    QLabel * label;
    QPushButton *addButton,
                *deleteButton;

    AddActionDialog * addDialog;

    PointListWidget * pointList;

public:
    explicit ActionListWidget(QWidget *parent = nullptr);

signals:
    void actionAdded(ActionType type, QString info);
public slots:
    void addAction(ActionType type, QString info);
    void openAddDialog();
    void deleteAction();
};

#endif // ACTIONLISTWIDGET_H
