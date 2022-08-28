#ifndef kalmanFilter_H_
#define kalmanFilter_H_

#include "../../CISLibrary/Eigen/Dense"
#include "../../CISLibrary/Eigen/Geometry"
#include "satelliteState.h"

using Eigen::MatrixXd;
using Eigen::Quaternion;
using namespace Eigen;

void qFilter(SatelliteState lastStateEst,
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

#endif