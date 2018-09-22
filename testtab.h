#ifndef TESTTAB_H
#define TESTTAB_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>

class TestTab : public QWidget
{
    Q_OBJECT

    QPushButton * robotInit,
                  * start;

public:
    explicit TestTab(QWidget *parent = nullptr);

signals:
    void initPressed();
    void startPressed();
public slots:
};

#endif // TESTTAB_H
