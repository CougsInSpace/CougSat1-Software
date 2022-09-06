#ifndef kalmanFilter_H_
#define kalmanFilter_H_

#include "../../CISLibrary/Eigen/Dense"
#include "../../CISLibrary/Eigen/Geometry"
#include "../../CISLibrary/Eigen/Cholesky"
#include "satelliteState.h"
#include <mbed_stats.h>

using Eigen::MatrixXd;
using Eigen::Quaternion;
using namespace Eigen;
using namespace std;

struct returnKalman {
    SatelliteState stateEst;
    MatrixXf covEst;
};

returnKalman qFilter(SatelliteState lastStateEst,
    MatrixXf lastEstErrCovariance, 
    MatrixXf procNoiseCovariance,
    Matrix3f R,
    Vector3f measi,
    Vector3f measf,
    float deltaT);
MatrixXf choleskyDecomposition(MatrixXf A, short dimensionSize);
Vector3f qToRotVec(Quaternionf q);
Quaternionf rotVecToQ(Vector3f rotVec);
MatrixXf stateArrToSigmaMat(SatelliteState stateArr[12]);
SatelliteState averageState(SatelliteState stateArr[12], Quaternionf avgCalcStartQ);
MatrixXf step1(MatrixXf mat);
SatelliteState step12(MatrixXf xHat_k);

#endif