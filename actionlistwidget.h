#ifndef ACTIONLISTWIDGET_H
#define ACTIONLISTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QEvent>

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

    QStringList getInfo(QString s);
    void setPointListPtr(PointListWidget * ptr)
    {
        pointList = ptr;
    }

    void deleteAction();
    int size();
    QString getActionInfo(int);
    ActionType getActionType(int);

signals:
    void actionAdded(ActionType type, QString info);
    void actionDeleted(int);
    void actionMoved(int, int, int);
public slots:
    void addAction(ActionType type, QString info);
    void openAddDialog();
    void deleteAction(int i);
    void deleteCurrentAction();
};

#endif // ACTIONLISTWIDGET_H
