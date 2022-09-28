#include <kalmanFilterMultiplicative.h>

ReturnKalman multiplicativeFilter(Vector3d wk_B2I_B, 
    Quaterniond qk_I2B,
    Vector3d biask, 
    Vector3d SMeas, 
    Vector3d SRef,
    bool eclp,
    Vector3d BMeas, 
    Vector3d BRef,
    float dt,
    float sig_r,
    float sig_w,
    Matrix3d R_ss,
    Matrix3d R_m,
    MatrixXd Pk) {

    // CHECK THAT BIASK DOESN'T DIVERGE
    // Function Start
    Vector3d biask1 = biask;

    Matrix3d Skew_w = skew(wk_B2I_B);

    Matrix4d Omega;
    Omega << 0,              wk_B2I_B(2), -wk_B2I_B(1), wk_B2I_B(0),
            -wk_B2I_B(2),  0,              wk_B2I_B(0), wk_B2I_B(1),
             wk_B2I_B(1), -wk_B2I_B(0),  0,             wk_B2I_B(2),
            -wk_B2I_B(0), -wk_B2I_B(1), -wk_B2I_B(2), 0            ;

    Quaterniond qk1_I2B;
    qk1_I2B.coeffs() = (MatrixXd::Identity(4,4) + (dt/2) * Omega) * qk_I2B.coeffs();
    
    Matrix3d Phi_11 = Matrix3d::Identity() - dt*Skew_w + (pow(dt,2)/2)*(Skew_w*Skew_w);
    Matrix3d Phi_12 = -1*Matrix3d::Identity()*dt + (pow(dt,2)/2)*Skew_w - (pow(dt,3)/6)*(Skew_w*Skew_w);

    MatrixXd Phi = construct6by6(Phi_11, Phi_12, Matrix3d::Zero(), Matrix3d::Identity());

    MatrixXd Gk = construct6by6(-1*Matrix3d::Identity(), Matrix3d::Zero(), Matrix3d::Zero(), Matrix3d::Identity());

    Matrix3d Qk_11 = pow(sig_r, 2)*dt*Matrix3d::Identity() + pow(sig_w,2)*(Matrix3d::Identity()*(pow(dt,3)/3) + (2*pow(dt,5)/factorial(5))*(Skew_w*Skew_w));
    Matrix3d Qk_12 = -1*pow(sig_w,2)*(Matrix3d::Identity()*(pow(dt,2)/2) - (pow(dt,3)/factorial(3))*Skew_w + (pow(dt,4)/factorial(4))*(Skew_w*Skew_w));
    Matrix3d Qk_22 = (pow(sig_w,2))*dt*Matrix3d::Identity();
    MatrixXd Qk = construct6by6(Qk_11, Qk_12, Qk_12.transpose(), Qk_22);

    MatrixXd Pk1 = Phi*Pk*Phi.transpose() + Gk*Qk*Gk.transpose();

    Matrix3d Rk1_I2B = qk1_I2B.toRotationMatrix();

    // Sun measurement vector
    VectorXd delX = VectorXd::Zero(6);
    for (short i = 0; i < 2; i++) {
        MatrixXd H(3,6);
        Matrix3d den;
        MatrixXd K;
        Vector3d res;
        if(eclp == false && i == 0) {
            Matrix3d Att2ax = Rk1_I2B.transpose();
            Att2ax.col(2) = MatrixXd::Zero(3,1);
            Att2ax.transposeInPlace();

            H << skew(Rk1_I2B*SRef), Matrix3d::Zero();
            
            den = H*Pk1*H.transpose() + R_ss;
            K = (Pk1*H.transpose())*den.inverse();

            Pk1 = (MatrixXd::Identity(6,6) - K*H)*Pk1;

            res = SMeas - Att2ax*SRef;

            delX = delX + K*(res-H*delX);
        } else if (i == 1) {

            // Mag measurement vector
            // No if statement
            H << skew(Rk1_I2B*BRef), Matrix3d::Zero();

            den = (H*Pk1*H.transpose() + R_m); // Pk1 is getting huge

            K = (Pk1*H.transpose())*den.inverse(); 

            Pk1 = (MatrixXd::Identity(6,6) - K*H)*Pk1;

            res = BMeas - Rk1_I2B*BRef; //  how far off the predicted value of BMeas is
            // cout << "delX" << endl;
            // cout << delX << endl;
            // cout << "K*(res-H*delX" << endl;
            // cout << K*(res-H*delX) << endl;


            delX = delX + K*(res-H*delX);
        }
    }
    // End

    Vector3d del_qk1_vec;
    del_qk1_vec(0) = .5*delX(0);
    del_qk1_vec(1) = .5*delX(1);
    del_qk1_vec(2) = .5*delX(2);

    double del_qk1_ns = del_qk1_vec.squaredNorm();
    Quaterniond del_qk1;
    if (del_qk1_ns > 1) {
        del_qk1.coeffs() << del_qk1_vec, 1;
        del_qk1.coeffs() *= 1/(sqrt(1 + del_qk1_ns));
    } else {
        del_qk1.coeffs() << del_qk1_vec, sqrt(1 - del_qk1_ns);
        // cout << "inloop" << endl;
        // cout << del_qk1 << endl;
    }
    Quaterniond qk1_I2B_Corrected =  del_qk1*qk1_I2B; 
    // cout << del_qk1 << endl;
    // cout << qk1_I2B << endl;

    biask1(0) += delX(3);
    biask1(1) += delX(4);
    biask1(2) += delX(5);

    Vector3d wk1_B2I_B = wk_B2I_B - biask1;
    // cout << "omega" << endl;
    // cout << wk_B2I_B << endl;

    ReturnKalman returnVars;
    returnVars.wk1_B2I_B = wk1_B2I_B;
    returnVars.qk1_I2B = qk1_I2B_Corrected;
    returnVars.Pk1 = Pk1;
    returnVars.biask1 = biask1;

    return returnVars;
}

Matrix3d skew(Vector3d vec) {
    Matrix3d mat;
    mat << 0,     -vec(2), vec(1),
           vec(2), 0,     -vec(0),
          -vec(1), vec(0), 0;
    return mat;
}

MatrixXd construct6by6(Matrix3d mat_11, Matrix3d mat_12, Matrix3d mat_21, Matrix3d mat_22) {
    MatrixXd sixBySix(6,6);
    Matrix3d allMats[4] = {mat_11, mat_12, mat_21, mat_22};
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