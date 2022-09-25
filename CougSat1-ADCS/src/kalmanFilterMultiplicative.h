#ifndef kalmanFilter_H_
#define kalmanFilter_H_

// #include "../../CISLibrary/Eigen/Dense"
// #include "../../CISLibrary/Eigen/Geometry"
// #include "../../CISLibrary/Eigen/Cholesky"
#include <Eigen.h>
// #include "..\..\CISLibrary\Eigen\Eigen_Repo\Eigen\Geometry"
#include "satelliteState.h"

using Eigen::MatrixXd;
using Eigen::Quaterniond;
using namespace Eigen;
using namespace std;

struct ReturnKalman {
    Vector3d wk1_B2I_B;
    Quaterniond qk1_I2B;
    Vector3d biask1;
    MatrixXd Pk1;
};

ReturnKalman multiplicativeFilter(Vector3d wk_B2I_B, 
    Quaterniond qk_I2B,
    Vector3d biask, 
    Vector3d SMeas, 
    Vector3d SRef,
    bool eclp,
    Vector3d BMeas,
    Vector3d BRef,
    float dt,
    float sig_r,
    float sig_w,
    Matrix3d R_ss,
    Matrix3d R_m,
    MatrixXd Pk);
Matrix3d skew(Vector3d vec);
MatrixXd construct6by6(Matrix3d mat_11, Matrix3d mat_12, Matrix3d mat_21, Matrix3d mat_22);
int factorial(int x);

#endif