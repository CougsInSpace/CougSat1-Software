#pragma once

#include <stdint.h>
#include <Eigen.h>
#include <iostream>
// #include "Quaternion.h"
// #include "..\..\Eigen\Eigen_Repo\Eigen\Geometry"
// #include "..\..\Eigen\Eigen_Repo\Eigen\LU"

#define STATE_SIZE 15
#define MEASSUREMENT_GYR_SIZE 3
#define MEASSUREMENT_ACC_SIZE 3
#define MEASSUREMENT_MAG_SIZE 3

using Eigen::Quaternion;
using namespace Eigen;
using namespace std;

namespace IMU_EKF
{

template <typename precision>
class ESKF
{
public:
    ESKF();
    void init();
    void initWithAcc(const float ax, const float ay, const float az);
    void initWithAccAndMag(const float ax, const float ay, const float az, const float mx, const float my, const float mz, const Eigen::Matrix<precision, 3, 3> &Winv, const Eigen::Matrix<precision, 3, 1> &V);
    void predict(precision dt);
    void correctGyr(const float gx, const float gy, const float gz);
    void correctMeasurement(const float mxi, const float myi, const float mzi, const float mxf, const float myf, const float mzf);
    void correctMag(const float mx, const float my, const float mz, const float incl, const float B, const Eigen::Matrix<precision, 3, 3> &W, const Eigen::Matrix<precision, 3, 1> &V);
    void reset();

    Eigen::Matrix<precision, STATE_SIZE, 1> getState() const;
    void getAttitude(float &roll, float &pitch, float &yaw) const;
    Eigen::Quaternion<precision> getAttitude() const;
    void getAcceleration(float &x, float &y, float &z) const;

private:
    Eigen::Matrix<precision, 3, 3> toCrossMatrix(Eigen::Matrix<precision, 3, 1> vec);
    Eigen::Matrix<precision, STATE_SIZE, 1> x_;
    Eigen::Quaternion<precision> qref_;
    Eigen::Matrix<precision, STATE_SIZE, STATE_SIZE> P_;

    Eigen::Matrix<precision, STATE_SIZE, STATE_SIZE> Q_;
    Eigen::Matrix<precision, MEASSUREMENT_GYR_SIZE, MEASSUREMENT_GYR_SIZE> R_Gyr_;
    Eigen::Matrix<precision, MEASSUREMENT_ACC_SIZE, MEASSUREMENT_ACC_SIZE> R_Acc_;
    Eigen::Matrix<precision, MEASSUREMENT_MAG_SIZE, MEASSUREMENT_MAG_SIZE> R_Mag_;
};
} // namespace IMU_EKF

// ugly but necessary
// to get templates working
#include "ESKF.cpp"
