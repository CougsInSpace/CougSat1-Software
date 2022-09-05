#ifndef SatelliteState_H
#define SatelliteState_H

#include "../Eigen/Dense"
#include "../Eigen/Geometry"
#include <iostream>

using Eigen::MatrixXd;
using Eigen::Quaternion;
using namespace Eigen;


typedef struct statevector {
    float omega1;
    float omega2;
    float omega3;
    float qw;
    float qx;
    float qy;
    float qz;
} StateVector;

class SatelliteState {
public:
    SatelliteState();
    SatelliteState(float omega1, float omega2, float omega3, float qw, float qx, float qy, float qz);
    SatelliteState(Vector3f omega, Quaternionf q);
    SatelliteState(MatrixXf mat);
    void initFloats(float omega1, float omega2, float omega3, float qw, float qx, float qy, float qz);
    void initVecQ(Vector3f omega, Quaternionf q);
    void initMat(MatrixXf mat);
    MatrixXf getMat(); 
    Quaternionf getQ();
    Vector3f getOmega();
    Vector3f getRot();
    MatrixXf getRotMat();
private:
    Vector3f qToRotVec(Quaternionf q);

    StateVector stateVec;
};
#endif