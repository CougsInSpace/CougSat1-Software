#include "vectorFunctions.h"

#include <iostream>
using namespace std;

#define FLOAT_COMPARISON_EPSILON 0.00001 //value used for comparing floats to 0 

bool floatCloseToZero(float f) {
    return f < FLOAT_COMPARISON_EPSILON;
}
Vector4f rodRotation(Vector3f a, Vector3f aT); //forward declaration 

void testRodRotation() {
    Vector3f a(1, 0, 0); 
    Vector3f aT(0, 0, -1); 
    Vector4f rod = rodRotation(a, aT); 
    std::cout << rod << std::endl;
}
int main() {
    // testRodRotation();

    return 0;
}
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
    Vec3 rotVec = axis * theta; 
    return rotVec
}



























Quaternionf determineAttitude(Vector3f x1i, Vector3f x1f, Vector3f x2i, Vector3f x2f) {
    // find simple axis-angle between x1i and x1f
    Vector3f ax1 = x1i.cross(x1f);
    ax1.normalize();
    float theta1 = vecAngle(x1i, x1f);

    // create the first quaternion
    AngleAxisf axAng1(theta1, ax1);
    Quaternionf q1(axAng1);

    // apply axAng1 rotation to x2i to get new x2i called x2Prime
    Quaternionf x2iq(0, x2i(0), x2i(1), x2i(2));
    Quaternionf x2Primeq = q1 * x2iq * q1.conjugate();
    Vector3f x2Prime(x2Primeq.coeffs()[0], x2Primeq.coeffs()[1], x2Primeq.coeffs()[2]);

    // define the axis for the second axis-angle rotation
    Vector3f ax2 = x1f;
    ax2.normalize();

    // find the angle for the second axis-angle rotation
    double theta2 = vecOrthoAngle(x2Prime, x2f, ax2);

    // check the direction of the second rotation
    // this is an extra step that is only neccesary for the second rotation
    Vector3f dirCheckVec = x2Prime.cross(x2f);
    //TODO try catch for divide by zero
    int8_t dirCheck;
    ax2.dot(dirCheckVec) >=0 ? dirCheck = 1 : dirCheck = -1;
    ax2 *= dirCheck;

    // create the second quaternion
    AngleAxisf axAng2(theta2, ax2);
    Quaternionf q2(axAng2);

    // calculate the final rotation by multiplying the two quaternion rotations
    Quaternionf bigQ =  q2 * q1;

    return bigQ; // not commutative
}

Vector3f vectorProject(Vector3f v, Vector3f u) {
    // project u onto v
    float vn = v.norm();
    float un = u.norm();

    return v * (v.dot(u) / (vn * un));
}

Vector3f planeProject(Vector3f v, Vector3f n) {
    v.normalize();
    n.normalize();
    Vector3f vProj = vectorProject(n,v);
    return v - vProj;
}

float vecAngle(Vector3f u, Vector3f v) {
    float vn = v.norm();
    float un = u.norm();

    float theta = acos(v.dot(u) / (vn * un));
    return theta;
}

float vecOrthoAngle(Vector3f u, Vector3f v, Vector3f ax) {
    Vector3f uOrtho = planeProject(u,ax);
    Vector3f vOrtho = planeProject(v,ax);

    return vecAngle(uOrtho, vOrtho);
}

