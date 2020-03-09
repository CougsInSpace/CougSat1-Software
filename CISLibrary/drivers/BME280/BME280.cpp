/**
 ******************************************************************************
 * @file    BME280.cpp
 * @author  Toyomasa Watarai
 * @version V1.0.0
 * @date    11 March 2017
 * @brief   BME280 class implementation
 ******************************************************************************
 * @attention
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "mbed.h"
#include "BME280.h"

BME280::BME280(PinName sda, PinName scl, char slave_adr)
    :
    i2c_p(new I2C(sda, scl)), 
    i2c(*i2c_p),
    address(slave_adr),
    t_fine(0)
{
    initialize();
}

BME280::BME280(I2C &i2c_obj, char slave_adr)
    :
    i2c_p(NULL), 
    i2c(i2c_obj),
    address(slave_adr),
    t_fine(0)
{
    initialize();
}

BME280::~BME280()
{
    if (NULL != i2c_p)
        delete  i2c_p;
}
    
void BME280::initialize()
{
    char cmd[18];
 
    cmd[0] = 0xf2; // ctrl_hum
    cmd[1] = 0x01; // Humidity oversampling x1
    i2c.write(address, cmd, 2);
 
    cmd[0] = 0xf4; // ctrl_meas
    cmd[1] = 0x27; // Temparature oversampling x1, Pressure oversampling x1, Normal mode
    i2c.write(address, cmd, 2);
 
    cmd[0] = 0xf5; // config
    cmd[1] = 0xa0; // Standby 1000ms, Filter off
    i2c.write(address, cmd, 2);
 
    cmd[0] = 0x88; // read dig_T regs
    i2c.write(address, cmd, 1);
    i2c.read(address, cmd, 6);
 
    dig_T1 = (cmd[1] << 8) | cmd[0];
    dig_T2 = (cmd[3] << 8) | cmd[2];
    dig_T3 = (cmd[5] << 8) | cmd[4];
 
    cmd[0] = 0x8E; // read dig_P regs
    i2c.write(address, cmd, 1);
    i2c.read(address, cmd, 18);
 
    dig_P1 = (cmd[ 1] << 8) | cmd[ 0];
    dig_P2 = (cmd[ 3] << 8) | cmd[ 2];
    dig_P3 = (cmd[ 5] << 8) | cmd[ 4];
    dig_P4 = (cmd[ 7] << 8) | cmd[ 6];
    dig_P5 = (cmd[ 9] << 8) | cmd[ 8];
    dig_P6 = (cmd[11] << 8) | cmd[10];
    dig_P7 = (cmd[13] << 8) | cmd[12];
    dig_P8 = (cmd[15] << 8) | cmd[14];
    dig_P9 = (cmd[17] << 8) | cmd[16];
 
    cmd[0] = 0xA1; // read dig_H regs
    i2c.write(address, cmd, 1);
    i2c.read(address, cmd, 1);
     cmd[1] = 0xE1; // read dig_H regs
    i2c.write(address, &cmd[1], 1);
    i2c.read(address, &cmd[1], 7);

    dig_H1 = cmd[0];
    dig_H2 = (cmd[2] << 8) | cmd[1];
    dig_H3 = cmd[3];
    dig_H4 = (cmd[4] << 4) | (cmd[5] & 0x0f);
    dig_H5 = (cmd[6] << 4) | ((cmd[5]>>4) & 0x0f);
    dig_H6 = cmd[7];
}
 
float BME280::getTemperature()
{
    uint32_t temp_raw;
    float tempf;
    char cmd[4];
 
    cmd[0] = 0xfa; // temp_msb
    i2c.write(address, cmd, 1);
    i2c.read(address, &cmd[1], 3);
 
    temp_raw = (cmd[1] << 12) | (cmd[2] << 4) | (cmd[3] >> 4);
 
    int32_t temp;
 
    temp =
        (((((temp_raw >> 3) - (dig_T1 << 1))) * dig_T2) >> 11) +
        ((((((temp_raw >> 4) - dig_T1) * ((temp_raw >> 4) - dig_T1)) >> 12) * dig_T3) >> 14);
 
    t_fine = temp;
    temp = (temp * 5 + 128) >> 8;
    tempf = (float)temp;
 
    return (tempf/100.0f);
}
 
float BME280::getPressure()
{
    uint32_t press_raw;
    float pressf;
    char cmd[4];
 
    cmd[0] = 0xf7; // press_msb
    i2c.write(address, cmd, 1);
    i2c.read(address, &cmd[1], 3);
 
    press_raw = (cmd[1] << 12) | (cmd[2] << 4) | (cmd[3] >> 4);
 
    int32_t var1, var2;
    uint32_t press;
 
    var1 = (t_fine >> 1) - 64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * dig_P6;
    var2 = var2 + ((var1 * dig_P5) << 1);
    var2 = (var2 >> 2) + (dig_P4 << 16);
    var1 = (((dig_P3 * (((var1 >> 2)*(var1 >> 2)) >> 13)) >> 3) + ((dig_P2 * var1) >> 1)) >> 18;
    var1 = ((32768 + var1) * dig_P1) >> 15;
    if (var1 == 0) {
        return 0;
    }
    press = (((1048576 - press_raw) - (var2 >> 12))) * 3125;
    if(press < 0x80000000) {
        press = (press << 1) / var1;
    } else {
        press = (press / var1) * 2;
    }
    var1 = ((int32_t)dig_P9 * ((int32_t)(((press >> 3) * (press >> 3)) >> 13))) >> 12;
    var2 = (((int32_t)(press >> 2)) * (int32_t)dig_P8) >> 13;
    press = (press + ((var1 + var2 + dig_P7) >> 4));
 
    pressf = (float)press;
    return (pressf/100.0f);
}
 
float BME280::getHumidity()
{
    uint32_t hum_raw;
    float humf;
    char cmd[4];
 
    cmd[0] = 0xfd; // hum_msb
    i2c.write(address, cmd, 1);
    i2c.read(address, &cmd[1], 2);
 
    hum_raw = (cmd[1] << 8) | cmd[2];
 
    int32_t v_x1;
 
    v_x1 = t_fine - 76800;
    v_x1 =  (((((hum_raw << 14) -(((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * v_x1)) +
               ((int32_t)16384)) >> 15) * (((((((v_x1 * (int32_t)dig_H6) >> 10) *
                                            (((v_x1 * ((int32_t)dig_H3)) >> 11) + 32768)) >> 10) + 2097152) *
                                            (int32_t)dig_H2 + 8192) >> 14));
    v_x1 = (v_x1 - (((((v_x1 >> 15) * (v_x1 >> 15)) >> 7) * (int32_t)dig_H1) >> 4));
    v_x1 = (v_x1 < 0 ? 0 : v_x1);
    v_x1 = (v_x1 > 419430400 ? 419430400 : v_x1);
 
    humf = (float)(v_x1 >> 12);
 
    return (humf/1024.0f);
}
