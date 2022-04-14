#ifndef vectorFunctions_H_
#define vectorFunctions_H_

#include "../../CISLibrary/Eigen/Dense"
#include "../../CISLibrary/Eigen/Geometry"
#include <iostream>

using Eigen::MatrixXd;
using Eigen::Quaternion;
using namespace Eigen;

Quaternionf determineAttitude(Vector3f x1i, Vector3f x1f, Vector3f x2i, Vector3f x2f);
Vector3f vectorProject(Vector3f v, Vector3f u);
Vector3f planeProject(Vector3f v, Vector3f n);
Vector3f vectorProject(Vector3f v, Vector3f u);
float vecAngle(Vector3f u, Vector3f v);
float vecOrthoAngle(Vector3f u, Vector3f v, Vector3f ax);

#endif /*vectorFunctions_H_*/