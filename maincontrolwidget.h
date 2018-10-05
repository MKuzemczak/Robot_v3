#ifndef MAINCONTROLWIDGET_H
#define MAINCONTROLWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "diode.h"
#include "fixedgridlayout.h"

class MainControlWidget : public QWidget
{
    Q_OBJECT

    QPushButton * runButton,
                * stopButton,
                * upButton,
                * downButton,
                * leftButton,
                * rightButton,
                * frontButton,
                * rearButton,
                * aheadButton,
                * drawBackButton,
                * setButton,
                * toListButton;

    QLineEdit * xEdit,
                * yEdit,
                * zEdit;

    QLabel * xLabel,
            * yLabel,
            * zLabel;

    Diode * runningDiode,
            * serialStateDiode;

    QGroupBox * createPointMovBox();
    QGroupBox * createButtonMovBox();
    QGroupBox * createSequenceControlBox();

public:
    explicit MainControlWidget(QWidget *parent = nullptr);

signals:
    void runClicked();
    void stopClicked();
    void set(int x, int y, int z);
    void addPointToList(int x, int y, int z);
    void upPressed();
    void upReleased();
    void downPressed();
    void downReleased();
    void leftPressed();
    void leftReleased();
    void rightPressed();
    void rightReleased();
    void frontPressed();
    void frontReleased();
    void rearPressed();
    void rearReleased();
    void aheadPressed();
    void aheadReleased();
    void drawBackPressed();
    void drawBackReleased();

public slots:
    void emitSet();
    void emitPointToList();

};

#endif // MAINCONTROLWIDGET_H
