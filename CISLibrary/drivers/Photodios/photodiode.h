#ifndef PHOTODIODE_H
#define PHOTODIODE_H
#include <mbed.h>
// This file reads in the voltage from a photodiode from all 6 sides of Cougsat.


// Struct for holding voltage values from photodiodes on each side.
typedef struct voltages
{
    float volt_pos_x;
    float volt_neg_x;
    float volt_pos_y;
    float volt_neg_y;
    float volt_pos_z;
    float volt_neg_z;
}Voltages;

typedef struct sunvector
{
    float x;
    float y;
    float z;
}SunVector;

// Class for reading voltage ouput from photodiodes.
class Photodiodes {
public:
    Photodiodes(PinName pin_1, PinName pin_2, PinName pin_3, PinName pin_4, PinName pin_5, PinName pin_6);
    Voltages* getVoltages();
    float get_pos_x();
    float get_neg_x();
    float get_pos_y();
    float get_neg_y();
    float get_pos_z();
    float get_neg_z();
    SunVector getSunVector();
    void setOffset(double posX, double negX, double posY, double negY ,double posZ, double negZ);
private:
    double pos_x_offset;
    double neg_x_offset;
    double pos_y_offset;
    double neg_y_offset;
    double pos_z_offset;
    double neg_z_offset;

    AnalogIn pos_x; // each of these read in from the constructor
    AnalogIn neg_x;
    AnalogIn pos_y;
    AnalogIn neg_y;
    AnalogIn pos_z;
    AnalogIn neg_z;
    Voltages curVolt;
};



#endif /* PHOTODIODE_H */