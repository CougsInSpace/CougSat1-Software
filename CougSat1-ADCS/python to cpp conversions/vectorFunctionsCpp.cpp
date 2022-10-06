#include <iostream>
// #include "../../CISLibrary/Eigen/Dense"
// #include "../../CISLibrary/Eigen/Geometry"
#include "../../CISLibrary/Eigen/Eigen.h"

using Eigen::MatrixXd;
using Eigen::Quaternion;
using namespace Eigen;
using namespace std;

#define FLOAT_COMPARISON_EPSILON 0.00001 //value used for comparing floats to 0 





bool floatCloseToZero(float f);
Vector4f rodRotation(Vector3f a, Vector3f aT);
Vector3f rotationVector(Vector3f a, Vector3f aT);
float vecAngle(Vector3f u, Vector3f v);
Vector3f saturate(Vector3f v, float min, float max);
float vecOrthoAngle(Vector3f u, Vector3f v, Vector3f ax);
Vector3f planeProject(Vector3f v, Vector3f n);
Vector3f vectorProject(Vector3f v, Vector3f u);
Vector3f findTorque(Vector3f u, Vector3f v, Vector3f mag);
Vector3f applyRodRotation(Vector3f v, Vector4f rod);
Vector3f torque2Dipole(Vector3f torqueDir, Vector3f mag);

bool floatCloseToZero(float f) {
    return f < FLOAT_COMPARISON_EPSILON;
}
 


int main() {

    //Saturate and torque2Dipole seem to be working fine, still requires testing



    //Saturate function test
    //test param Vector(5,10,15)
    //min:3 , max: 16

    // Vector3f satTest(5,10,15);
    // Vector3f satTest1(9,20,-4);
    // Vector3f satTest2(-3, -4,9.5);
    // Vector3f satTest3(20,30,-90);

    // float min = 3;
    // float max = 12; 

    
    // Vector3f satReturn = saturate(satTest,min,max);
    // cout << satReturn;  

    //findTorque test
    //test param: u(5,5,5) v(7,7,7) mag(3,2,1)
    Vector3f u(5,5,5); 
    Vector3f v(4,7,7);
    Vector3f mag(3,2,1);

    // cout << findTorque(u,v,mag); 


    //applyRodRotation test
    //test param: v2(4,4,4) rod(4,4,4,50)

    Vector4f rod(4,-3,4,0);
    Vector3f v2(4,-3,4);

    // cout << applyRodRotation(v2,rod);


    //torque2diople test
    //param: tordueDir(5,5,5) mag2(6,6,6)

    Vector3f torqueDir(5,5,5);
    Vector3f mag2(3,2,1);

    cout << torque2Dipole(torqueDir,mag2); 


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

//TODO: untested, implementation needed, need to doulbe check 
Vector3f applyRodRotation(Vector3f v, Vector4f rod) {
    Vector3f k;
    k << rod(0), rod(1), rod(2);

    float theta = rod[3]; 

    k.normalize(); 
    v.normalize();

    // add scalars 
    Vector3f vT = (v * cos(theta)) + (k.cross(v) * sin(theta)) + (k * k.dot(v))*(1 - cos(theta));
    return vT; 
 } 

//TODO: Untested
Vector3f torque2Dipole(Vector3f torqueDir, Vector3f mag) {

    Vector3f dipoleDir = mag.cross(torqueDir);

    return dipoleDir;
}



//TODO: test functions, find thetaErrorVec
Vector3f findTorque(Vector3f u, Vector3f v, Vector3f mag) {

    Vector3f w1 = u.cross(v); 

    Vector3f w2 = (u + v) / 2; 

    Vector3f norm = w1.cross(w2);
    Vector3f torque = (norm.cross(mag)); 

    float theta = vecOrthoAngle(u,v,torque);
    Vector4f rod;
    rod << -1*torque, theta;
    Vector3f vec = applyRodRotation(u,rod); 

    if (vecAngle(vec,v) < 0.00001){
    torque = (-1 * torque);
    }

    return torque; 

}


//TODO: testing, sytax check
Vector3f saturate(Vector3f v, float min, float max) {


    for (int i = 0; i < v.size(); i++) {
        if (v[i] > max || v[i] < min) {
            v *= abs(max / v[i]);
        }

    }

    return v; 
}



// these functions are already tested 
//theta error
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

Vector3f planeProject(Vector3f v, Vector3f n) {
    v.normalize();
    n.normalize();
    Vector3f vProj = vectorProject(n,v);
    return v - vProj;
}

Vector3f vectorProject(Vector3f v, Vector3f u) {
    // project u onto v
    float vn = v.norm();
    float un = u.norm();

    return v * (v.dot(u) / (vn * un));
}

