#ifndef MAINCONTROLWIDGET_H
#define MAINCONTROLWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QSlider>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "diode.h"
#include "fixedgridlayout.h"
#include "lista.h"
#include "program.h"

class MainControlWidget : public QWidget
{
    Q_OBJECT

    Program * program;

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

    Lista<QSlider*> sliders;
    Lista<QLabel*> sliderLabels;
    Lista<QLineEdit*> sliderLineEdits;

    QSlider * gripperSlider;

    QGroupBox * createPointMovBox();
    QGroupBox * createButtonMovBox();
    QGroupBox * createSequenceControlBox();
    QGroupBox * createSliderBox();
    QGroupBox * createGripperBox();

public:
    explicit MainControlWidget(Program * pr, QWidget *parent = nullptr);

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
    void sliderChanged(int, int);

public slots:
    void emitSet();
    void emitPointToList();
    void displayPoint(int x, int y, int z);
    void setPortDiodeOn();
    void setPortDiodeOff();
    void setRunningDiodeOn();
    void setRunningDiodeOff();
    void handleSliderAction();
    void updateSliders();

};

#endif // MAINCONTROLWIDGET_H
