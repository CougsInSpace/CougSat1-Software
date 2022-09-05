#include "satelliteState.h"

SatelliteState::SatelliteState() {

}

SatelliteState::SatelliteState(float omega1, float omega2, float omega3, float qw, float qx, float qy, float qz) {
    this->initFloats(omega1, omega2, omega3, qw, qx, qy, qz);
}

SatelliteState::SatelliteState(Vector3f omega, Quaternionf q) {
    this->initVecQ(omega, q);
}

SatelliteState::SatelliteState(MatrixXf mat) {
    this->initMat(mat);
}

void SatelliteState::initFloats(float omega1, float omega2, float omega3, float qw, float qx, float qy, float qz) {
    this->stateVec.omega1 = omega1;
    this->stateVec.omega2 = omega2;
    this->stateVec.omega3 = omega3;
    this->stateVec.qw = qw;
    this->stateVec.qx = qx;
    this->stateVec.qy = qy;
    this->stateVec.qz = qz;
    return;
}

void SatelliteState::initVecQ(Vector3f omega, Quaternionf q) {
    this->stateVec.omega1 = omega(0);
    this->stateVec.omega2 = omega(1);
    this->stateVec.omega3 = omega(2);
    this->stateVec.qw = q.coeffs()[3];
    this->stateVec.qx = q.coeffs()[0];
    this->stateVec.qy = q.coeffs()[1];
    this->stateVec.qz = q.coeffs()[2];
    return;
}

void SatelliteState::initMat(MatrixXf mat) {
    this->stateVec.omega1 = mat(0,0);
    this->stateVec.omega2 = mat(1,0);
    this->stateVec.omega3 = mat(2,0);
    this->stateVec.qw = mat(3,0);
    this->stateVec.qx = mat(4,0);
    this->stateVec.qy = mat(5,0);
    this->stateVec.qz = mat(6,0);
    return;
}

Vector3f SatelliteState::qToRotVec(Quaternionf q) {
    float a = acos(q.coeffs()[3]) * 2;
    Vector3f axis(q.coeffs()[0], q.coeffs()[1], q.coeffs()[2]);
    axis.normalize();

    return (a * axis);
}

MatrixXf SatelliteState::getMat() {
    MatrixXf stateMat(7,1);
    stateMat(0,0) = stateVec.omega1;
    stateMat(1,0) = stateVec.omega2;
    stateMat(2,0) = stateVec.omega3;
    stateMat(3,0) = stateVec.qw;
    stateMat(4,0) = stateVec.qx;
    stateMat(5,0) = stateVec.qy;
    stateMat(6,0) = stateVec.qz;
    return stateMat;
}

Quaternionf SatelliteState::getQ() {
    Quaternionf q(this->stateVec.qw, this->stateVec.qx, this->stateVec.qy, this->stateVec.qz);
    return q;
}

Vector3f SatelliteState::getOmega() {
    Vector3f omega(this->stateVec.omega1, this->stateVec.omega2, this->stateVec.omega3);
    return omega;
}

Vector3f SatelliteState::getRot() {
    Quaternionf q(this->stateVec.qw, this->stateVec.qx, this->stateVec.qy, this->stateVec.qz);
    return qToRotVec(q);
}

MatrixXf SatelliteState::getRotMat() {
    MatrixXf rotMat(6,1);
    Quaternionf q(this->stateVec.qw, this->stateVec.qx, this->stateVec.qy, this->stateVec.qz);
    Vector3f rotVec = qToRotVec(q);
    Vector3f omega(this->stateVec.omega1, this->stateVec.omega2, this->stateVec.omega3);
    rotMat << omega, rotVec;

    return rotMat;
}