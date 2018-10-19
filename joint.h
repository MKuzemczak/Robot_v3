#pragma once

#include <QDebug>

#include "eigenaddons.h"
#include "lista.h"

//#define DEBUG_JOINT

#define ERR_NO_LENGTH 1

class Joint
{
    double alpha; // kat alfa - wokol tego X, miedzy poprzednim Z a tym tutaj.
    double Theta; // kat Theta - wokol tego Z
    double aLength; // dlugosc a - wzdluz tego x
    double dLength; // długość d - wzdluz poprzedniego z
    double baseTheta;
    Eigen::Matrix4d DHmatrix; // macierz DH transformacji punktu w tym ukladzie do poprzedniego ukladu
    Eigen::Vector3d locationInGlobal,  // wektor polozenia przegubu w globalnym ukladzie
        ZaxisInGlobal;

    Lista<Eigen::Vector2i, Eigen::aligned_allocator<Eigen::Vector2i>> servosMinMax;					// list of minimum & maximum signals of servos that this joint represents
    Eigen::Vector2i angleConversionMinMaxDeg,		// list if min&max angles needed for conversion to servo. In degrees (because Integer)
                            angleConstructionMinMaxDeg;		// list of min&max angles imposed by construction of the robot. In degrees

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    Joint();
    Joint(double a, double al, double dl);

    ~Joint();


    void mapThetaToServo(Lista<int> & lista);


    /////////////////////////////////////// setters & getters & adders
    void setAlpha(double a);
    double getAlpha();

    // aktualizuj kat Theta
    void setTheta(double T);
    double getTheta();

    void setBaseTheta(double t);
    double getBaseTheta();

    void setaLength(double l);
    double getaLength();

    void setdLength(double l);
    double getdLength();

    void setLocation(Eigen::Vector3d & v);
    Eigen::Vector3d & getLocation();

    void setZinGlobal(Eigen::Vector3d & v);
    Eigen::Vector3d & getZinGlobal();

    Eigen::Matrix4d & getDHmatrix();

    void addServoMinMax(int min, int max);
    void setServoMinMax(int servo, int min, int max);
    int getServoAmount();

    void setConversionMinMaxDeg(int min, int max);
    int getConversionMinDeg();
    int getConversionMaxDeg();

    void setConstructionMinMaxDeg(int min, int max);
    Eigen::Vector2i & getConstructionMinMaxDeg();
    int getConstructionMinDeg();
    int getConstructionMaxDeg();
    ///////////////////////////////////////////////////////// !setters & getters & adders

};

double map(double x, double in_min, double in_max, double out_min, double out_max);
