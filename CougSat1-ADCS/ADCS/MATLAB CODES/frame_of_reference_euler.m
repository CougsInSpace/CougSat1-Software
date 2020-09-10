% bdot algorithm cosidering polar orbit as a simulation, changes the frame of
% reference and adds the rotating matrix functions. 
% all units are in MKS unless otherwise specified

clear all;
clc;
% also add the constraints related to the power

%% Specify time
dt = 0.32; % time step between data points
T = 90*60; % [s] orbital period of ISS [8]

t=0:dt:T; % total time measurements
%% Initializations
inertia = [0.00602377522,0.00000294634,0.00000423169;0.00000294634,0.00130455071,0.00001311622;0.00000294634,0.00001311622,0.00601351576]';
% inertia [kg*m^2] is [Ixx,Ixy,Ixz; Iyx,Iyy,Iyz; Izx,Izy,Izz]

Ls = 400e3; % [m] satellite orbital altitude
alpha = zeros(length(t),3); % [rad/s^2] angular acceleration matrix
omega = zeros(length(t),3); % [rad/s] angular velocity matrix
theta = zeros(length(t),3); % [rad] satellite angle matrix
omega_i = [0.1;-1.5;0.3]'; % [rad/s] instantaneous angular velocity of satellite
Re = 6371.2e3; % [m] Earth volumetric mean radius
 Rs = Re+Ls; % [m] satellite distance from Earth
 p = (111e3*Rs)/Re;
 G = 6.67428e-11; % Earth gravitational constant
M = 5.9723e24; % [kg] Earth mass
v_sat = sqrt(G*M/Rs); % [m/s] linear velocity of the satellite

alpha_i = [0,0,0]; % [rad/s^2] instantaneous angular acceleration of satellite

%% Main loop
 for i = 1:length(t)
    omega_prev = omega_i; % [rad/s] previous angular velocity
    lati = (v_sat*i*dt)/p; % represents the polar orbit as its latitude changes differently when its in space.
    lat = 25+lati; % setting up the latitude for the law, latitude changes due to the change in the y coordinate of the system.
    long = 60+((7.29e-5*i*dt)*180/3.14159); % longitude changes as Earth is rotating
    % cannot find calculations that latitude and longitude were based off
    % latitude and longitude to be obtained from on-board GPS
    
    % Gives error on 90
     if lat > 90
        b = mod(lat,90);
        lat = 90-b;
    end
    if lat < -90
        c = mod(lat,-90);
        lat = -90-c; 
    end
     if long > 180
        d = mod(long,180);
        long = d-180;

     end
    if long < -180
        l = mod(long,-180);
        long = 180+l;
    end
    if lat == 90
        lat = 89.97;
    end
    if lat == -90
        lat = -89.97;
    end
   
    [b_NRNT,horIntensity,declination,inclination,totalIntensity] = igrfmagm(Ls,lat,long,decyear(2019,10,15),12); 
    % magFieldVector_NRNT [T] is initially gathered magnetic field vector with no rotation & no translation. 
    % Z is the vertical component (+ve down) for the magnetic field vector
    % horIntensity [nT] is horizontal intensity
    % declination is in degrees (+ve east)
    % inclination is in degrees (+ve down)
    % totalIntensity in [nT]
    % THIS FUNCTIONS MODEL IS VALID BETWEEN THE YEARS OF 1900 AND 2020 -- NEEDS TO BE UPDATED UPON RELEASE OF NEW MODEL
    b_NR = (b_NRNT*1e-9).'; % [nT] translated magnetic field vector
   
    if i == 1 % initialize rotation values
        theta_x = 0; % [rad]
        theta_y = 0; % [rad]
        theta_z = 0; % [rad]
    else % update previous rotation position to current
        theta_x = theta_x+(omega_i(1)*dt)+(0.5*alpha_i(1)*dt^2); % [rad]
        theta_y = theta_y+(omega_i(2)*dt)+(0.5*alpha_i(2)*dt^2); % [rad]
        theta_z = theta_z+(omega_i(3)*dt)+(0.5*alpha_i(3)*dt^2); % [rad]
    end    
    
    % direction cosine matrix (DCM) to translate obtained magnetic field vector to the satellite (body) frame of reference
    rx = [1,0,0;0,cos(theta_x),-sin(theta_x);0,sin(theta_x),cos(theta_x)];
    ry = [cos(theta_y),0,sin(theta_y);0,1,0;-sin(theta_y),0,cos(theta_y)];
    rz = [cos(theta_z),-sin(theta_z),0;sin(theta_z),cos(theta_z),0;0,0,1];
    b = rx*ry*rz*b_NR; % [nT] rotated & translated magnetic field vector
    
    %% B-dot
    k = 1; % [Js/T^2] control gain
    bdet = 1/(sqrt(dot(b,b))); % magnetic field vector determinant
    bdot = cross(b,omega_i); % [T/s] magnetic field vector derivative
    mu = -(k/bdet)*bdot; % [A*m^2] instantaneous magnetic dipole
    atorque = cross(mu,b); % [Nm] instantaneous atmospheric torque acting on the satellite
    inertia_inv = inv(inertia); % [kg*m^2] inverse inertia matrix
    
    alpha_i = atorque*inertia_inv; % [rad/s^2] calculates new angular acceleration  
    omega_i = omega_prev+(alpha_i*dt); % [rad/s] calculates new angular velocity

    disp('New angular velocity [omega_x, omega_y, omega_z]:');
    disp (omega_i);
    
    % update angle, angular velocity, and angular acceleration matrices
    omega(i,:) = omega_i;
    alpha(i,:) = alpha_i;
    theta(i,1) = theta_x;
    theta(i,2) = theta_y;
    theta(i,3) = theta_z;
     
 end

%% Angular Acceleration Plots
figure(1)
subplot(3,1,1)
plot(t/60,alpha(:,1));
title('CougSat-1 Angular Acceleration in x-axis Post-ejection')
xlabel('Time [min] where 90 is 1 Orbit')
ylabel('\alpha_x [rad/s^2]')
subplot(3,1,2)
plot(t/60,alpha(:,2));
title('CougSat-1 Angular Acceleration in y-axis Post-ejection')
xlabel('Time [min] where 90 is 1 Orbit')
ylabel('\alpha_y [rad/s^2]')
subplot(3,1,3)
plot(t/60,alpha(:,3));
title('CougSat-1 Angular Acceleration in z-axis Post-ejection')
xlabel('Time [min] where 90 is 1 Orbit')
ylabel('\alpha_z [rad/s^2]')
hold on

figure(2)
subplot(3,1,1)
plot(t/60,omega(:,1));
title('CougSat-1 Angular Velocity in x-axis Post-ejection')
xlabel('Time [min] where 90 is 1 Orbit')
ylabel('\omega_x [rad/s]')
subplot(3,1,2)
plot(t/60,omega(:,2));
title('CougSat-1 Angular Velocity in y-axis Post-ejection')
xlabel('Time [min] where 90 is 1 Orbit')
ylabel('\omega_y [rad/s]')
subplot(3,1,3)
plot(t/60,omega(:,3));
title('CougSat-1 Angular Velocity in z-axis Post-ejection')
xlabel('Time [min] where 90 is 1 Orbit')
ylabel('\omega_z [rad/s]')
hold on

figure(3)
subplot(3,1,1)
plot(t/60,theta(:,1));
title('CougSat-1 Angle in x-axis Post-ejection')
xlabel('Time [min] where 90 is 1 Orbit')
ylabel('\theta_x [rad]')
subplot(3,1,2)
plot(t/60,theta(:,2));
title('CougSat-1 Angle in y-axis Post-ejection')
xlabel('Time [min] where 90 is 1 Orbit')
ylabel('\theta_y [rad]')
subplot(3,1,3)
plot(t/60,theta(:,3));
title('CougSat-1 Angle in z-axis Post-ejection')
xlabel('Time [min] where 90 is 1 Orbit')
ylabel('\theta_z [rad]')

% plot the latitude and longitude on the world map