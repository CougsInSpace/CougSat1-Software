#ifndef kalmanFilter_H_
#define kalmanFilter_H_

// #include "../../CISLibrary/Eigen/Dense"
// #include "../../CISLibrary/Eigen/Geometry"
// #include "../../CISLibrary/Eigen/Cholesky"
#include <Eigen.h>
#include "..\..\CISLibrary\Eigen\Eigen_Repo\Eigen\Geometry"
#include "satelliteState.h"

using Eigen::MatrixXd;
using Eigen::Quaternion;
using namespace Eigen;
using namespace std;

struct ReturnKalman {
    Vector3f wk1_B2I_B;
    Quaternionf qk1_I2B;
    Vector3f biask1;
    MatrixXf Pk1;
};

ReturnKalman multiplicativeFilter(Vector3f wk_B2I_B, 
    Quaternionf qk_I2B,
    Vector3f biask, 
    Vector3f SMeas, 
    Vector3f SRef,
    bool eclp,
    Vector3f BMeas,
    Vector3f BRef,
    float dt,
    float sig_r,
    float sig_w,
    Matrix3f R_ss,
    Matrix3f R_m,
    MatrixXf Pk);
Matrix3f skew(Vector3f vec);
MatrixXf construct6by6(Matrix3f mat_11, Matrix3f mat_12, Matrix3f mat_21, Matrix3f mat_22);
int factorial(int x);

#endif