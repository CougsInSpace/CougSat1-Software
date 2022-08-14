#include "photodiode.h"

Photodiodes::Photodiodes(PinName pin_1, PinName pin_2, PinName pin_3, PinName pin_4, PinName pin_5, PinName pin_6): 
pos_x(pin_1), neg_x(pin_2), pos_y(pin_3), neg_y(pin_4), pos_z(pin_5), neg_z(pin_6) // reads in each pin from board to AnalogIn values.
{
    pos_x_offset = 0;
    neg_x_offset = 0;
    pos_y_offset = 0;
    neg_y_offset = 0;
    pos_z_offset = 0;
    neg_z_offset = 0;
}

Voltages* Photodiodes::getVoltages() // Reads voltages into struct voltages
{
    curVolt.volt_pos_x = pos_x.read() - pos_x_offset;
    curVolt.volt_pos_x < 0 ? curVolt.volt_pos_x = 0 : 0;
    curVolt.volt_neg_x = neg_x.read() - neg_x_offset;
    curVolt.volt_neg_x < 0 ? curVolt.volt_neg_x = 0 : 0;
    curVolt.volt_pos_y = pos_y.read() - pos_y_offset;
    curVolt.volt_pos_y < 0 ? curVolt.volt_pos_y = 0 : 0;
    curVolt.volt_neg_y = neg_y.read() - neg_y_offset;
    curVolt.volt_neg_y < 0 ? curVolt.volt_neg_y = 0 : 0;
    curVolt.volt_pos_z = pos_z.read() - pos_z_offset;
    curVolt.volt_pos_z < 0 ? curVolt.volt_pos_z = 0 : 0;
    curVolt.volt_neg_z = neg_z.read() - neg_z_offset;
    curVolt.volt_neg_z < 0 ? curVolt.volt_neg_z = 0 : 0;

    return &curVolt;
}

float Photodiodes::get_pos_x() // returns positive x photodiode voltage
{
    float x = pos_x.read();
    return x;
}
float Photodiodes::get_neg_x() // returns negative x photodiode voltage
{
    float x = neg_x.read();
    return x;
}
float Photodiodes::get_pos_y() // returns positive y photodiode voltage
{
    float x = pos_y.read();
    return x;
}
float Photodiodes::get_neg_y() // returns negative y photodiode voltage
{
    float x = neg_y.read();
    return x;
}
float Photodiodes::get_pos_z() // returns positive z photodiode voltage
{
    float x = pos_z.read();
    return x;
}
float Photodiodes::get_neg_z() // returns positive z photodiode voltage
{
    float x = neg_z.read();
    return x;
}
void Photodiodes::setOffset(double posX, double negX, double posY, double negY ,double posZ, double negZ){
    this->pos_x_offset = posX;
    this->neg_x_offset = negX;
    this->pos_y_offset = posY;
    this->neg_y_offset = negY;
    this->pos_z_offset = posZ;
    this->neg_z_offset = negZ;

}

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
}

