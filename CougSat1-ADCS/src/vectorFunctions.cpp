#include "vectorFunctions.h"

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

