#ifndef TESTTAB_H
#define TESTTAB_H

#include <QDebug>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>

class TestTab : public QWidget
{
    Q_OBJECT

    QPushButton * robotInit,
                  * start,
                  * debugPauseButton;

public:
    explicit TestTab(QWidget *parent = nullptr);

signals:
    void initPressed();
    void startPressed();
public slots:
    void debugPause();
};

#endif // TESTTAB_H
