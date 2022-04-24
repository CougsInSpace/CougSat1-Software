#include "vectorFunctions.h"

Quaternionf determineAttitude(Vector3f x1i, Vector3f x1f, Vector3f x2i, Vector3f x2f) {
    Vector3f ax1 = x1i.cross(x1f);
    ax1.normalize();
    float theta1 = vecAngle(x1i, x1f);
    AngleAxisf axAng1(theta1, ax1);
    Quaternionf q1(axAng1);

    Quaternionf x2iq(0, x2i(0), x2i(1), x2i(2));
    Quaternionf x2Primeq = q1 * x2iq * q1.conjugate();
    Vector3f x2Prime(x2Primeq.coeffs()[0], x2Primeq.coeffs()[1], x2Primeq.coeffs()[2]);

    Vector3f ax2 = x1f;
    ax2.normalize();
    // std::cout << "ax2: " << ax2 << std::endl;

    double theta2 = vecOrthoAngle(x2Prime, x2f, ax2);
    // std::cout << "theta2: " << theta2 << std::endl;
    Vector3f dirCheckVec = x2Prime.cross(x2f);
    // std::cout << "dirCheckVec: " << dirCheckVec << std::endl;
    //TODO try catch for divide by zero
    int8_t dirCheck;
    ax2.dot(dirCheckVec) >=0 ? dirCheck = 1 : dirCheck = -1;
    // std::cout << "dirCHeck: " << dirCheck << std::endl;
    ax2 *= dirCheck;
    // std::cout << "ax2: " << ax2 << std::endl;
    AngleAxisf axAng2(theta2, ax2);
    //std::cout << "axAng2: " << axAng2 << std::endl;
    Quaternionf q2(axAng2);
    // std::cout << "q2: " << q2 << std::endl;


    // std::cout << "Q1: " << q1 << std::endl;
    // std::cout << "Q2: " << q2 << std::endl;
    Quaternionf bigQ =  q2 * q1;
    // std::cout << "BIG DOG Q : " << bigQ << std::endl;
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

