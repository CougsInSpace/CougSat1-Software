%==========================================================================
%==========================================================================
% Author: Erick Sturm
% Date: July 2004
% Program: Polysat Orbit Propagator & Attitude Simulator
%==========================================================================
% Description:
% This program was initially created to take the
% NASA TLEs of a cubesat and use them to determine
% the lat and long of the cubesat at another date.
%==========================================================================
% Revised: Dan Guerrant
% Date: September 2004-June 05
% This version is OPAS with my bug fixes and:
% 0. Minus Solar Power and Current calc. (OPASquick)
% 1. 3-axis stabilization (zeros rates) (OPAS3axis1)
% 2. 3-axis control (zeros quaternions/aligns body,inertial frames)
% (OPAS3axis2)
% 3. Allows user to input quaternions in inertial frame to point to.
% (OPAS3axis3)
% 4. Adds ability to point to quaternions in orbital frame. (OPAS3axis4)
% 5. Bug fixes quaternion error. Attitude sim in previous versions
% incorrect. Still having trouble w/ control in orbital frame. Debug
% version where desired torques inputed directly into attitude sim.
% (OPAS3axis5, April 05)
% 6. More acurately models sensors. Omega I>O transform finally
% working. Includes GG disturbance. (OPAS3axis6, June 05)
% 7. Creates different plots based on control law in use. Fixed error
% in current limiter. (OPAS3axisFinal, June 05)
%==========================================================================
function OPAS
close all;warning off MATLAB:singularMatrix;%clc
start = clock; %Store starting time for run time calculation
%==========================================================================
%Scan in the Data from the NASA TLE
%==========================================================================
%Specs for our supposed orbit: rp=660 ra=760 i=98 raan=22:30
fid = fopen('CP2TLE.txt','rb');
L0 = fgetl(fid);
L1 = fscanf(fid,'%d%6d%*c%5d%*3c%2d%f%f%5d%*c%*d%5d%*c%*d%d%5d',[1,10]); 

L2 = fscanf(fid,'%d%6d%f%f%f%f%f%f',[1,8]);
fclose(fid);
eY0 = L1(1,4); %Epoch Year
eD0 = L1(1,5); %Epoch Day
nD0 = L1(1,6)*4*pi/(24*3600)^2; %First Derivative of Mean Motion (rad/sec2)
nD20 = L1(1,7)*12*pi/(24*3600)^3; %Second Derivative of Mean Motion (rad/sec3)
D0 = L1(1,8); %Drag Term / Radiation Pressure Term
i0 = L2(1,3)*pi/180; %Inclination (rads)
Om0 = L2(1,4)*pi/180; %Right Ascension of the Ascending Node (rads)
e0 = L2(1,5)/1e7; %Eccentricity
om0 = L2(1,6)*pi/180; %Argument of Perigee (rads)
M0 = L2(1,7)*pi/180; %Mean Anomaly (rads)
n0 = L2(1,8)*2*pi/24/3600; %Mean Motion (rad/sec)
%==========================================================================
%Constants
%==========================================================================
Re = 6378.13649; %Equatorial Radius of the Earth (km)
mu = 3.986004415e5; %Gravitational Constant of the Earth (km3/s2)
we = 7.2921158553e-5; %Angular Velocity of the Earth (rad/s)
oe = 0.40909262967; %Obliquity of the Eccliptic (rad)
ne = 1.9909887984e-7; %Mean Motion of the Earth (rad/s)
Pmax = 1; %Maximum Solar Panel Power (W)
Ps = 1367; %Solar Radiation Power (W/m2)
t0o = 0; %Simulation Start Time
J2 = 0.00108263; %J2 Gravitational Perturbation Constant
Lv = 2*pi/180; %Elevation Angle required to see the S/C (rad)
%==========================================================================
%Define the Final Time
%==========================================================================
eYd = eY0+0; %Epoch Year Desired
eDd = eD0+0; %Epoch Day Desired
eHd = 01; %Epoch Hour Desired
eMd = 40; %Epoch Minute Desired
eSd = 00; %Epoch Second Desired
etf = eSd+60*(eMd+60*(eHd+24*(eDd+365.25*eYd))); %Epoch Time Final
eti = 60*(60*(24*(eD0+365.25*eY0))); %Epoch Time Initial
tfo = etf-eti; %Simulation Stop Time
% tspano=[t0o,tfo]; %Simulation Time Span fed into ode45
%==========================================================================
%Calculation of Time from Vernal Equinox
%==========================================================================
Yve = 04; %Epoch Year of Vernal Equinox
Dve = 79; %Epoch Day of Vernal Equinox
Hve = 06; %Epoch Hour of Vernal Equinox
Mve = 47; %Epoch Minute of Vernal Equinox
Sve = 00; %Epoch Second of Vernal Equinox
tve = Sve+60*(Mve+60*(Hve+24*(Dve+365.25*Yve))); %Epoch Time of Vernal Equinox
veo = 0;%-5.39; %Vernal Equinox Offset due to Equinox not happening @ 0:00 GMT
dti = eti-tve; %Initial Change in Time from Vernal Equinox
dtf = etf-tve; %Final Change in Time from Vernal Equinox 

%==========================================================================
%Initial Condition Calculation
%==========================================================================
TLE = [i0;Om0;e0;om0;M0;n0]; %TLEs Loaded above
Keps = tle2keps(TLE,mu); %Convert TLEs to Keplerian Elements
a0 = Keps(1); %Semi-Major Axis (km)
e0 = Keps(2); %Eccentricty
nu0 = Keps(3); %True Anomaly (radians)
i0 = Keps(4); %Inclination (radians)
Om0 = Keps(5); %RAAN (radians)
om0 = Keps(6); %Arg of Perigee (radians)
P0 = a0*(1-e0^2); %Semi-Lattice Rectum (km)
r0 = P0/(1+e0*cos(nu0)); %Radius (km)
v0 = sqrt(mu*(2/r0-1/a0)); %Velocity (km/sec)
y0 = (nu0/abs(nu0))*acos((sqrt(mu*P0)/(r0*v0))); %Gamma (rad)
rD0 = v0*sin(y0); %Radial Velocity (km/sec)
nuD0 = v0*cos(y0)/r0; %Angular Velocity (rad/sec)
state0 = [n0;nD0;a0;e0;i0;Om0;om0;nu0;nuD0;r0;rD0]; %Initial State
%==========================================================================
%Orbit Propagation
%==========================================================================
tfo = .3*pi*sqrt(a0^3/mu); %Final Time for Orbit Propagator
options = odeset('RelTol', 1e-4);
[to,xo] = ode45(@oDiffEq,[t0o,tfo],state0,[],mu,J2,nD20,Re);
mm = xo(:,1); %Mean Motion (rad/s)
nD = xo(:,2); %First Derivative of Mean Motion (rad/s2)
a = xo(:,3); %Semi-Major Axis (km)
e = xo(:,4); %Eccentricity
i = xo(:,5); %Inclination (rad)
Om = xo(:,6); %RAAN (rad)
om = xo(:,7); %Arg of Perigee (rad)
nu = xo(:,8); %True Anomaly (rad)
nuD = xo(:,9); %Angular Velocity (rad/s)
r = xo(:,10); %Radius (km)
rD = xo(:,11); %Radial Velocity (km/s)
%==========================================================================
%Frame Transformations
%==========================================================================
xi=xo2xi(r,nu,om,Om,i); % Orbit 2 Inertial
xe=xi2xe(xi,to,dti,we,veo); % Inertial 2 Earth
rll = xe2rll(xe); % Earth 2 Lat, Long, & Radius
R = rll(:,1); %Radius to S/C
lat = rll(:,2); %Latitude of S/C
long = rll(:,3); %Longitude of S/C
%==========================================================================
%Find the Magnetic Field Vector using the IGRF Model
%==========================================================================
BL=rllt2BL(R,lat,long,to,eti,Re); %Mag-Field in Local N,E,Nadir (Teslas)
Be=xm2xe(BL,lat,long); %Mag-Field in Earth-Fixed
Bi=xe2xi(Be,to,dti,we,veo); %Mag-Field in Inertial
%Normalize the Mag-Field for Unit Vector Plot 

for ndx=1:length(Bi)
 uBi(ndx,:)=Bi(ndx,:)/norm(Bi(ndx,:)); %Unit Mag-Field Vector in Inertial
Frame
end
%==========================================================================
%Create a Ground Track
%==========================================================================
%Create a Figure Window
f1=figure(1);
set(f1,'color','w','Renderer','zbuffer')
set(f1,'Name', 'Orbital Views')
set(f1,'units','normalized','position',[0 0 1 1])
% movegui(f1,'onscreen')
%Create the World Map Background and Ground Track Axes
World = imread('worldmap.jpg'); %Read World Map
imagesc([-180 180],[90 -90],World); hold; %Plot World Map
a1=gca;
set(a1,'YDir','normal')
grid on
%Algorithm to plot the Ground Track
[longm,latm]=col2mat(long,lat,i); %Convert Columns to Matrices
[m,n]=size(longm);
for ndx=1:n
 for mdx=2:m
 if longm(mdx,ndx)==0 & longm(mdx-1,ndx)~=0
 for k=1:(mdx-1)
 longp(k)=longm(k,ndx);
 latp(k)=latm(k,ndx);
 end
 plot(longp,latp,'Linewidth',2,'Color','r')
 clear longp;
 clear latp;
 elseif longm(m,ndx)~=0
 plot(longm(:,ndx),latm(:,ndx),'Linewidth',2,'Color','r')
 end
 end
end
%Find the Ground Area that can see the S/C
gai=r2gai(xi,Re,Lv);
%Algorithm to plot the Ground Area that can see the S/C
[galo,gala]=gai2gall(gai,to,dti,we,i,veo); %Convert above to Lat & Long
[m,n]=size(galo);
if n>1
 for ndx=1:n
 for mdx=2:m
 if galo(mdx,ndx)==0 & galo(mdx-1,ndx)~=0
 for k=1:(mdx-1)
 galop(k)=galo(k,ndx);
 galap(k)=gala(k,ndx);
 end
 plot(galop,galap,'Linewidth',1.25,'Color','g')
 clear galop;
 clear galap;
 elseif galo(m,ndx)~=0
 plot(galo(:,ndx),gala(:,ndx),'Linewidth',1.25,'Color','g')
 end
 end
 end 

else
 plot(galo,gala,'linewidth',1.25,'Color','g');
end
%Plotting Cal Poly, SLO, CA
plot(-120.66512,35.30243,'rx')
%Defining Ground Track Axes Properties
title('Ground Track of the S/C')
axis([-180 180 -90 90])
set(a1,'xtick',[-180 -165 -150 -135 -120 -105 -90 -75 -60 -45 ...
 -30 -15 0 15 30 45 60 75 90 105 120 135 150 165 180])
set(a1,'ytick',[-90 -75 -60 -45 -30 -15 0 15 30 45 60 75 90])
set(a1,'units','normalized','position',[.3 .2 .6 .4])
set(a1,'xcolor','k','ycolor','k')
set(get(a1,'Title'),'FontWeight','bold','Color','k')
%==========================================================================
%Attitude Simulator
%==========================================================================
%S/C Properties------------------------------------------------------------
m = 1.00; %Mass (kg)
l = 0.10; %Length (m)
w = 0.09; %Width (m)
h = 0.11; %Height (m)
% I = 1/12*m*[(h^2+w^2);(l^2+h^2);(w^2+l^2)]; %Mass Moments of Inertia (kgm^2)
I = [8.817e-4; 9.804e-4; 10.35e-4]; %MMI from IDEAS model of CP2
K = [(I(2)-I(3))/I(1);
 (I(3)-I(1))/I(2);
 (I(1)-I(2))/I(3)];
%Simulation Setup----------------------------------------------------------
t0a = t0o; %Seconds to offset AtSim Start from OrProp Start
tfa = tfo; %Seconds to run AtSim
tr = 9.9; %Seconds taking readings
dtrc = 0.1; %Seconds between reading end and control start
tc = tr; %Seconds to have Torquers on [0.025,6.4]
dtcr = dtrc; %Seconds between control end and reading start
%Initial Conditions--------------------------------------------------------
% w0 = [0.005 -0.005 0.002]; %Conservative after BDot case (rad/s)
w0 = [1*pi/30 .05 -.05]; %Worst likely deployment case
q0 = [0 0 0 1]; %Initial Quaternion - Inertial->Body
state0 = [w0 q0]; %Initial State
%Desired State-------------------------------------------------------------
frame = 0; %0 if desidred given in inertial, 1 for orbital
switch 5 %Desired Quaternions (frame defined above)
 case 1, qd = [1 0 0 0]; %180 about b1
 case 2, qd = [0 1 0 0]; %180 about b2
 case 3, qd = [0 0 1 0]; %180 about b3
 case 4, qd = [0 0 0 1]; %Alligned
 case 5, qd = [0.5 0.5 0.5 0.5]; %120 about [1 1 1]
 case 6, qd = [0 0 sqrt(2)/2 sqrt(2)/2]; %90 about b3
 case 7, qd = [0 sqrt(2)/2 0 sqrt(2)/2]; %90 about b2
 case 8, qd = [sqrt(2)/2 0 0 sqrt(2)/2]; %90 about b1
end
if frame
 qdo = qd; %Desired Quaternions in orbital frame
 wdo = [0 0 0]; 

else
 qdi = qd; %Desired Quaternions in inertial frame
 wdi = [0 0 0];
end
%Magnetometer Modeling Method----------------------------------------------
test = 5; %Switch selects the test case listed below switch test
 case 1 %Perfect 3-axis. No mag/pos/rate error, direct torquage from control law.
 case 2 %3-axis. No mag/pos/rate error, control torque turned into mag commands.
 case 3 %3-axis CP3 sim. Mag/pos/rate error, control torque to mag commands.
 case 4 %Perfect B-Dot sim. No mag error.
 case 5 %CP3 B-dot sim. Mag error.
 case 6 %CP2 B-dot sim. Mag error, actual mag cal data
 case 7 %CP2 B-dot sim. Mag error, actual mag cal data, b-dot w bits (as implimented)
end
if test >= 6
 %Actual CP2 callibration data from Side Test Data.xls
 CalData = [ % m b
 4.50e-7 -5.93e-5; %Front Magnetometer Axis A (+X) FVII
 4.29e-7 -4.75e-5; %Front Magnetometer Axis B (+Z)
 4.40e-7 -5.45e-5; %Left Magnetometer Axis A (-Y) SXVIII
 4.33e-7 -5.26e-5; %Left Magnetometer Axis B (-Z)
 4.92e-7 -5.68e-5; %Right Magnetometer Axis A (-Y) SXX
 4.61e-7 -5.79e-5; %Right Magnetometer Axis B (+Z)
 4.16e-7 -5.60e-5; %Top Magnetometer Axis A (-Y) SXXI
 3.98e-7 -5.07e-5; %Top Magnetometer Axis B (-X)
 4.38e-7 -5.58e-5; %Bottom Magnetometer Axis A (-Y) SXXII
 4.26e-7 -5.50e-5; %Bottom Magnetometer Axis B (+X)
 ];
else
 % Ideal mag setup: 0=-500mG,128=0, 255=500mG
 % Tesla = m * Bit value + b
 CalData = [5e-5/128*ones(10,1) -5e-5*ones(10,1)];
end
StDvM = 2; %Standard Deviation in Magnetometer Readings (Bits)
StDvG = 5e-5; %Std Dev in gyro readings (rad/s)
StDvQ = 1e-2; %Std Dev in quaternion readings from star tracker
Gres = 1e-4; %Resolution of gyros(rad/s)
Qres = 1e-1; %Resolution of quaternions
Ires = 0.0012; %Resolution of Magnetorquer Current (Amps/Step)
Ilim = 0.3; %Max current per torquer(Amps)
%Controller Parameters-----------------------------------------------------
N = 54; %Number of Torquer Loops
A = .003; %Average Area of Torquer Loops (m^2)
NA = 0.150622; %Sum of area enclosed by each loop (~N*A)
C = 8e-3; %Kyle's B-Dot Constant on CP2 (~Kk*CalData(:,1))
Kk = 20e3; %C*128/5e-5; %B-Dot Controller Gain for CP3
(~C/CalData(:,1))
C1 = 20e-3; %Rate gain
C2 = 10e-6; %Quaternion gain
%Sim w/ Torquing-----------------------------------------------------------
%Loop Initialization
 tas = t0a; %attitude simulation time
 wbri = w0; %Initial Omegas in the Body Frame during Readings 
 qri = q0; %Quaternions after torquers
 ndx = 1; mdx = 1; idx = 1;

while tas < tfa
 mub=[0;0;0]; %Magnetic Dipole generated by Torquers
 Treq=[0;0;0];

 %Attitude Sim of S/C During Readings
 [tar,stater]=ode45(@aDiffEq,[tas,tas+tr,tas+tr+dtrc],[wbri';qri'],...
 options,I,K,mub,to,Bi,Treq,test,mm);
 wbr=stater(:,1:3); %Omegas in the Body Frame during Readings
 qr =stater(:,4:7); %Quaternions during Readings
 wbci=wbr(end,:); %Initial Omegas in the Body Frame during
Torquing
 qci =qr(end,:); %Initial Quaternions during Torquing

 % Mag-field and Attitude Measurments
 Bb=Bi2Bb(to,Bi,tar,qr); %Mag-Field in Body Frame during Readings
(Tesla)
 if (test==1 | test==2 | test==4)
 Bbm = Bb; %Measured Mag-Field (no error)
 wbrn = wbr(2,:); %Measured angular rates (no error)
 qrn = qr(2,:); %Measured quaternions (no error)
 else
 [BbmBit,Bbm] = magErr(Bb,CalData,StDvM); %Measured
Mag-Field (error introduced)
 wbrn = roundto(wbr(2,:)+StDvG.*randn(size(wbr(2,:))),Gres); %Measured
angular rates (error introduced)
 qrn = roundto(qr(2,:)+StDvQ.*randn(size(qr(2,:))),Qres); %Measured
quaternions (error introduced)
 end

 %Algorithm to Allow Plotting of Measured Mag-Field
 taBm(idx:idx+2)=tar;
 Bm(idx:idx+2,1:3)=Bbm;B(idx:idx+2,1:3)=Bb;
 idx=idx+3;

 %Inertial to Orbital State Vector Transformation
 mm1 = interp1(to,mm,tar(2)); %Mean Motion(rad/s)
 theta = interp1(to,nu,tar(2)); %True Anomoly(rad)
 qoi = [0 0 sin(theta/2) cos(theta/2)]; %Orbital>Inertial rotation
 qio = [0 0 sin(-theta/2) cos(-theta/2)]; %Inertial>Orbital rotation
 qorb(mdx,:) = qtrans(qio,qr(2,:)); %Actual state in orbital frame
 worb(mdx,:) = wi2wo(wbr(2,:),qr(2,:),mm1); %Actual rates in orbital frame

 %%%%%%%%%%%%%%%%% Controllers %%%%%%%%%%%%%%%%%
 %%%%%%%%%%%%%% 3-axis controller %%%%%%%%%%%%%%
 %Find the error
 if frame %Converts desired vector from orbital to
inertial
 qdi = qtrans(qoi,qdo); %Desired quaternions in inertial frame
 wdi = wo2wi(wdo,qdo,mm1); %Desired rates in inertial frame
 else %Converts desired vector from inertial to
orbital
 qdo = qtrans(qio,qdi); %Desired quaternions in orbital frame
 wdo = wi2wo(wdi,qdi,mm1); %Desired rates in orbital frame
 end
 qea(mdx,:) = qerr(qdi,qr(2,:)); %Calculate actual quaternion error
 qe = qerr(qdi,qrn); %Quaternion error used by algorithm
 we = wbrn - wdi; %Calculate rate error

 %Control Law
 Treq = -diag(I)*(C1.*we+C2.*qe(1:3))'; %Torque vector
requested 

 mub3a = (cross(Bbm(2,:)',Treq)./(norm(Bbm(2,:)))^2)'; %Magnetic dipole
vector requested

 %%%%%%%%%%%%%% B-Dot Controller %%%%%%%%%%%%%%%
 if test==6 %BDot using mag bit values (as implimented on CP2)
 BDot = (BbmBit(2,:)-BbmBit(1,:))/(tar(2)-tar(1));
 mubBd = -C*BDot; %Magnetic dipole vector requested
 else %BDot using mag bit values converted back to Tesla
 BDot = (Bbm(2,:)-Bbm(1,:))/(tar(2)-tar(1));
 mubBd = -Kk*BDot; %Magnetic dipole vector requested
 end

 %%%%%%%%% Select active control law %%%%%%%%%%%
 if test<=3
 mub = mub3a; %3-axis is active
 mub1 = mubBd;
 else
 mub = mubBd; %B-dot is active
 mub1 = mub3a;
 end

 %%%%%%%%%%%%%%% Current Limiter %%%%%%%%%%%%%%%%
 Ic = roundto(mub/(NA),Ires); %Current requested for the Torquers
(Amps)
 while abs(Ic(1))>Ilim | abs(Ic(2))>Ilim | abs(Ic(3))>Ilim
 Ic = roundto(Ic./2,Ires); %Current provided to torquers
% disp('clip')
 end
 mub = Ic*NA;
 %%%%%%%%%% End controller algorithms %%%%%%%%%%%

 %Algorithm to Allow Plotting of B-Dot, Mag Dipole, and Required Current
 taBD(mdx,1) = tar(2); BD(mdx,:) = BDot;
 mdp(mdx,:) = mub; mdp1(mdx,:) = mub1;
 qdip(mdx,:) = qdi; qdop(mdx,:) = qdo;
 wdip(mdx,:) = wdi; wdop(mdx,:) = wdo;
 wnp(mdx,:) = wbrn; qnp(mdx,:) = qrn;
 qep(mdx,:) = qe; Ir(mdx,1:3) = Ic;
 mdx=mdx+1;
 %--Convergence Tests--
 if test<=3 % Three-axis deadband shutoff
 if norm(we)<1e-3 & 2*180/pi*acos(qe(4))<4
% disp('Deadband')
 mub = [0;0;0];
 end
 else
 if mdx>20 % B-dot shutoff
 normBD = mean([norm(BD(end,:)),norm(BD(end-1,:)),...
 norm(BD(end-2,:)),norm(BD(end-3,:)),norm(BD(end-4,:))]);
 if normBD<4e-8
 disp('Time to Converge (min)');disp(tas/60)
 disp('Norm BDot (T/s)');disp(norm(BDot))
 break
 end
 end
 end
% %Inactive torquer tests
% mub(1) = 0;
% mub(2) = 0;

 %Attitude Sim of S/C during Torquing
 [tac,statec]=ode45(@aDiffEq,[tas+tr+dtrc,tas+tr+dtrc+tc],[wbci';qci'],... 

 options,I,K,mub,to,Bi,Treq,test,mm);
 wbc=statec(:,1:3); %Omegas in the Body Frame during Torquing
 qc =statec(:,4:7); %Quaternions during Torquing
 wbri=wbc(length(tac),:); %Initial Omegas in the Body Frame during Readings
 qri =qc(length(tac),:); %Initial Quaternions during Readings
 tas=tac(length(tac)); %Increase Attitude Simulation Loop Time

 %Algorithm to Create Plottable Vectors of Time, Omegas, and Quaternions
 tan=[tar(1:length(tar)-1);tac(1:length(tac)-1)];
 ta(ndx:ndx+length(tan)-1)=tan;
 wbn=[wbr(1:length(tar)-1,1:3);wbc(1:length(tac)-1,1:3)];
 wb(ndx:ndx+length(tan)-1,1:3)=wbn;
 qn=[qr(1:length(tar)-1,1:4);qc(1:length(tac)-1,1:4)];
 qi(ndx:ndx+length(tan)-1,1:4)=qn;
 ndx=ndx+length(tan);
end
%Plotting the Results of the Simulation------------------------------------
f2=figure(2);
set(f2,'units','normalized','position',[0 0.18 1
0.75],'Color','w','Name','Detumbling Algorithm');
%%%%% Creates different plot layouts for 3-axis and B-dot results
if test <= 3
 a1=axes('parent',f2,'units','normalized','position',[.03 .55 .3
.4],'nextplot','add');
 plot(taBD./60,qep,'Color',[.6 .6 .6]);plot(taBD./60,qea);
 title('a) Quaternion
Error','FontWeight','Bold','Color','k');set(gca,'YLim',[-1 1])
 a2=axes('parent',f2,'units','normalized','position',[.37 .55 .3
.4],'nextplot','add');
 plot(taBD./60,wnp,'Color',[.6 .6 .6]);
 plot(ta./60,wb);plot(taBD./60,wdip,':');%set(gca,'YLim',[-3e-3 3e-3])
 title('b) Inertial Angular Rates
(rad/s)','FontWeight','Bold','Color','k');
 a3=axes('parent',f2,'units','normalized','position',[.7 .55 .3
.4],'nextplot','add');
 plot(taBD./60,qnp,'Color',[.6 .6 .6]);
 plot(taBD./60,qdip,':');plot(ta./60,qi);
 title('c) Inertial
Quaternions','FontWeight','Bold','Color','k');set(gca,'YLim',[-1 1])
 a4=axes('parent',f2,'units','normalized','position',[.03 .05 .3
.4],'nextplot','add');
 plot(taBD./60,2.*180./pi.*acos(qep(:,4)),'Color',[.6 .6 .6]);
 plot(taBD./60,2.*180./pi.*acos(qea(:,4)));set(gca,'YLim',[0 60])
 title('d) Pointing error (deg)','FontWeight','Bold','Color','k');
% % plot(taBD,mdp1,':'); %'Color',[.6 .6 .6]);plot(taBD./60,mdp);
% plot(taBD./60,Ir)%;set(gca,'YLim',[-Ilim Ilim])
% title('d) Torquer Current Provided
(A)','FontWeight','Bold','Color','k');
 a5=axes('parent',f2,'units','normalized','position',[.37 .05 .3
.4],'nextplot','add');
 plot(taBD./60,worb);plot(taBD./60,wdop,':');%set(gca,'YLim',[-3e-3 3e-3])
 title('e) Orbital Angular Rates (rad/s)','FontWeight','Bold','Color','k');
 a6=axes('parent',f2,'units','normalized','position',[.7 .05 .3
.4],'nextplot','add');
 plot(taBD./60,qdop,':');plot(taBD./60,qorb);
 title('f) Orbital
Quaternions','FontWeight','Bold','Color','k');set(gca,'YLim',[-1 1])
else
 a1=axes('parent',f2,'units','normalized','position',[.03 .55 .3
.4],'nextplot','add');
 plot(to./60,Bi);
 title('a) Magnetic Field in Orbital Frame
(T)','FontWeight','Bold','Color','k'); 

 a2=axes('parent',f2,'units','normalized','position',[.37 .55 .3
.4],'nextplot','add');
 plot(taBm./60,Bm,'Color',[.6 .6 .6]);plot(taBm./60,B);
 title('b) Magnetic Field in Body Frame
(T)','FontWeight','Bold','Color','k');
 a3=axes('parent',f2,'units','normalized','position',[.7 .55 .3
.4],'nextplot','add');
 plot(taBD./60,BD);
 title('c) B-dot (T/sec)','FontWeight','Bold','Color','k');
 a4=axes('parent',f2,'units','normalized','position',[.03 .05 .3
.4],'nextplot','add');
 plot(taBD./60,Ir);set(gca,'YLim',[-Ilim Ilim])
 title('d) Torquer Current Provided (A)','FontWeight','Bold','Color','k');
 a5=axes('parent',f2,'units','normalized','position',[.37 .05 .3
.4],'nextplot','add');
% plot(ta./60,[wb sqrt(wb(:,1).^2+wb(:,2).^2+wb(:,3).^2)]); %Includes
magnitude
 plot(ta./60,wb);
 set(gca,'YLim',[-.01 .01],'YTick',-.01:0.002:.01);
 title('e) Inertial Angular Rates
(rad/s)','FontWeight','Bold','Color','k');
 a6=axes('parent',f2,'units','normalized','position',[.7 .05 .3
.4],'nextplot','add');
 plot(ta./60,qi);
 title('f) Inertial
Quaternions','FontWeight','Bold','Color','k');set(gca,'YLim',[-1 1])
end
set(a1,'XLim',[0
tas./60],'xgrid','on','ygrid','on','Color','w','xcolor','k','ycolor','k','zcol
or','k');
set(a2,'XLim',[0
tas./60],'xgrid','on','ygrid','on','Color','w','xcolor','k','ycolor','k','zcol
or','k');
set(a3,'XLim',[0
tas./60],'xgrid','on','ygrid','on','Color','w','xcolor','k','ycolor','k','zcol
or','k');
set(a4,'XLim',[0
tas./60],'xgrid','on','ygrid','on','Color','w','xcolor','k','ycolor','k','zcol
or','k');
set(a5,'XLim',[0
tas./60],'xgrid','on','ygrid','on','Color','w','xcolor','k','ycolor','k','zcol
or','k');
set(a6,'XLim',[0
tas./60],'xgrid','on','ygrid','on','Color','w','xcolor','k','ycolor','k','zcol
or','k');
disp('Run Time(s)');disp(etime(clock,start))
i = 0; %Avoid desplaying NaN's from very last cycle
while isnan(wb((end-i),1))
 i = i+1;
end
disp('Final Inertial Quaternians and Rates(deg/s then rad/s)');
disp(qi((end-i),:));disp([wb((end-i),:) norm(wb((end-i),:))].*180./pi);
disp([wb((end-i),:) norm(wb((end-i),:))])
disp('Final Orbital Quaternians and Rates(deg/s then rad/s)');
disp(qorb((end-1),:));disp(worb((end-1),:).*180./pi)
disp('Quaternion Error')
disp(qea(end-1,:))
disp('Pointing Accuracy (deg)')
disp(2*180/pi*acos(qea((end-1),4)))
beep
%========================================================================== 

%Below are functions called in "OPAS" above.
%VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
function [BbmBit,Bbm] = magErr(Bb,CalData,sig)
%Simulate magneometer reading with noise. fmt in bit value
fmtA = round((Bb(:,1)-CalData(1,2))/CalData(1,1)+sig*randn(3,1)); %Front
Magnetometer Axis A (+X)
fmtB = round((Bb(:,3)-CalData(2,2))/CalData(2,1)+sig*randn(3,1)); %Front
Magnetometer Axis B (+Z)
lmtA = round((-Bb(:,2)-CalData(3,2))/CalData(3,1)+sig*randn(3,1)); %Left
Magnetometer Axis A (-Y)
lmtB = round((-Bb(:,3)-CalData(4,2))/CalData(4,1)+sig*randn(3,1)); %Left
Magnetometer Axis B (-Z)
rmtA = round((-Bb(:,2)-CalData(5,2))/CalData(5,1)+sig*randn(3,1)); %Right
Magnetometer Axis A (-Y)
rmtB = round((Bb(:,3)-CalData(6,2))/CalData(6,1)+sig*randn(3,1)); %Right
Magnetometer Axis B (+Z)
tmtA = round((-Bb(:,2)-CalData(7,2))/CalData(7,1)+sig*randn(3,1)); %Top
Magnetometer Axis A (-Y)
tmtB = round((-Bb(:,1)-CalData(8,2))/CalData(8,1)+sig*randn(3,1)); %Top
Magnetometer Axis B (-X)
bmtA = round((-Bb(:,2)-CalData(9,2))/CalData(9,1)+sig*randn(3,1)); %Bottom
Magnetometer Axis A (-Y)
bmtB = round((Bb(:,1)-CalData(10,2))/CalData(10,1)+sig*randn(3,1));%Bottom
Magnetometer Axis B (+X)
xmt = median([fmtA';255-tmtB';bmtB']); %Average of X-Axis
magnetometer Readings
ymt = median([255-lmtA';255-rmtA';255-tmtA';255-bmtA']); %Average of Y-Axis
magnetometer Readings
zmt = median([fmtB';255-lmtB';rmtB']); %Average of Z-Axis
magnetometer Readings
BbmBit = [xmt',ymt',zmt']; %Measured Magnetic Field as
implemented on CP2 (bits)
%Simulate conversion by satellite into Tesla
mt = [fmtA fmtB lmtA lmtB rmtA rmtB tmtA tmtB bmtA bmtB]';
mtCal = [CalData(:,1) CalData(:,1) CalData(:,1)].*mt + [CalData(:,2)
CalData(:,2) CalData(:,2)];
fmtAcal = mtCal(1,:); fmtBcal = mtCal(2,:); lmtAcal = mtCal(3,:); lmtBcal =
mtCal(4,:);
rmtAcal = mtCal(5,:); rmtBcal = mtCal(6,:); tmtAcal = mtCal(7,:);
tmtBcal = mtCal(8,:); bmtAcal = mtCal(9,:); bmtBcal = mtCal(10,:);
xmtCal = median([fmtAcal;-tmtBcal;bmtBcal]); %Average of X-Axis
magnetometer Readings
ymtCal = median([-lmtAcal;-rmtAcal;-tmtAcal;-bmtAcal]); %Average of Y-Axis
magnetometer Readings
zmtCal = median([fmtBcal;-lmtBcal;rmtBcal]); %Average of Z-Axis
magnetometer Readings
Bbm = [xmtCal',ymtCal',zmtCal']; %Measured Magnetic Field in the Body
Frame (Tesla)
%==========================================================================
function qe = qerr(qd,qa)
%Calculates the quaternion error rotation
%qd = desired, qa = actual 

q = [
 qd(4) qd(3) -qd(2) -qd(1);
 -qd(3) qd(4) qd(1) -qd(2);
 qd(2) -qd(1) qd(4) -qd(3);
 qd(1) qd(2) qd(3) qd(4)
];
qe = (q*[qa(1);qa(2);qa(3);qa(4)])';
%==========================================================================
function qt = qtrans(q1,q2)
% Returns sum of two successive quaternion rotations
q = [
 q2(4) q2(3) -q2(2) q2(1);
 -q2(3) q2(4) q2(1) q2(2);
 q2(2) -q2(1) q2(4) q2(3);
 -q2(1) -q2(2) -q2(3) q2(4)
];
qt = (q*[q1(1);q1(2);q1(3);q1(4)])';
%==========================================================================
function wo = wi2wo(w,q,mm)
%Converts angular rates from inertial to orbital
T = [
 1-2*(q(2)^2+q(3)^2) 2*(q(1)*q(2)+q(3)*q(4)) 2*(q(1)*q(3)-
q(2)*q(4));
 2*(q(1)*q(2)-q(3)*q(4)) 1-2*(q(1)^2+q(3)^2)
2*(q(2)*q(3)+q(1)*q(4));
 2*(q(1)*q(3)+q(2)*q(4)) 2*(q(2)*q(3)-q(1)*q(4)) 1-
2*(q(1)^2+q(2)^2)
];
wo = ([w(1);w(2);w(3)]-T*[0;0;mm])';
%==========================================================================
function wi = wo2wi(w,q,mm)
%Converts angular rates from orbital to inertial
T = [
 1-2*(q(2)^2+q(3)^2) 2*(q(1)*q(2)+q(3)*q(4)) 2*(q(1)*q(3)-
q(2)*q(4));
 2*(q(1)*q(2)-q(3)*q(4)) 1-2*(q(1)^2+q(3)^2)
2*(q(2)*q(3)+q(1)*q(4));
 2*(q(1)*q(3)+q(2)*q(4)) 2*(q(2)*q(3)-q(1)*q(4)) 1-
2*(q(1)^2+q(2)^2)
];
wb = ([w(1);w(2);w(3)]+T*[0;0;mm])';
%==========================================================================
function xi=xo2xi(r,nu,om,Om,i)
%Converts from Orbit Frame to Inertial Frame in Orbit Propagator
uo = r.*cos(nu+om);
vo = r.*sin(nu+om);
ui = uo.*cos(Om)-vo.*cos(i).*sin(Om);
vi = uo.*sin(Om)+vo.*cos(i).*cos(Om); 

wi = vo.*sin(i);
xi = [ui,vi,wi];
%==========================================================================
function xe=xi2xe(xi,t,dti,wie,veo)
%Converts from Inertial Frame to Earth Frame
ui = xi(:,1);
vi = xi(:,2);
wi = xi(:,3);
off=veo*3600*wie;
for ndx = 1:length(xi)
 ue(ndx) = cos((wie)*(t(ndx)+dti)+off)*ui(ndx)+sin((wie)*(t(ndx)+dti)+off)*vi(ndx);
 ve(ndx) = -
sin((wie)*(t(ndx)+dti)+off)*ui(ndx)+cos((wie)*(t(ndx)+dti)+off)*vi(ndx);
 we(ndx) = wi(ndx);
end
xe = [ue',ve',we'];
%==========================================================================
function rll=xe2rll(xe)
%Converts from Earth Frame to Radius, Longitude, & Latitude
ue = xe(:,1);
ve = xe(:,2);
we = xe(:,3);
for ndx = 1:length(xe)
 r(ndx) = sqrt((ue(ndx))^2+(ve(ndx))^2+(we(ndx))^2);
 lat(ndx)=asin(we(ndx)/r(ndx))*180/pi;

long(ndx)=(ve(ndx)/abs(ve(ndx)))*acos(ue(ndx)/sqrt((ue(ndx))^2+(ve(ndx))^2))*1
80/pi;
end
rll = [r',lat',long'];
%==========================================================================
function BL=rllt2BL(R,lat,long,t,eti,Re)
%Finds Mag-Field Vector @ given alt, lat, long, time.
Bfyr = (t+eti)/3600/24/365.25+2000;
BR = R-Re;
BLat = 90-lat;
BLong = long;
for ndx=1:length(R)
 BLi(ndx,:) = magfd(Bfyr(ndx),1,BR(ndx),BLat(ndx),BLong(ndx));
end
BL=(1e-9)*BLi; %Mag field (Teslas)
%========================================================================== 

function xe=xm2xe(mfv,lat,long)
%Converts from Mag-Field Frame to Earth Frame
um = mfv(:,1);
vm = mfv(:,2);
wm = mfv(:,3);
xm = [um';vm';wm'];
lt = pi/180*lat;
lg = pi/180*long;
for ndx=1:length(mfv)
 xe(:,ndx)=[-sin(lt(ndx))*cos(lg(ndx)),-sin(lg(ndx)),-
cos(lt(ndx))*cos(lg(ndx));
 -sin(lt(ndx))*sin(lg(ndx)), cos(lg(ndx)),-
cos(lt(ndx))*sin(lg(ndx));
 cos(lt(ndx)) , 0,-sin(lt(ndx))
]*xm(:,ndx);
end
xe=xe';
%==========================================================================
function xi=xe2xi(xe,t,dti,wei,veo)
%Converts from Earth-Fixed Frame to Inertial Frame
xe=xe';
off=veo*3600*wei;
for ndx=1:length(xe)
 xi(:,ndx) = [cos((wei)*(t(ndx)+dti)+off),-sin((wei)*(t(ndx)+dti)+off),0;
 sin((wei)*(t(ndx)+dti)+off), cos((wei)*(t(ndx)+dti)+off),0;
 0, 0,1]*xe(:,ndx);
end
xi=xi';
%==========================================================================
function gai=r2gai(xi,Re,Lv)
%Creates the boundary on Earth in inertial coordinates that can view the S/C
Rv = xi(length(xi),:);
Rm = norm(Rv);
r = Re*cos(Lv)*(cos(asin(Re/Rm*cos(Lv)))-Re/Rm*sin(Lv));
cm = Re*(sin(Lv)*cos(asin(Re/Rm*cos(Lv)))+Re/Rm*(cos(Lv))^2);
cv = cm/Rm*Rv;
ci1 = (-r):10:(+r);
ci2 = ci1;
ci = [flipud(ci1');ci2';ci1(length(ci1))];
cj1 = sqrt(r^2-(ci1).^2);
cj2 = -sqrt(r^2-(ci2).^2);
cj = [flipud(cj1');cj2';cj1(length(cj1))];
ck = zeros(size(ci));
cx = [ci,cj,ck]; 

if Rv(1)>0
 th = atan(Rv(2)/Rv(1))+pi/2;
else
 th = atan(Rv(2)/Rv(1))+3*pi/2;
end
ph = pi-atan(sqrt((Rv(1))^2+(Rv(2))^2)/Rv(3));

cii = (ci*cos(th)-cj*cos(ph)*sin(th)-ck*sin(ph)*sin(th))+cv(1);
cji = (ci*sin(th)+cj*cos(ph)*cos(th)+ck*sin(ph)*sin(th))+cv(2);
cki = ( -cj*sin(ph) +ck*cos(ph))+cv(3);
cxi = [cii,cji,cki];
gai = cxi;
%==========================================================================
function [galo,gala]=gai2gall(gai,t,dti,wie,i,veo)
%Creates Plottable Matrices of the Lat and Long defining
%the Ground Area that can view the S/C from the coordinates
%of that boundary in the inertial frame
ui = gai(:,1);
vi = gai(:,2);
wi = gai(:,3);
f=length(t);
off=veo*3600*wie;
for ndx = 1:length(gai)
 ue(ndx) =
cos((wie)*(t(f)+dti)+off)*ui(ndx)+sin((wie)*(t(f)+dti)+off)*vi(ndx);
 ve(ndx) = -
sin((wie)*(t(f)+dti)+off)*ui(ndx)+cos((wie)*(t(f)+dti)+off)*vi(ndx);
 we(ndx) = wi(ndx);
end
gae = [ue',ve',we'];
gall = xe2rll(gae);
long = gall(:,3);
lat = gall(:,2);
count=0;
for index=1:length(long)
 if (long(index)<-179)|(long(index)>179)
 count=count+1;
 end
end
if count>0
 n = 1;
 m = 0;
 for ndx=1:length(long)
 if i(length(i))>(pi/2)
 if ndx==1|ndx==2
 longm(ndx,n)=long(ndx);
 latm(ndx,n)=lat(ndx);
 else
 if (long(ndx)>long(ndx-1))|((long(ndx)<long(ndx-1))&(long(ndx1)>long(ndx-2)))
 n=n+1;
 m=ndx-1; 

 longm(ndx-m,n)=long(ndx);
 latm(ndx-m,n)=lat(ndx);
 else
 longm(ndx-m,n)=long(ndx);
 latm(ndx-m,n)=lat(ndx);
 end
 end
 elseif i(length(i))<(pi/2)
 if ndx==1|ndx==2
 longm(ndx,n)=long(ndx);
 latm(ndx,n)=lat(ndx);
 else
 if (long(ndx)<long(ndx-1))|((long(ndx)>long(ndx-1))&(long(ndx1)<long(ndx-2)))
 n=n+1;
 m=ndx-1;
 longm(ndx-m,n)=long(ndx);
 latm(ndx-m,n)=lat(ndx);
 else
 longm(ndx-m,n)=long(ndx);
 latm(ndx-m,n)=lat(ndx);
 end
 end
 else
 longm=long;
 latm=lat;
 end
 end
 galo=longm;
 gala=latm;
else
 galo=long;
 gala=lat;
end
%==========================================================================
function ad=roundto(a,b)
% Rounds a number (a) to a resolution (b)
c = rem(a,b);
d = c/b;
[m,n]=size(a);
for mdx=1:m
 for ndx=1:n
 if d(mdx,ndx)>=0.5
 a(mdx,ndx)=a(mdx,ndx)+(b-c(mdx,ndx));
 elseif d(mdx,ndx)<=-0.5
 a(mdx,ndx)=a(mdx,ndx)-(b+c(mdx,ndx));
 else
 a(mdx,ndx)=a(mdx,ndx)-c(mdx,ndx);
 end
 end
end
ad=a;
%==========================================================================
function [longm,latm]=col2mat(long,lat,i)
%Converts from Column to Matrix
% 1 

% 2 1 -3
%Changes -3 to 2 -2
% -2 0 -1
% -1
n = 1;
m = 0;
for ndx=1:length(long)
 if i(ndx)>(pi/2)
 if ndx==1
 longm(ndx,n)=long(ndx);
 latm(ndx,n)=lat(ndx);
 else
 if long(ndx)>long(ndx-1)
 n=n+1;
 m=ndx-1;
 longm(ndx-m,n)=long(ndx);
 latm(ndx-m,n)=lat(ndx);
 else
 longm(ndx-m,n)=long(ndx);
 latm(ndx-m,n)=lat(ndx);
 end
 end
 elseif i(ndx)<(pi/2)
 if ndx==1
 longm(ndx,n)=long(ndx);
 latm(ndx,n)=lat(ndx);
 else
 if long(ndx)<long(ndx-1)
 n=n+1;
 m=ndx-1;
 longm(ndx-m,n)=long(ndx);
 latm(ndx-m,n)=lat(ndx);
 else
 longm(ndx-m,n)=long(ndx);
 latm(ndx-m,n)=lat(ndx);
 end
 end
 else
 longm=long;
 latm=lat;
 end
end
%==========================================================================
function keps=tle2keps(tle,mu)
%Converts NASA TLE to Keplerian Elements
i = tle(1);
Om = tle(2);
e = tle(3);
om = tle(4);
M = tle(5);
n = tle(6);
a = (mu/n^2)^(1/3);
E = Me2E(M,e);
nu = 2*atan(sqrt((1+e)/(1-e))*tan(E/2));
keps = [a; e; nu; i; Om; om]; 

%======================================================================
function E=Me2E(M,e)
%Solves for Eccentric Anomaly given Mean Anomaly and
%Eccentricity using Newton's Iteration Method
Ec = M;
Ei = Ec - (Ec-e*sin(Ec)-M)/(1-e*cos(Ec));
while (abs(Ei-Ec)>1e-7)
 Ec = Ei;
 Ei = Ec - (Ec-e*sin(Ec)-M)/(1-e*cos(Ec));
end;
E = Ei;
%==========================================================================
function stateD=oDiffEq(t,state,mu,J2,nD20,Re)
%The Orbital Differential Equations
n = state(1);%Mean Motion
nD = state(2);%First Derivative of Mean Motion (rad/s2)
a = state(3);%(mu/n^2)^(1/3);Semi-Major Axis
e = state(4);%Eccentricity
i = state(5);%Inclination (rad)
Om = state(6);%RAAN (rad)
om = state(7);%Arg of Perigee (rad)
nu = state(8);%True Anomaly (rad)
nuD = state(9);%Angular Velocity (rad/s)
r = state(10);%(a*(1-e^2))/(1+e*cos(nu))%Radius (km)
rD = state(11);%Radial Velocity (km/s)
stateD(1) = nD;
stateD(2) = nD20;
stateD(3) = 0;
stateD(4) = 0;
stateD(5) = 0;
stateD(6) = (pi*cos(i))/(43200*n)*(-0.00338-0.00154)*pi/15552000 ...
 -1.5*n*J2*(Re/a)^2*cos(i)/((1-e^2)^2);
stateD(7) = (pi*(4-5*(sin(i))^2))/(43200*n)*(0.00169+0.00077)*pi/15552000 ...
 +0.75*n*J2*(Re/a)^2*(4-5*(sin(i))^2)/((1-e^2)^2);
stateD(8) = nuD;
stateD(9) = -2*rD*nuD/r;
stateD(10) = rD;
stateD(11) = -mu/r^2 + r*nuD^2;
stateD=stateD';
%==========================================================================
function xb=xi2xb(xi,q)
%Inertial->Body Quaternion Transformation Matrix for Orbit Propagator
xin=xi';
for ndx=1:length(xi)
 i2b=[1 - 2*(q(ndx,2)^2 + q(ndx,3)^2), 2*(q(ndx,1)*q(ndx,2) +
q(ndx,3)*q(ndx,4)), 2*(q(ndx,1)*q(ndx,3) - q(ndx,2)*q(ndx,4)); 

 2*(q(ndx,1)*q(ndx,2) - q(ndx,3)*q(ndx,4)), 1 - 2*(q(ndx,1)^2 +
q(ndx,3)^2), 2*(q(ndx,2)*q(ndx,3) + q(ndx,1)*q(ndx,4));
 2*(q(ndx,1)*q(ndx,3) + q(ndx,2)*q(ndx,4)), 2*(q(ndx,2)*q(ndx,3) -
q(ndx,1)*q(ndx,4)), 1 - 2*(q(ndx,1)^2 + q(ndx,2)^2)];
 xbn(:,ndx)= i2b*xin(:,ndx);
end
xb=xbn';
%==========================================================================
function Bb=Bi2Bb(to,Bio,ta,q)
%Inertial->Body Quaternion Transformation Matrix for Orbit Propagator

Bia=interp1(to,Bio,ta);
Bia=Bia';
for ndx=1:length(ta)
 i2b=[1 - 2*(q(ndx,2)^2 + q(ndx,3)^2), 2*(q(ndx,1)*q(ndx,2) +
q(ndx,3)*q(ndx,4)), 2*(q(ndx,1)*q(ndx,3) - q(ndx,2)*q(ndx,4));
 2*(q(ndx,1)*q(ndx,2) - q(ndx,3)*q(ndx,4)), 1 - 2*(q(ndx,1)^2 +
q(ndx,3)^2), 2*(q(ndx,2)*q(ndx,3) + q(ndx,1)*q(ndx,4));
 2*(q(ndx,1)*q(ndx,3) + q(ndx,2)*q(ndx,4)), 2*(q(ndx,2)*q(ndx,3) -
q(ndx,1)*q(ndx,4)), 1 - 2*(q(ndx,1)^2 + q(ndx,2)^2)];
 Bb(:,ndx)= i2b*Bia(:,ndx);
end
Bb=Bb';
%==========================================================================
function stateD = aDiffEq(t,state,I,K,mub,to,Bi,Treq,test,mm)
%The Attitude Differential Equations

w=state(1:3);
q=state(4:7);
Bb = Bi2Bb(to,Bi,t,q'); %Magnetic field in body frame (Tesla)
if test==1
 T = Treq; %Torque directly from command (for debugging)
else
 T = cross(mub',Bb'); %Torque using magnetorquers
end
mm1 = interp1(to,mm,t); %Mean Motion(rad/s)
C11 = 1-2*(q(2)^2+q(3)^2); %3 components of quaternion transformation
C21 = 2*(q(1)*q(2)-q(3)*q(4)); %matrix necessary for gravity gradient
C31 = 2*(q(1)*q(3)+q(2)*q(4));
% GG = [0 0 0];
GG = [-K(1)*3*mm1^2*C21*C31 -K(2)*3*mm1^2*C11*C31 -K(3)*3*mm1^2*C11*C21];
wD = [K(1)*w(2)*w(3)+T(1)/I(1)+GG(1);
 K(2)*w(3)*w(1)+T(2)/I(2)+GG(2);
 K(3)*w(1)*w(2)+T(3)/I(3)+GG(3)];
Wp = [ 0, w(3), -w(2), w(1);
 -w(3), 0, w(1), w(2);
 w(2), -w(1), 0, w(3);
 -w(1), -w(2), -w(3), 0]; 

qD = 1/2*Wp*q;
stateD=[wD;qD];
%========================================================================== 