#include <kalmanFilterMultiplicative.h>

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
    MatrixXf Pk) {

    // CHECK THAT BIASK DOESN'T DIVERGE
    // Function Start
    Vector3f biask1 = biask;

    Matrix3f Skew_w = skew(wk_B2I_B);

    Matrix4f Omega;
    Omega << 0,              wk_B2I_B(2), -wk_B2I_B(1), wk_B2I_B(0),
            -wk_B2I_B(2),  0,              wk_B2I_B(0), wk_B2I_B(1),
             wk_B2I_B(1), -wk_B2I_B(0),  0,             wk_B2I_B(2),
            -wk_B2I_B(0), -wk_B2I_B(1), -wk_B2I_B(2), 0            ;

    Quaternionf qk1_I2B;
    qk1_I2B.coeffs() = (MatrixXf::Identity(4,4) + (dt/2) * Omega) * qk_I2B.coeffs();
    
    Matrix3f Phi_11 = Matrix3f::Identity() - dt*Skew_w + (pow(dt,2)/2)*(Skew_w*Skew_w);
    Matrix3f Phi_12 = -1*Matrix3f::Identity()*dt + (pow(dt,2)/2)*Skew_w - (pow(dt,3)/6)*(Skew_w*Skew_w);

    MatrixXf Phi = construct6by6(Phi_11, Phi_12, Matrix3f::Zero(), Matrix3f::Identity());

    MatrixXf Gk = construct6by6(-1*Matrix3f::Identity(), Matrix3f::Zero(), Matrix3f::Zero(), Matrix3f::Identity());

    Matrix3f Qk_11 = pow(sig_r, 2)*dt*Matrix3f::Identity() + pow(sig_w,2)*(Matrix3f::Identity()*(pow(dt,3)/3) + (2*pow(dt,5)/factorial(5))*(Skew_w*Skew_w));
    Matrix3f Qk_12 = -1*pow(sig_w,2)*(Matrix3f::Identity()*(pow(dt,2)/2) - (pow(dt,3)/factorial(3))*Skew_w + (pow(dt,4)/factorial(4))*(Skew_w*Skew_w));
    Matrix3f Qk_22 = (pow(sig_w,2))*dt*Matrix3f::Identity();
    MatrixXf Qk = construct6by6(Qk_11, Qk_12, Qk_12.transpose(), Qk_22);

    MatrixXf Pk1 = Phi*Pk*Phi.transpose() + Gk*Qk*Gk.transpose();

    Matrix3f Rk1_I2B = qk1_I2B.toRotationMatrix();

    // Sun measurement vector
    VectorXf delX = VectorXf::Zero(6);
    for (short i = 0; i < 2; i++) {
        MatrixXf H(3,6);
        Matrix3f den;
        MatrixXf K;
        Vector3f res;
        if(eclp == false && i == 0) {
            Matrix3f Att2ax = Rk1_I2B.transpose();
            Att2ax.col(2) = MatrixXf::Zero(3,1);
            Att2ax.transposeInPlace();

            H << skew(Rk1_I2B*SRef), Matrix3f::Zero();
            
            den = H*Pk1*H.transpose() + R_ss;
            K = (Pk1*H.transpose())*den.inverse();

            Pk1 = (MatrixXf::Identity(6,6) - K*H)*Pk1;

            res = SMeas - Att2ax*SRef;

            delX = delX + K*(res-H*delX);
        } else if (i == 1) {

            // Mag measurement vector
            // No if statement
            H << skew(Rk1_I2B*BRef), Matrix3f::Zero();

            den = (H*Pk1*H.transpose() + R_m); // Pk1 is getting huge

            K = (Pk1*H.transpose())*den.inverse(); 

            Pk1 = (MatrixXf::Identity(6,6) - K*H)*Pk1; // WHY UPDATE IT TWICE?

            res = BMeas - Rk1_I2B*BRef; //  how far off the predicted value of BMeas is
            // cout << "res" << endl;
            // cout << res << endl;
            // cout << "K" << endl;
            // cout << K << endl;
            // cout << BMeas << endl;


            delX = delX + K*(res-H*delX);
        }
    }
    // End

    Vector3f del_qk1_vec;
    del_qk1_vec(0) = delX(0);
    del_qk1_vec(1) = delX(1);
    del_qk1_vec(2) = delX(2);

    float del_qk1_ns = del_qk1_vec.squaredNorm();
    Quaternionf del_qk1;
    if (del_qk1_ns > 1) {
        del_qk1.coeffs() << del_qk1_vec, 1;
        del_qk1.coeffs() *= 1/(sqrt(1 + del_qk1_ns));
    } else {
        del_qk1.coeffs() << del_qk1_vec, sqrt(1 - del_qk1_ns);
    }
    Quaternionf qk1_I2B_Corrected = del_qk1 * qk1_I2B;

    biask1(0) += delX(3);
    biask1(1) += delX(4);
    biask1(2) += delX(5);

    Vector3f wk1_B2I_B = wk_B2I_B - biask1;
    // cout << "omega" << endl;
    // cout << wk_B2I_B << endl;

    ReturnKalman returnVars;
    returnVars.wk1_B2I_B = wk1_B2I_B;
    returnVars.qk1_I2B = qk1_I2B_Corrected;
    returnVars.Pk1 = Pk1;
    returnVars.biask1 = biask1;

    return returnVars;
}

Matrix3f skew(Vector3f vec) {
    Matrix3f mat;
    mat << 0,     -vec(2), vec(1),
           vec(2), 0,     -vec(0),
          -vec(1), vec(0), 0;
    return mat;
}

MatrixXf construct6by6(Matrix3f mat_11, Matrix3f mat_12, Matrix3f mat_21, Matrix3f mat_22) {
    MatrixXf sixBySix(6,6);
    Matrix3f allMats[4] = {mat_11, mat_12, mat_21, mat_22};
    short indexShift[8] = {0,0, 0,3, 3,0, 3,3};
    for (short i = 0; i < 4; i++) {
        for (short j = 0; j < 3; j++) { // row select
            for (short k = 0; k < 3; k++) { // col select
                sixBySix(k + indexShift[2*i], j + indexShift[2*i+1]) = allMats[i](k,j);
            }
        }
    }
    return sixBySix;
}

int factorial(int x) {
    int ret = x;
    for (short i = x-1; i > 0; i--) {
        ret *= i;
    }
    return ret;
}