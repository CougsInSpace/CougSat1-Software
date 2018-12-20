#include <PID_v1.h>

#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
/* This driver reads raw data from the BNO055

   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3V DC
   Connect GROUND to common ground

   History
   =======
   2015/MAR/03  - First release (KTOWN)
*/

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (10)
#define SERVOMIN  860 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  2100 // this is the 'maximum' pulse length count (out of 4096)


Servo PWM;
Adafruit_BNO055 bno = Adafruit_BNO055();

// pwm = Adafruit_PWMServoDriver();

int LED_BLUE = 10;
int GO_FLIGHT = 7;
int LED_RED = 9;
int LED_CALIB = 12;
int Off_By_x = 5;
double Input_x;
const long interval = 2000;
unsigned long previousMillis = 0;
int onLeft = 0;
int pos_lock = 0;
int onRight = 0;
/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
  Serial.begin(9600);
  Serial.println("Orientation Sensor Raw Data Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  PWM.attach(10);
  PWM.writeMicroseconds(1500);
 // pwm.begin();
 // pwm.setPWMFreq(1600);
  
  delay(1000);

  //Initialize variables we are linked to
  float Input_x = 0;
  int sensor_update_time=500;
  volatile unsigned long current_time=millis();
  
  //Initialize LED's and Read Pin
  pinMode(GO_FLIGHT, INPUT);
  pinMode(LED_BLUE, OUTPUT);//off left
  pinMode(LED_RED, OUTPUT);//off right
  pinMode(LED_CALIB, OUTPUT);//sensor calibrated
  
  /* Display the current temperature */
  int8_t temp = bno.getTemp();
  Serial.print("Current Temperature: ");
  Serial.print(temp);
  Serial.println(" C");
  Serial.println("");

  bno.setExtCrystalUse(true);
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void update_sensor_data()
{
  sensors_event_t event;
  bno.getEvent(&event);

  Input_x=event.orientation.x;
}

void loop(void)
{
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  int z =1;
  
  uint8_t system, gyro, accel, mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);
  
  /* Display the floating point data */
  if (z == 1){
  Serial.print("X: ");
  Serial.print(euler.x());
  Serial.print(" Y: ");
  Serial.print(euler.y());
  Serial.print(" Z: ");
  Serial.print(euler.z());
  Serial.print("\t\t");

  /* Display calibration status for each sensor. */
  Serial.print("CALIBRATION: Sys=");
  Serial.print(system, DEC);
  Serial.print(" Gyro=");
  Serial.print(gyro, DEC);
  Serial.print(" Accel=");
  Serial.print(accel, DEC);
  Serial.print(" Mag=");
  Serial.println(mag, DEC);
  }
  if(mag == 3){
    digitalWrite(LED_CALIB, HIGH);
    pos_lock = 1;
  }
  else{
    digitalWrite(LED_CALIB, LOW);
  }
  int val= 0;
  val = digitalRead(GO_FLIGHT);//switch is in testing mode
  if (val == 1){
     double x = euler.x();
     if ((x > Off_By_x*2) && (x < 180)){
          PWM.writeMicroseconds(960);
        //digitalWrite(LED_BLUE, LOW);
        //digitalWrite(LED_RED, HIGH);
     }
     else if((x < (360 - Off_By_x*2))&& (x > 180)){
         PWM.writeMicroseconds(2000);
        //digitalWrite(LED_RED, LOW);
        //digitalWrite(LED_BLUE, HIGH);
     }
     else if((x > (Off_By_x)) && (x < Off_By_x*2)){
         PWM.writeMicroseconds(960);
     }
     else if((x < (360 - (Off_By_x))) && (x > (360 - Off_By_x*2))){
         PWM.writeMicroseconds(2000);
     }
     else{
        PWM.writeMicroseconds(1500);
       // digitalWrite(LED_RED, LOW);
       //digitalWrite(LED_BLUE, LOW);
     }
  }
  delay(BNO055_SAMPLERATE_DELAY_MS);
}
