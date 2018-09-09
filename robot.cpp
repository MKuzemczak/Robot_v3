#include "robot.h"

using namespace Eigen;

Robot::Robot()
{
    DOF = 0;

    joints.push_back(&TCP);
}


Robot::~Robot()
{
}


void Robot::updateDHmatrices()
{
    double sT, cT, sa, ca, aLength, dLength;

    for (int i = 0; i < static_cast<int>(joints.size()); i++)
    {
        Matrix4d & DHmatrix = joints[i]->getDHmatrix();

        if (i == 0)
        {
            cT = 1;
            sT = 0;
        }
        else
        {
            cT = cos(joints[i - 1]->getTheta());
            sT = sin(joints[i - 1]->getTheta());
        }

        ca = cos(joints[i]->getAlpha());
        sa = sin(joints[i]->getAlpha());

        aLength = joints[i]->getaLength();
        dLength = joints[i]->getdLength();


        DHmatrix(0, 0) = cT;
        DHmatrix(0, 1) = -sT * ca;
        DHmatrix(0, 2) = sT * sa;
        DHmatrix(0, 3) = aLength * cT;
        DHmatrix(1, 0) = sT;
        DHmatrix(1, 1) = cT * ca;
        DHmatrix(1, 2) = -cT * sa;
        DHmatrix(1, 3) = aLength * sT;
        DHmatrix(2, 1) = sa;
        DHmatrix(2, 2) = ca;
        DHmatrix(2, 3) = dLength;

    }


}

void Robot::updateDHvectors()
{
    Vector3d v;
    Matrix4d m;

    m = Matrix4d::Identity();

    for (int i = 0; i < static_cast<int>(joints.size()); i++)
    {
        m *= joints[i]->getDHmatrix();

        //qDebug("Przemnazana macierz nr " << i << ":\n" << m << '\n'; // debug

        v << m(0, 3), m(1, 3), m(2, 3);

        joints[i]->setLocation(v);

        Vector4d Z;
        Z << 0, 0, 1, 1;

        Z = (m * Z).eval();

        v << Z(0), Z(1), Z(2);
        v -= joints[i]->getLocation();

        joints[i]->setZinGlobal(v);

        //qDebug("Joint " << i << " Z:\n" << joints[i]->getZinGlobal() << '\n'; // debug
    }
}


bool Robot::jacobian(Eigen::MatrixXd & jacobM, int indexOfSetJoint/*Lista<Joint> & joints, Eigen::Vector3d & setPoint*/)
{
    if (indexOfSetJoint != jacobM.cols())
    {
        qDebug("jacobian: ilosc przegubow i ilosc kolumn w macierzy jakobianowej nie sa sobie rowne!\n");
        qDebug("Przeguby: %d, kolumny: %d\n", static_cast<int>(joints.size()), static_cast<int>(jacobM.cols()));
        return false;
    }

    if (jacobM.rows() != 3)
    {
        qDebug("Macierz jakobiego nie sklada sie z trzech wierszy (z wektorow 3D)!\n");
        qDebug("jacobM.cols(): %d\n", static_cast<int>(jacobM.cols()));
        return false;
    }

    Eigen::Vector3d jToP,   // vector pointing from currently processed joint to setPoint
                    dJToP,  // rotation around Z derivative - change of jToP vector
                    setPoint = TCP.getLocation();

    for (int i = 0; i < indexOfSetJoint; i++)
    {
        jToP = setPoint - joints[i]->getLocation();
        dJToP = joints[i]->getZinGlobal().cross(jToP);
        jacobM.col(i)	 = dJToP;
    }

    return true;
}

bool Robot::jacobAlgStep(double param, int indexOfSetJoint, Eigen::Vector3d & target)
{
    if (param < SMALLEST)
    {
        qDebug("Error jacobAlgStep: parametr algorytmu zerowy!\n");
        return false;
    }

    if (indexOfSetJoint < 2)
    {
        qDebug("Error jacobAlgStep:"
               " nie mozna zastosowac algorytmu do przegubow nizszysch niz nr %d\n", indexOfSetJoint);
        return false;
    }

    Eigen::MatrixXd jacobM(3, indexOfSetJoint);
    Eigen::VectorXd thetas(indexOfSetJoint);
    Eigen::MatrixXd invJ(indexOfSetJoint, 3);
    Eigen::MatrixXd mult;
    Eigen::Vector3d subt;

    for (int i = 0; i < indexOfSetJoint; i++)
        thetas(i) = joints[i]->getTheta();

    subt = TCP.getLocation() - target;

    jacobian(jacobM, indexOfSetJoint);

    invJ = psInv(jacobM);
    mult = param * (invJ * subt);

    thetas = (thetas - mult).eval();

    for (int i = 0; i < indexOfSetJoint; i++)
    {
        constrain(thetas(i), joints[i]->getConstructionMinMaxDeg()[0] * DEG_TO_RAD, joints[i]->getConstructionMinMaxDeg()[1] * DEG_TO_RAD);
        joints[i]->setTheta(thetas(i));
    }


    updateDHmatrices();
    updateDHvectors();

    return true;
}

bool Robot::jacobAlgStep(double param, int startJoint, int endJoint, int setJoint, Eigen::Vector3d & target)
{
    if (param < SMALLEST)
    {
        qDebug("Error jacobAlgStep: parametr algorytmu zerowy!\n");
        return false;
    }

    if (setJoint < 2)
    {
        qDebug("Error jacobAlgStep: nie mozna zastosowac algorytmu"
               " do przegubow nizszysch niz nr %d\n", setJoint);
        return false;
    }

    if (setJoint < endJoint)
    {
        qDebug("Error Robot::jacobAlgStep(...):"
               " przegub, ktorego lokalizacja jest ustawiana (setJoint) ma indeks"
               " nizszy od ostatniego ustawianego przugubu (endJoint).\n");
        qDebug("setJoint == %d, endJoint == %d\n", setJoint, endJoint);
        return true;
    }

    if (startJoint >= endJoint)
    {
        qDebug("Error Robot::jacobAlgStep(...):"
               " poczatkowy indeks zakresu zmienianych przegubow"
               " jest wiekszy od koncowego indeksu.\n");
        return false;
    }

    int amount = endJoint - startJoint + 1;

    Eigen::MatrixXd jacobM(3, amount);
    Eigen::VectorXd thetas(amount);
    Eigen::MatrixXd invJ(amount, 3);
    Eigen::VectorXd mult;
    Eigen::Vector3d subt;

    for (int i = 0; i < amount; i++)
        thetas(i) = joints[startJoint + i]->getTheta();

    subt = joints[setJoint]->getLocation() - target;

#ifdef DEBUG_ROBOT
    //qDebug << "subt:\n" << subt << '\n';

#endif // DEBUG_ROBOT


    jacobian(jacobM, startJoint, endJoint, setJoint);

    invJ = psInv(jacobM);


#ifdef DEBUG_ROBOT
    //qDebug("invJ:\n" << invJ << '\n';
#endif // DEBUG_ROBOT

    mult = param * (invJ * subt);

#ifdef DEBUG_ROBOT
    //qDebug("mult:\n" << mult << '\n';
#endif // DEBUG_ROBOT

    thetas = (thetas - mult).eval();

#ifdef DEBUG_ROBOT
    //qDebug("thetas:\n" << thetas << '\n';
#endif // DEBUG_ROBOT




    for (int i = 0; i < amount; i++)
    {
        constrain(thetas(i), joints[startJoint + i]->getConstructionMinMaxDeg()[0] * DEG_TO_RAD, joints[startJoint + i]->getConstructionMinMaxDeg()[1] * DEG_TO_RAD);
        joints[startJoint + i]->setTheta(thetas(i));
    }


    updateDHmatrices();
    updateDHvectors();

    return true;
}

bool Robot::jacobian(Eigen::MatrixXd & jacobM, int startJoint, int endJoint, int setJoint)
{
    int amount = endJoint - startJoint + 1;

    if (amount != jacobM.cols())
    {
        qDebug("jacobian: ilosc przegubow i ilosc kolumn w macierzy jakobianowej nie sa sobie rowne!\n");
        qDebug("Przeguby: %d, kolumny: %d\n", amount, static_cast<int>(jacobM.cols()));
        return false;
    }

    if (jacobM.rows() != 3)
    {
        qDebug("Macierz jakobiego nie sklada sie z trzech wierszy (z wektorow 3D)!\n");
        qDebug("jacobM.cols(): %d\n", static_cast<int>(jacobM.cols()));
        return false;
    }

    Eigen::Vector3d jToP,    // vector pointing from currently processed joint to setPoint
                    dJToP,   // rotation around Z derivative - change of jToP vector
                    setPoint = joints[setJoint]->getLocation();

    for(int i = 0 ; i < amount; i++)
    {
        jToP = setPoint - joints[startJoint + i]->getLocation();
        dJToP = joints[startJoint + i]->getZinGlobal().cross(jToP);
        jacobM.col(i) = dJToP;

#ifdef DEBUG_ROBOT
        qDebug() << "Robot::jacobian(Eigen::MatrixXd &, int, int, int) : jToP:\n" << jToP << "\n";
#endif // DEBUG_ROBOT
    }

#ifdef DEBUG_ROBOT
    qDebug() << "Robot::jacobian(Eigen::MatrixXd &, int, int, int) : jacobM:\n" << jacobM << "\n";
#endif // DEBUG_ROBOT


    return true;
}

bool Robot::set(Eigen::Vector3d & point)
{
    updateDHmatrices();
    updateDHvectors();

    while (static_cast<double>((point - TCP.getLocation()).norm()) > 1)
    {
        if(!jacobAlgStep(1, static_cast<int>(joints.size()) - 1, point))
            return false;
    }

    return true;
}

bool Robot::set(int startJoint, int endJoint, int setJoint, Eigen::Vector3d & point)
{
#ifdef DEBUG_ROBOT
    //qDebug("Robot::set(int, int, int, Eigen::Vector3d &)\n");
#endif

    updateDHmatrices();
    updateDHvectors();

    while (static_cast<double>((point - joints[setJoint]->getLocation()).norm()) > 1)
    {
        if(!jacobAlgStep(1, startJoint, endJoint, setJoint, point))
            return false;


#ifdef DEBUG_ROBOT
        qDebug("Robot::set(int, int, int, Eigen::Vector3d &), koniec petli\n");
        qDebug("startJoint == %d, endJoint == %d, setJoint == %d\n",
               startJoint, endJoint, setJoint);
        qDebug() << "point:\n" << point;
        qDebug() << "Joints:";
        for (int i = 0; i < static_cast<int>(joints.size()); i++)
        {
            qDebug() << "Joint " << i << ":\n"
                     << joints[i]->getLocation()
                     << "Theta == "
                     << joints[i]->getTheta() / DEG_TO_RAD
                     << "\n\n";
        }

#endif // DEBUG_ROBOT
    }

    return true;
}

bool Robot::setRegional(Eigen::Vector3d & point)
{
    updateDHmatrices();
    updateDHvectors();

#ifdef DEBUG_ROBOT
    qDebug("Robot::setRegional(), zaktualizowane DH\n");
#endif // DEBUG_ROBOT


    while (static_cast<double>((point - TCP.getLocation()).norm()) > 1)
    {
        if(!jacobAlgStep(1, 0, static_cast<int>(regJoints.size()) - 1, static_cast<int>(joints.size()) - 1, point))
            return false;

#ifdef DEBUG_ROBOT
        qDebug("Robot::setRegional(), koniec petli\n");
#endif // DEBUG_ROBOT
    }

    return true;
}


void Robot::mapThetasToServos(Lista<int> & lista)
{
    for (int i = 0; i < static_cast<int>(joints.size()) - 1; i++)
    {
        joints[i]->mapThetaToServo(lista);
    }
}


////////////////////////////////////////////////////////////////////// setters & getters & adders

void Robot::setJointConversionMinMax(int joint, int min, int max)
{
    joints[joint]->setConversionMinMaxDeg(min, max);
}

int Robot::getJointConversionMin(int joint)
{
    return joints[joint]->getConversionMinDeg();
}
int Robot::getJointConversionMax(int joint)
{
    return joints[joint]->getConversionMaxDeg();
}

void Robot::setJointConstructionMinMax(int joint, int min, int max)
{
    joints[joint]->setConstructionMinMaxDeg(min, max);
}

int Robot::getJointConstructionMin(int joint)
{
    return joints[joint]->getConstructionMinDeg();
}

int Robot::getJointConstructionMax(int joint)
{
    return joints[joint]->getConstructionMaxDeg();
}

void Robot::addJointServoMinMax(int joint, int min, int max)
{
    joints[joint]->addServoMinMax(min, max);
}

int Robot::getDOF()
{
    return DOF;
}

double Robot::getJointThetaRad(int joint)
{
    return joints[joint]->getTheta();
}

Eigen::Vector3d & Robot::getTCPlocation()
{
    return TCP.getLocation();
}

void Robot::setTCPaLength(double l)
{
    TCP.setaLength(l);

    updateDHmatrices();
    updateDHvectors();
}

void Robot::addRegJoint(double a, double al, double dl)
{
    regJoints.push_back(Joint(a * DEG_TO_RAD, al, dl));

    // dodanie do listy wszystkich przegubow wskaznik na teraz dodawany
    joints.insert(joints.iteratorAt(static_cast<int>(regJoints.size()) - 1),
                  &(regJoints[static_cast<int>(regJoints.size()) - 1]));

    DOF++;

    updateDHmatrices();
    updateDHvectors();
}

void Robot::addLocJoint(double a, double al, double dl)
{
    locJoints.push_back(Joint(a * DEG_TO_RAD, al, dl));

    joints.insert(joints.iteratorAt(static_cast<int>(joints.size()) - 1),
                  &(locJoints[static_cast<int>(locJoints.size()) - 1]));

    DOF++;

    updateDHmatrices();
    updateDHvectors();
}


void Robot::setThetaDeg(int joint, double theta)
{
    joints[joint]->setTheta(theta*DEG_TO_RAD);

    updateDHmatrices();
    updateDHvectors();
}
void Robot::setThetaRad(int joint, double theta)
{
    joints[joint]->setTheta(theta*DEG_TO_RAD);

    updateDHmatrices();
    updateDHvectors();
}

Eigen::Vector3d & Robot::getJointLocation(int joint)
{
    return joints[joint]->getLocation();
}

Eigen::Matrix4d & Robot::getJointDHmatrix(int joint)
{
    return joints[joint]->getDHmatrix();
}

Eigen::Vector3d & Robot::getJointZinGlobal(int joint)
{
    return joints[joint]->getZinGlobal();
}

int Robot::getJointServoAmount(int joint)
{
    return joints[joint]->getServoAmount();
}

int Robot::getRegJointsAmount()
{
    return static_cast<int>(regJoints.size());
}
int Robot::getLocJointsAmount()
{
    return static_cast<int>(locJoints.size());
}

//////////////////////////////////////////////////////////////////// !setters & getters & adders



////////////////////////////////////////////////////////////// other
void constrain(double & x, double min, double max)
{
    if (x < min)
        x = min;
    if (x > max)
        x = max;
}
