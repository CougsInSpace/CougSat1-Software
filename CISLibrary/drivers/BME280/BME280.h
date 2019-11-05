/**
 ******************************************************************************
 * @file    BME280.h
 * @author  Toyomasa Watarai
 * @version V1.0.0
 * @date    11 March 2017
 * @brief   This file contains the class of a BME280 Combined humidity and pressure sensor library with I2C interface
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
 
/**
 *  Library for "BME280 temperature, humidity and pressure sensor module" from Switch Science
 *    https://www.switch-science.com/catalog/2236/
 *
 *  For more information about the BME280:
 *    http://ae-bst.resource.bosch.com/media/products/dokumente/bme280/BST-BME280_DS001-10.pdf
 */
 
#ifndef MBED_BME280_H
#define MBED_BME280_H

#include "mbed.h"

#define DEFAULT_SLAVE_ADDRESS (0x76 << 1)

#ifdef _DEBUG
extern Serial pc;
#define DEBUG_PRINT(...) pc.printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

/**  Interface for controlling BME280 Combined humidity and pressure sensor
 *
 * @code
 * #include "mbed.h"
 * #include "BME280.h"
 * 
 * Serial pc(USBTX, USBRX);
 * 
 * #if defined(TARGET_LPC1768)
 * BME280 sensor(p28, p27);
 * #else
 * BME280 sensor(I2C_SDA, I2C_SCL);
 * #endif
 * 
 * int main() {
 *     
 *     while(1) {
 *         pc.printf("%2.2f degC, %04.2f hPa, %2.2f %%\n", sensor.getTemperature(), sensor.getPressure(), sensor.getHumidity());
 *         wait(1);
 *     }
 * }
 * 
 * @endcode
 */

/** BME280 class
 *
 *  BME280: A library to correct environmental data using Boshe BME280 environmental sensor device
 *
 */ 
class BME280
{
public:

    /** Create a BME280 instance
     *  which is connected to specified I2C pins with specified address
     *
     * @param sda I2C-bus SDA pin
     * @param scl I2C-bus SCL pin
     * @param slave_adr (option) I2C-bus address (default: 0x76)
     */
    BME280(PinName sda, PinName sck, char slave_adr = DEFAULT_SLAVE_ADDRESS);

    /** Create a BME280 instance
     *  which is connected to specified I2C pins with specified address
     *
     * @param i2c_obj I2C object (instance)
     * @param slave_adr (option) I2C-bus address (default: 0x76)
     */
    BME280(I2C &i2c_obj, char slave_adr = DEFAULT_SLAVE_ADDRESS);

    /** Destructor of BME280
     */
    virtual ~BME280();

    /** Initializa BME280 sensor
     *
     *  Configure sensor setting and read parameters for calibration
     *
     */
    void initialize(void);

    /** Read the current temperature value (degree Celsius) from BME280 sensor
     *
     */
    float getTemperature(void);

    /** Read the current pressure value (hectopascal)from BME280 sensor
     *
     */
    float getPressure(void);

    /** Read the current humidity value (humidity %) from BME280 sensor
     *
     */
    float getHumidity(void);

private:

    I2C         *i2c_p;
    I2C         &i2c;
    char        address;
    uint16_t    dig_T1;
    int16_t     dig_T2, dig_T3;
    uint16_t    dig_P1;
    int16_t     dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
    uint16_t    dig_H1, dig_H3;
    int16_t     dig_H2, dig_H4, dig_H5, dig_H6;
    int32_t     t_fine;

};

#endif // MBED_BME280_H
