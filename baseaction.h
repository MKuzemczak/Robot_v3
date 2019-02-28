#pragma once

#include <QObject>

#include "eigenaddons.h"
#include "lista.h"
#include "robot.h"
#include "comflags.h"
#include "actiontype.h"



/* Klasa Action, po której dziedziczą klasy pochodne,
 * pozwala na stworzenie jednolitej sekwencji, listy czynności, które robot ma wykonać.
 *
 * Każda klasa pochodna dla Action przechowuje dane potrzebne
 *  do wykonania danej czynności (np. punkty pośrednie lub czas postoju),
 *  oraz metodę, która wykorzystuje te dane (np. interpoluje tor lub obsługuje czasomierz).
 *
 **/
class BaseAction : public QObject
{
    Q_OBJECT

    // klasa bazowa, niewiele tu jest, wszystko w klasach pochodnych

    ActionType type;
    bool calculated,
        done;


    SerialPort * arduinoPortPtr;
    Flags * flagsPtr;

    QThread * parentThreadPtr;


public:


    BaseAction();
    BaseAction(QObject * parent);
    virtual ~BaseAction();

    void setType(ActionType t);
    void setCalculated();
    bool isCalculated();
    void resetCalculated();
    void setDone();
    void resetDone();
    void setArduinoPortPtr(SerialPort * ptr);
    SerialPort * arduinoPort();
    void setFlagsPtr(Flags * ptr);
    Flags * flags();
    bool isDone();
    void setParentThreadPtr(QThread * ptr);
    QThread * getParentThreadPtr();

    virtual bool calculate(Robot & robot);
    virtual void execute();
    virtual int size();
    virtual void clear();

public slots:
    void calcSlot(Robot * robot);
    void moveToParentThread();

signals:
    void calculationsFinished();
    void calculationsFailed();
};

class StraightLineMovAction : public BaseAction
{
    Eigen::Vector3d starting,
                    destination;
    int speed;
    bool speedChanged;

    Lista<Lista<int>> pathInServoDegs;

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    StraightLineMovAction(Eigen::Vector3d start,
                          Eigen::Vector3d dest,
                          int spd,
                          SerialPort * port,
                          Flags * flags);
    ~StraightLineMovAction()
    {
    }

    virtual bool calculate(Robot & robot);
    virtual void execute();
    virtual void clear();

    void lerp(Lista<Eigen::Vector3d, Eigen::aligned_allocator<Eigen::Vector3d>> & path);
    virtual int size();
};

class FreeMovAction : public BaseAction
{
    Eigen::Vector3d destination;
    int speed;
    bool speedChanged;
    Lista<int> destInServoDegs;

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    FreeMovAction(Eigen::Vector3d dest,
                  int spd,
                  SerialPort * port,
                  Flags * flags);
    ~FreeMovAction();

    virtual bool calculate(Robot & robot);
    virtual void execute();
    virtual void clear();
};

class ArchMovAction : public BaseAction
{
    Eigen::Vector3d starting,
                    intermediate,
                    destination;
    int speed;
    bool speedChanged;
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    ArchMovAction(Eigen::Vector3d start,
                  Eigen::Vector3d inter,
                  Eigen::Vector3d dest,
                  int spd,
                  SerialPort * port,
                  Flags * flags);

    ~ArchMovAction();

    virtual bool calculate(Robot & robot);
    virtual void execute();
    virtual void clear();

};

class DelayAction : public BaseAction
{
    double millis;

public:
    DelayAction(double mill);
    ~DelayAction();

    virtual bool calculate(Robot & robot)
    {
        robot.getDOF();

        return true;
    }
    virtual void execute()
    {
    }

    virtual void clear()
    {

    }
};

class SetSingleJointAction : public BaseAction
{
    int joint,
        angleDeg,
        speed;
    bool constTCPlocationFlag;
    bool speedChanged;
    Lista<Lista<int>> pathInServoDegs;

public:
    SetSingleJointAction(int j, int deg, int spd,
                         bool TCP,
                         SerialPort * port,
                         Flags * flags);
    ~SetSingleJointAction();

    virtual bool calculate(Robot & robot);
    virtual void execute();
    virtual int size();
    virtual void clear();
};

class ConstTCPOrientAction : public BaseAction
{
    Eigen::Vector3d starting,
                    destination;
    int speed;
    bool speedChanged;
    Lista<Lista<int>> pathInServoDegs;

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    ConstTCPOrientAction(Eigen::Vector3d start,
                         Eigen::Vector3d dest,
                         int spd,
                         SerialPort * port,
                         Flags * flags);
    ~ConstTCPOrientAction();

    virtual bool calculate(Robot & robot);
    virtual void execute();
    virtual int size();
    virtual void clear();
    void lerp(Lista<Eigen::Vector3d, Eigen::aligned_allocator<Eigen::Vector3d>> & path);

};

class GripperAction : public BaseAction
{
    int newSetting, servoIndex;

public:
    GripperAction(int set,
                  SerialPort * port,
                  Flags * flags);
    ~GripperAction();

    virtual bool calculate(Robot & robot);
    virtual void execute();
    virtual int size();
    virtual void clear();
};

class setAllAnglesAction : public BaseAction
{
    Lista<int> angles;
    Lista<int> servoDegs;
    int speed;
    bool speedChanged;
public:
    setAllAnglesAction(Lista<int>,
                       int,
                       SerialPort*,
                       Flags*);
    ~setAllAnglesAction();

    virtual bool calculate(Robot & robot);
    virtual void execute();
    virtual int size();
    virtual void clear();
};
