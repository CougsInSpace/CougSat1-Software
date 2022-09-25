#include <iostream>
#include "../../CISLibrary/Eigen/Eigen_Repo/Eigen/Dense"
#include "../../CISLibrary/Eigen/Eigen_Repo/Eigen/Geometry"

using Eigen::MatrixXd;
using Eigen::Quaternion;
using namespace Eigen;
using namespace std;

#define FLOAT_COMPARISON_EPSILON 0.00001 //value used for comparing floats to 0 

bool floatCloseToZero(float f);
Vector4f rodRotation(Vector3f a, Vector3f aT);
Vector3f rotationVector(Vector3f a, Vector3f aT);
float vecAngle(Vector3f u, Vector3f v);

bool floatCloseToZero(float f) {
    return f < FLOAT_COMPARISON_EPSILON;
}
 


int main() {

    return 0;
}

// difference between the different normalize functions 
// https://stackoverflow.com/questions/48019329/difference-between-norm-normalize-and-normalized-in-eigen
//
Vector4f rodRotation(Vector3f a, Vector3f aT) {
    Vector3f axis = a.cross(aT); //cross product
    float theta = 0;
    if (floatCloseToZero(axis.norm())) {
        cout << "float is close to 0\n";
        axis[0] = 0; //x 
        axis[1] = 0; //y 
        axis[2] = 1; //z
        theta = 0; 
    } else {
        cout << "float is not close to 0\n";
        axis.normalize(); //modifies axis in place
        theta = vecAngle(a, aT);
    }
    Vector4f rod(axis[0], axis[1], axis[2], theta); //sets the values in rod
    return rod;
}

// TODO This function is still untested
Vector3f rotationVector(Vector3f a, Vector3f aT) {
    Vector3f axis = a.cross(aT); 
    //handle case if a and aT are parallel 
    float theta = 0;
    if (floatCloseToZero(axis.norm())) {
        cout << "float is close to 0\n"; 
        axis[0] = 0; //x 
        axis[1] = 0; //y 
        axis[2] = 1; //z 
        theta = 0;
    } else {
        cout << "float is not close to 0\n";
        axis.normalize();
        theta = vecAngle(a, aT); 
    }
    Vector3f rotVec = axis * theta; 
    return rotVec;
}









float vecAngle(Vector3f u, Vector3f v) {
    float vn = v.norm();
    float un = u.norm();

    float theta = acos(v.dot(u) / (vn * un));
    return theta;
}