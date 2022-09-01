#include "kalmanFilter.h"


returnKalman qFilter(SatelliteState lastStateEst,
    MatrixXf lastEstErrCovariance, 
    MatrixXf procNoiseCovariance,
    Matrix3f R,
    Vector3f measi,
    Vector3f measf,
    float deltaT) {
    /* lastStateQ = the quaternion component of the previous recursions state vector
       lastStateOmega = the angular velocity component of the previous recursions state vector
       lastEstErrCovariance = the covariance matrix of the last estimates error
       procNoiseCovariance = the covariance matrix of the process noise. I believe this is just a catchall for unpredicatable noise
                             and will need to be chosen manually
    */

    // Step 0
    // Combine estimate covariance and process noise covariance
    MatrixXf lastCovariance = lastEstErrCovariance + procNoiseCovariance;

    // Step 1
    // Create set of state vectors with desired covariance and mean of 0
    LLT<MatrixXf> lltOfA(lastCovariance);
    MatrixXf S = lltOfA.matrixL(); // cholesky decomposition
    S = S * sqrt(12);
    MatrixXf W_i(6, 12);
    W_i << S, -1 * S; // concatenate matrices

    // Step 2
    // Build set of state vector sigma points X_i by converting rot vector to quaternion representation
    // and shifting the sigma points by the last error estimate
    SatelliteState X_i[12];
    for (short i = 0; i < 12; i++) {
        Vector3f omega(W_i(0,i), W_i(1,i), W_i(2,i));
        omega = omega + lastStateEst.getOmega();
        Vector3f rot(W_i(3,i), W_i(4,i), W_i(5,i));
        Quaternionf q = lastStateEst.getQ() * rotVecToQ(rot);
        X_i[i].initVecQ(omega, q);
    }
    MatrixXf X_i_Mat = stateArrToSigmaMat(X_i);
    // Quaternionf qTest = lastStateEst.getQ() * lastStateEst.getQ();
    // cout << averageState(X_i, qTest).getQ() << endl;
    // cout << "______" << endl;

    // Step 3
    // Predict value of state vector at next time interval
    SatelliteState Y_i[12];
    for (short i = 0; i < 12; i++) {
        // find change in orientation over deltaT
        Vector3f deltaRot = X_i[i].getOmega() * deltaT;
        // find new quaternion orientation after deltaT 
        Quaternionf newStateQ = rotVecToQ(deltaRot) * X_i[i].getQ();
        // init sigma point in Y_i
        Y_i[i].initVecQ(X_i[i].getOmega(), newStateQ);
    }

    // Step 4
    // Find average state vector of Y_i
    // cout << lastStateEst.getQ() << endl;
    SatelliteState xHat_kMinus = averageState(Y_i, lastStateEst.getQ());
    // cout << xHat_kMinus.getQ() << endl;

    //Step 5
    // Create WPrime_i matrix by subtracting the mean vector and converting the quaternion rotation into a rotation vector
    MatrixXf WPrime_i(6,12);
    for (short i = 0; i < 12; i++) {
        Vector3f omega = Y_i[i].getOmega() - xHat_kMinus.getOmega();
        Vector3f rot = qToRotVec(xHat_kMinus.getQ().conjugate() * Y_i[i].getQ());

        for (short j = 0; j < 3; j++) {
            // WPrime_i is upside down from W_i for some reason
            WPrime_i(j, i) = rot(j);
            WPrime_i(j + 3,i) = omega(j);
        }
    }

    //Step 6
    //Find a priori covariance of WPrime_i
    MatrixXf P_kMinus(6,6);
    for (short i = 0; i < 12; i++) {
        P_kMinus += WPrime_i.col(i) * WPrime_i.col(i).transpose();
    }

    // Step 7
    // Predict measurement vector direction using measurement model
    Quaternionf suniq(0, measi(0), measi(1), measi(2));// TODO HOW TO ACCOUNT FOR MULTIPLE MEASUREMENT VECTORS? MAYBE SWITCH BETWEEN THEM ON EACH RECURSION?
    MatrixXf Z_i_Mat(3, 12);
    for (short i = 0; i < 12; i++) {
        Quaternionf sunfqPredicted = Y_i[i].getQ() * suniq * Y_i[i].getQ().conjugate();
        for (short j = 0; j < 3; j++) {
            Z_i_Mat(j,i) = sunfqPredicted.coeffs()[j];
        }
    }

    // Step 8
    // Find average of Z_i to get a priori estimate of z_k and find the difference between the predicted and actual
    // measurement, the innovation v_k.
    // Average predicted measurement
    Vector3f z_kMinus(0,0,0);
    for (short i = 0; i < 12; i++) {
        Vector3f z_i(Z_i_Mat(0,i), Z_i_Mat(1,i), Z_i_Mat(2,i));
        z_kMinus += z_i;
    }
    z_kMinus /= 12;

    // The innovation
    Vector3f v_k = measf - z_kMinus;
    // cout << v_k << endl;

    // Step 9
    // Find innovation covariance P_vv by adding measurement noise R to measurement covariance P_zz
    // Create matrix out of vector z_kMinus
    MatrixXf Z_kMinusMat(3, 12);
    for (short i = 0; i < 12; i++) {
        Z_kMinusMat.col(i) = z_kMinus;
    }
    // Find P_zz
    MatrixXf Z_i_MatCov = Z_i_Mat - Z_kMinusMat;
    Matrix3f P_zz;
    for (short i = 0; i < 12; i++) {
        P_zz += Z_i_MatCov.col(i) * Z_i_MatCov.col(i).transpose();
    }
    P_zz /= 12;
    Matrix3f P_vv = P_zz + R;

    // Step 10
    // Calculate the cross correlation matrix P_xz
    MatrixXf P_xz(6,3);
    for (short i = 0; i < 12; i++) {
        P_xz += WPrime_i.col(i) * Z_i_MatCov.col(i).transpose();
    }
    P_xz /= 12;

    // Step 11
    // Calculate kalman gain K_k and apply it to state estimate xHat_k and estimate error covariance P_k
    MatrixXf K_k = P_xz * P_vv.inverse(); 
    MatrixXf xHat_k = xHat_kMinus.getRotMat() + (K_k * v_k); // State estimate
    MatrixXf P_k = P_kMinus - (K_k * P_vv * K_k.transpose()); // Covariance estimate
    
    //Step 12
    // Create updated estimate of covariance and state vector
    Vector3f omegaTemp(xHat_k(0,0),xHat_k(1,0),xHat_k(2,0));
    Vector3f rotTemp(xHat_k(3,0),xHat_k(4,0),xHat_k(5,0));
    SatelliteState updatedStateEst(omegaTemp, rotVecToQ(rotTemp));

    returnKalman stateCov;
    stateCov.stateEst = updatedStateEst;
    stateCov.covEst = P_k;
    
    return stateCov;
}

Vector3f qToRotVec(Quaternionf q) {
    float a = acos(q.coeffs()[3]) * 2;
    Vector3f axis(q.coeffs()[0], q.coeffs()[1], q.coeffs()[2]);
    axis.normalize();

    return (a * axis);
}

Quaternionf rotVecToQ(Vector3f rotVec) {
    float a = rotVec.norm();
    float sina = sin(a/2);
    rotVec.normalize();
    Quaternionf q(cos(a/2), sina * rotVec(0), sina * rotVec(1), sina * rotVec(2));
    return q;
}

MatrixXf stateArrToSigmaMat(SatelliteState stateArr[12]) {
    MatrixXf stateMat(7, 12);
    for (short i = 0; i < 12; i++) {
        stateMat.col(i) = stateArr[i].getMat();
    }
    return stateMat;
}

SatelliteState averageState(SatelliteState stateArr[12], Quaternionf avgCalcStartQ) {
    // Was giving wrong results because covariance matrix was too high
    // Find average angular velocity vector
    Vector3f omegaAvg(0,0,0);
    for (short i = 0; i < 12; i++) {
        omegaAvg = omegaAvg + stateArr[i].getOmega();
    }
    omegaAvg /= 12;

    //Find average quaternion orientation
    Quaternionf q_tAvg = avgCalcStartQ;
    Vector3f eRotAvg(1,1,1); 
    short counter = 0;
    while (eRotAvg.norm() > .01 && counter < 100) { // TODO pick appropriate error threshold
        // find quaternion rotation between each stateArr quaternion and the average 
        // quaternion
        Quaternionf e_iArr[12];
        for (short i = 0; i < 12; i++) {
            e_iArr[i] = stateArr[i].getQ() * q_tAvg.conjugate();
        }

        // convert quaternion rotations to rotation vectors
        Vector3f e_iRotArr[12];
        for (short i = 0; i < 12; i++) {
            e_iRotArr[i] = qToRotVec(e_iArr[i]);
        }
        
        // find average rotation vector from set of 12
        eRotAvg = Vector3f::Zero();
        for (short i = 0; i < 12; i++) {
            eRotAvg = eRotAvg + e_iRotArr[i];
        }
        eRotAvg /= 12;

        // update mean estimate
        q_tAvg = rotVecToQ(eRotAvg) * q_tAvg;
        // cout << q_tAvg << endl;
        
        counter++;

    }
    // cout << counter << endl;
    // cout << stateArrToSigmaMat(stateArr) << endl;

    SatelliteState state(omegaAvg, q_tAvg);
    return state;
}