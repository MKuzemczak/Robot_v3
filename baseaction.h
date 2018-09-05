#pragma once

#include "eigenaddons.h"
#include "lista.h"
#include "robot.h"
#include "comflags.h"

typedef enum
{
    STRAIGHT_LINE,
    FREE,
    ARCH,
    LOCAL_CHANGE,
    DELAY,
    SINGLE,
    CONST_STRAIGHT
} ActionType;


/* Klasa Action, po której dziedziczą klasy pochodne,
 * pozwala na stworzenie jednolitej sekwencji, listy czynności, które robot ma wykonać.
 *
 * Każda klasa pochodna dla Action przechowuje dane potrzebne
 *  do wykonania danej czynności (np. punkty pośrednie lub czas postoju),
 *  oraz metodę, która wykorzystuje te dane (np. interpoluje tor lub obsługuje czasomierz).
 *
 **/
class BaseAction
{
    // klasa bazowa, niewiele tu jest, wszystko w klasach pochodnych

    ActionType type;
    bool calculated,
        done;


    SerialPort * arduinoPortPtr;
    Flags * flagsPtr;


public:
    BaseAction();
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

    virtual void calculate(Robot & robot);
    virtual void execute();
    virtual int size();
};

class StraightLineMovAction : public BaseAction
{
    Eigen::Vector3d starting,
                    destination;

    Lista<Lista<int>> pathInServoDegs;

public:
    StraightLineMovAction(Eigen::Vector3d start,
                          Eigen::Vector3d dest,
                          SerialPort * port,
                          Flags * flags);
    ~StraightLineMovAction()
    {
    }

    virtual void calculate(Robot & robot);
    virtual void execute();

    void lerp(Lista<Eigen::Vector3d> & path);
    virtual int size();
};

class FreeMovAction : public BaseAction
{
    Eigen::Vector3d destination;

    Lista<int> destInServoDegs;

public:
    FreeMovAction(Eigen::Vector3d dest,
                  SerialPort * port,
                  Flags * flags);
    ~FreeMovAction();

    virtual void calculate(Robot & robot);
    virtual void execute();
};

class ArchMovAction : public BaseAction
{
    Eigen::Vector3d starting,
                    intermediate,
                    destination;


public:
    ArchMovAction(Eigen::Vector3d start,
                  Eigen::Vector3d inter,
                  Eigen::Vector3d dest,
                  SerialPort * port,
                  Flags * flags);

    ~ArchMovAction();

    virtual void calculate(Robot & robot);

    virtual void execute();

};

class DelayAction : public BaseAction
{
    double millis;

public:
    DelayAction(double mill);
    ~DelayAction();

    virtual void calculate(Robot & robot)
    {
    }
    virtual void execute()
    {
    }
};

class SetSingleJointAction : public BaseAction
{
    int joint,
        angleDeg;
    Lista<int> servoSignals;

public:
    SetSingleJointAction(int j, int deg,
                         SerialPort * port,
                         Flags * flags);
    ~SetSingleJointAction();

    virtual void calculate(Robot & robot);
    virtual void execute();
};

class ConstTCPOrientAction : public BaseAction
{
    Eigen::Vector3d starting,
                    destination;

    Lista<Lista<int>> pathInServoDegs;

public:
    ConstTCPOrientAction(Eigen::Vector3d start,
                         Eigen::Vector3d dest,
                         SerialPort * port,
                         Flags * flags);
    ~ConstTCPOrientAction();

    virtual void calculate(Robot & robot);
    virtual void execute();

    void lerp(Lista<Eigen::Vector3d> & path);
    virtual int size();
};
