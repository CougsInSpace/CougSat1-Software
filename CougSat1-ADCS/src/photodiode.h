#ifndef PHOTODIODE_H
#define PHOTODIODE_H
#include <mbed.h>
// This file reades in the voltage from a photodiode from all 6 sides of Cougsat.


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
private:
    AnalogIn pos_x; // each of these read in from the constructor
    AnalogIn neg_x;
    AnalogIn pos_y;
    AnalogIn neg_y;
    AnalogIn pos_z;
    AnalogIn neg_z;
    Voltages curVolt;
};


Photodiodes::Photodiodes(PinName pin_1, PinName pin_2, PinName pin_3, PinName pin_4, PinName pin_5, PinName pin_6): 
pos_x(pin_1), neg_x(pin_2), pos_y(pin_3), neg_y(pin_4), pos_z(pin_5), neg_z(pin_6) // reads in each pin from board to AnalogIn values.
{
    
}

Voltages* Photodiodes::getVoltages() // Reads voltages into struct voltages
{
    curVolt.volt_pos_x = pos_x.read();
    curVolt.volt_neg_x = neg_x.read();
    curVolt.volt_pos_y = pos_y.read();
    curVolt.volt_neg_y = neg_y.read();
    curVolt.volt_pos_z = pos_z.read();
    curVolt.volt_neg_z = neg_z.read();
    return &curVolt;
}

float Photodiodes::get_pos_x() // returns positive x photodiode voltage
{
    float x = pos_x.read();
    return x;
};
float Photodiodes::get_neg_x() // returns negative x photodiode voltage
{
    float x = neg_x.read();
    return x;
};
float Photodiodes::get_pos_y() // returns positive y photodiode voltage
{
    float x = pos_y.read();
    return x;
};
float Photodiodes::get_neg_y() // returns negative y photodiode voltage
{
    float x = neg_y.read();
    return x;
};
float Photodiodes::get_pos_z() // returns positive z photodiode voltage
{
    float x = pos_z.read();
    return x;
};
float Photodiodes::get_neg_z() // returns positive z photodiode voltage
{
    float x = neg_z.read();
    return x;
};

SunVector Photodiodes::getSunVector() // calculates a vector that points at the sun
{
    SunVector sunVector;
    float x = get_pos_x() - get_neg_x();
    float y = get_pos_x() - get_neg_x();
    float z = get_pos_x() - get_neg_x();
    sunVector.x = x;
    sunVector.y = y;
    sunVector.z = z;
    return sunVector;
};



#endif /* PHOTODIODE_H */