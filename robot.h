#pragma once

#include <vector>
#include <QDebug>

#include "lista.h"
#include "joint.h"

//#define DEBUG_ROBOT
//#define DEBUG_ROBOT1

#include <QThread>

#include "timer.h"

class Robot
{
    Joint TCP;// polozenie TCP
    int DOF;// liczba DOF

    int gripperSetting;

    Eigen::Vector3d TCPOrient;


    Lista<Joint, Eigen::aligned_allocator<Joint>> regJoints,
        locJoints;// lista przegubów

    Lista<Joint*, Eigen::aligned_allocator<Joint*>> joints;


    // aktualne rozwarcie chwytaka

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    Robot();
    ~Robot();

    void mapThetasToServos(Lista<int> & lista);

    void updateDHmatrices();
    void updateDHvectors();
    bool jacobian(Eigen::MatrixXd & jacobM, int indexOfSetJoint/*Lista<Joint> & joints, Eigen::Vector3d & setPoint*/);
    bool jacobAlgStep(double param, int indexOfSetJoint, Eigen::Vector3d & target);
    bool set(Eigen::Vector3d & point); // using jacobian algorithm
    bool set(int startJoint, int endJoint, int setJoint, Eigen::Vector3d & point);
    bool setRegional(Eigen::Vector3d & point);
    bool setExcluding(int excludedJoint, int setJoint, Eigen::Vector3d & point);

    bool jacobAlgStep(double param, int startJoint, int endJoint, int setJoint, Eigen::Vector3d & target);
    bool jacobian(Eigen::MatrixXd & jacobM, int startJoint, int endJoint, int setJoint);

    bool jacobAlgStep(double param, Lista<int> & indexes, int setJoint, Eigen::Vector3d & target);
    bool jacobian(Eigen::MatrixXd & jacobM, Lista<int> & indexes, int setJoint);

    //////////////////////////////////////////////////////// setters & getters & adders
    void addRegJoint(double a, double al, double dl);
    void addLocJoint(double a, double l, double dl);

    void setThetaDeg(int joint, double theta);
    void setThetaRad(int joint, double theta);

    void setTCPaLength(double l);

    Eigen::Vector3d & getJointLocation(int joint);
    Eigen::Vector3d & getTCPlocation();
    Eigen::Matrix4d & getJointDHmatrix(int joint);
    Eigen::Vector3d & getJointZinGlobal(int joint);

    int getDOF();

    double getJointThetaRad(int joint);

    void addJointServoMinMax(int joint, int min, int max);

    void setJointConversionMinMax(int joint, int min, int max);

    int getJointConversionMin(int joint);
    int getJointConversionMax(int joint);

    void setJointConstructionMinMax(int joint, int min, int max);

    int getJointConstructionMin(int joint);
    int getJointConstructionMax(int joint);

    int getJointServoAmount(int joint);

    int getRegJointsAmount();
    int getLocJointsAmount();

    void setTCPOrient(Eigen::Vector3d v);
    Eigen::Vector3d & getTCPOrient();

    void setGripper(int set);
    int getGripper();
    /////////////////////////////////////////////////////////////// !setter & getters & adders

};

void constrain(double & x, double min, double max);
