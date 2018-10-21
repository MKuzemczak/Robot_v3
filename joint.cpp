#include "Joint.h"



Joint::Joint()
{
    alpha = 0;
    Theta = 0;
    aLength = 0;
    dLength = 0;
    baseTheta = 0;

    DHmatrix = Eigen::Matrix4d::Identity();
}

Joint::Joint(double a, double al, double dl)
{
    alpha = a;
    Theta = 0;
    aLength = al;
    dLength = dl;
    baseTheta = 0;

    DHmatrix = Eigen::Matrix4d::Identity();
}


Joint::~Joint()
{
}


/*
 * This method maps joint's current Theta angle to all its servos' signals
 **/
void Joint::mapThetaToServo(Lista<int> & lista)
{
    for (int i = 0; i < static_cast<int>(servosMinMax.size()); i++)
    {
        int a = static_cast<int>(map(Theta,
            static_cast<double>(angleConversionMinMaxDeg[0]*DEG_TO_RAD),
            static_cast<double>(angleConversionMinMaxDeg[1]*DEG_TO_RAD),
            static_cast<double>(servosMinMax[i][0]),
            static_cast<double>(servosMinMax[i][1])));

#ifdef DEBUG_JOINT
        qDebug("Joint::Theta == %lf\n", Theta);
        qDebug("Joint::mapThetaToServo() : a == %d\n\n", a);
#endif // DEBUG_JOINT



        lista.push_back(a);
    }
}

///////////////////////////////////////////////////////////////////////////////////// setters & getters

int Joint::getConstructionMinDeg()
{
    return angleConstructionMinMaxDeg[0];
}

int Joint::getConstructionMaxDeg()
{
    return angleConstructionMinMaxDeg[1];
}

int Joint::getConversionMinDeg()
{
    return angleConversionMinMaxDeg[0];
}

int Joint::getConversionMaxDeg()
{
    return angleConversionMinMaxDeg[1];
}

int Joint::getServoAmount()
{
    return static_cast<int>(servosMinMax.size());
}

void Joint::setAlpha(double a)
{
    alpha = a;

}

double Joint::getAlpha()
{
    return alpha;
}

// aktualizuj kat Theta
void Joint::setTheta(double T)
{
    Theta = T;
}

double Joint::getTheta()
{
    return Theta;
}

void Joint::setBaseTheta(double t)
{
    baseTheta = t;
}

double Joint::getBaseTheta()
{
    return baseTheta;
}

void Joint::setaLength(double l)
{
    aLength = l;
}

double Joint::getaLength()
{
    return aLength;
}

void Joint::setdLength(double l)
{
    dLength = l;
}

double Joint::getdLength()
{
    return dLength;
}

void Joint::setLocation(Eigen::Vector3d & v)
{
    locationInGlobal = v;
}

Eigen::Vector3d & Joint::getLocation()
{
    return locationInGlobal;
}

void Joint::setZinGlobal(Eigen::Vector3d & v)
{
    ZaxisInGlobal = v;
}

Eigen::Vector3d & Joint::getZinGlobal()
{
    return ZaxisInGlobal;
}

Eigen::Matrix4d & Joint::getDHmatrix()
{
    return DHmatrix;
}

void Joint::addServoMinMax(int min, int max)
{
    Eigen::Vector2i v;

    v << min, max;


#ifdef DEBUG_JOINT
    qDebug("Joint::addServoMinMax(%d, %d);\n\n", min, max);
#endif // DEBUG_JOINT

    servosMinMax.push_back(v);
}

void Joint::setServoMinMax(int servo, int min, int max)
{
    if (servo < 0 || servo >= static_cast<int>(servosMinMax.size()))
        qDebug("Joint::setServoMinMax(...): Error: Servo index out of bounds\n");
    else
    {
        servosMinMax[servo][0] = min;
        servosMinMax[servo][1] = max;
    }


}

void Joint::setConversionMinMaxDeg(int min, int max)
{
    angleConversionMinMaxDeg << min, max;

#ifdef DEBUG_JOINT
    qDebug("Joint::setConversionMinMaxDeg(%d, %d);\n\n", min, max);
#endif // DEBUG_JOINT
}

void Joint::setConstructionMinMaxDeg(int min, int max)
{
    angleConstructionMinMaxDeg << min, max;

#ifdef DEBUG_JOINT
    qDebug("Joint::setConstructionMinMaxDeg(%d, %d);\n\n", min, max);
#endif // DEBUG_JOINT
}

Eigen::Vector2i & Joint::getConstructionMinMaxDeg()
{
    return angleConstructionMinMaxDeg;
}

//////////////////////////////////////////////////////////////////////////////// !setters & getters & adders


/////////////////////////////////////////////// others
double map(double x, double in_min, double in_max, double out_min, double out_max) {

#ifdef DEBUG_JOINT
    qDebug("map() : x == %lf, in_min == %lf, in_max == %lf, out_min == %lf, out_max == %lf\n\n",
           x, in_min, in_max, out_min, out_max);
#endif // DEBUG


    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
