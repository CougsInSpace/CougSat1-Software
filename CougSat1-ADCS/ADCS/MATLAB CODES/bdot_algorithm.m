%bdot algorithm cosidering polar orbit as a simulation, and considering
%ideal cases. no change in the frame of reference.


clear all;
clc;
%also add the constraints related to the power

%% Specify time
dt = 0.32; T = 90*60*3;

t=0:dt:T;
%% Initializations
inertia=[0.00602377522,0.00000294634,0.00000423169;0.00000294634,0.00130455071,0.00001311622;0.00000294634,0.00001311622,0.00601351576]';

alti=600000; % in meters 
ang_vel = zeros(length(t),3);
cang=[0.1;-1.5;0.3]';%initial angular velocity when the satellite is launched
Re = 6371.2e3;
 Rc = Re+alti; %  Distance from earth in metres
 p=((111e3*Rc)/Re);
 
 G = 6.67428e-11; % Earth gravitational constant
M = 5.972e24; % Earth mass
linvel = sqrt(G*M/Rc); %linear velocity of the satellite

angacc=[0;0;0]';

%% Main loop
 for i=1:length(t)
    cangi=cang;
    %represents the polar orbit
    lati=(linvel*i*dt)/p;% as its a latitude changes differently when its in space.
    lat=25+lati; 
    % setting up the latitude for the law, latitude changes due to the change in the y coordinate of the system.
    long=60+((7.29e-5*i*dt)*180/3.14);
    %longitude change as the eath is rotating
    % Gives error on 90
     if lat>90
        b=mod(lat,90);
        lat=90-b;
    end
    if lat<-90
        c=mod(lat,-90);
        lat=-90-c; 
    end
     if long>180
        d=mod(long,180);
        long=d-180;

     end
    if long<-180
        l=mod(long,-180);
        long=180+l;
    end
    if lat==90
        lat=89.97;
    end
    if lat==-90
        lat=-89.97;
    end
   
        
    [mag_field_vector1,hor_intensity,declination,inclination,total_intensity] = igrfmagm(alti,lat,long,decyear(2015,7,4),12); 
    mag_field_vector=mag_field_vector1*1e-9;
    % as the magnetic field vecctor has the value in nano tesla
    % convert the obtained magnetic field in the frame of reference of the
    % satellite.
    %% B-dot
    detb=sqrt(dot((mag_field_vector),(mag_field_vector)));
    m=((-1)/detb)*cross(mag_field_vector,cang); %cang is the current angular velocity
    
    vtorque=cross(m,mag_field_vector);   
    inertiain=inv(inertia);
    angacc=vtorque*inertiain;
      
    cang=cangi+(angacc*dt);%calculates the new angular velocity

    disp('new angular velocity');
    disp (cang);
    ang_vel(i,:)=cang;
    
     
 end

%% Plots
figure(1)
subplot(3,1,1)
plot(t/60,ang_vel(:,1));
subplot(3,1,2)
plot(t/60,ang_vel(:,2));
subplot(3,1,3)
plot(t/60,ang_vel(:,3));