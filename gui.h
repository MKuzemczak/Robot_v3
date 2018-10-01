#ifndef GUI_H
#define GUI_H

#include <QWidget>
#include <QThread>
#include <QKeyEvent>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QDialog>

#include <iostream>

#include "program.h"
#include "maintab.h"
#include "testtab.h"

class GUI : public QWidget
{
    Q_OBJECT

    QThread programThread;

    Program * program;

/////////////////////////////////////// GUI
    QDialog * dialog;

    QTabWidget * tabWidget;
/////////////////////////////////////// !GUI

public:
    explicit GUI(QWidget *parent = nullptr);
    ~GUI();


protected:
    void keyPressEvent(QKeyEvent *e);


signals:

    void keyPressed(int key);

public slots:
};

#endif // GUI_H
