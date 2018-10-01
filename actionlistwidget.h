#ifndef ACTIONLISTWIDGET_H
#define ACTIONLISTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>


class ActionListWidget : public QWidget
{
    Q_OBJECT

    QTableWidget * table;
    QLabel * label;
    QPushButton *addButton,
                *deleteButton;


public:
    explicit ActionListWidget(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // ACTIONLISTWIDGET_H
