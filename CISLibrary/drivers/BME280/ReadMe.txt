This document is intended to outline the functionality and design of the Bosch Sensortech BME280 temperature/pressure/humidity sensor driver written by Toyomasa Watarai

class Name: BME280
Description: Primary class for communicating with and reading values from a BME280 sensor
Author: Toyomasa Watarai

Functions:

Name: BME280()
Description: Initializer for the BME280 Class
Parameters: i2c pins individually or in an I2C class, optionally a non-default slave address
Returns: None

Name: ~BME280()
Description: Destructor for the BME280 Class.
Parameters: None
Returns: None

Name: initialize()
Description: Calibrates sensor and prepares it to read values
Parameters: None
Returns: None

Name: getTemperature()
Description: Returns the current temperature in degrees celsius as a float
Parameters: None
Returns: Temperature in degrees celsius as a float

Name: getPressure()
Description: Returns the current pressure in hectopascals as a float
Parameters: None
Returns: pressure in hectopascals as a float

Name: getHumidity()
Description: Returns the current humidity in percent humidity as a float
Parameters: None
Returns: humidity in percent humidity as a float