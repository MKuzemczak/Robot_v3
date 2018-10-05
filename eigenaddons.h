#ifndef EIGENADDONS_H
#define EIGENADDONS_H


#include <string>

#include <EigenLib/Eigen/Dense>
#include <EigenLib/Eigen/LU>

#include <QDebug>

#include "serialport.h"


#define SMALLEST 0.000001
#define DEG_TO_RAD 0.01745329251
#define PI 3.14159265359

template <typename Scalar, int RowsAtCompileTime, int ColsAtCompileTime>
    SerialPort & operator <<(SerialPort & port, Eigen::Matrix< Scalar, RowsAtCompileTime, ColsAtCompileTime> &m)
    {
        std::string s;

        for (int i = 0, rows = m.rows(), cols = m.cols(); i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                s.append(std::to_string(m(i,j)));
                s.append("  ");
            }
            s.append("\n");
        }

        port << s;

        return port;
    }

template <typename Scalar, int RowsAtCompileTime, int ColsAtCompileTime>
    QDebug & operator << (QDebug & port, Eigen::Matrix< Scalar, RowsAtCompileTime, ColsAtCompileTime> &m)
    {
        std::string s;

        for (int i = 0, rows = m.rows(), cols = m.cols(); i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                s.append(std::to_string(m(i,j)));
                s.append("  ");
            }
            s.append("\n");
        }

        port << s.c_str();

        return port;
    }


template <typename Scalar, int RowsAtCompileTime, int ColsAtCompileTime>
    void printNumbers(SerialPort & port, Eigen::Matrix< Scalar, RowsAtCompileTime, ColsAtCompileTime> &m)
    {
        std::string s;

        for (int i = 0, rows = m.rows(), cols = m.cols(); i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                s.append(std::to_string(m(i, j)));
                s.append("\n");
            }
        }

        port << s;
    }

template <class MatT>
    Eigen::Matrix<typename MatT::Scalar, MatT::ColsAtCompileTime, MatT::RowsAtCompileTime>
    psInv(const MatT &mat, typename MatT::Scalar tolerance = typename MatT::Scalar{ 1e-4 }) // choose appropriately
    {
        typedef typename MatT::Scalar Scalar;
        auto svd = mat.jacobiSvd(Eigen::ComputeFullU | Eigen::ComputeFullV);
        const auto &singularValues = svd.singularValues();
        Eigen::Matrix<Scalar, MatT::ColsAtCompileTime, MatT::RowsAtCompileTime> singularValuesInv(mat.cols(), mat.rows());
        singularValuesInv.setZero();

        for (int i = 0; i < static_cast<int>(singularValues.size()); ++i) {
            if (singularValues(i) > tolerance)
            {
                singularValuesInv(i, i) = Scalar{ 1 } / singularValues(i);
            }
            else
            {
                singularValuesInv(i, i) = Scalar{ 0 };
            }
        }
        return svd.matrixV() * singularValuesInv * svd.matrixU().adjoint();
    }

#endif // EIGENADDONS_H
