#ifndef SRC_SUBSYSTEMS_PAYLOAD_H_
#define SRC_SUBSYSTEMS_PAYLOAD_H_

#include "Subsystem.h"

#include "mbed.h"
// Includes the BME280 driver inside the CISLibrary for
// use by getPressure(), getTemperature(), and getHumidity().
#include "../../../CISLibrary/drivers/BME280/BME280.h"

/** 
 * The Payload class is a Subsystem serving the purpose of collecting
 * specific data about the environment within the CubeSat.
 * This includes: CO2 concentration, pressure, etc...
 * These data will then be used to monitor the environment before
 * the germination process initiates.
 * It is also responsible for capturing photos of the environment.
 *
 * So far, the Payload team has yet to figure out the broad issues listed:
 * - Whether the camera's timer (which runs every few hours) should be
 *   implemented within this class, or if an external system may handle it.
 * - If the experiment should simply be one method, or if it should be
 *   handled by an external system.
 * - In general, what is required of the Payload class for other
 *   subsystems and what processes the Payload class can/should handle on its own.
 */
class Payload : public Subsystem {
public:
  Payload(const Payload &) = delete;
  Payload & operator=(const Payload &) = delete;

  /** 
   * Constructor of the Payload class.
   * 
   * TODO: Discuss any variables or systems that need
   * to be initialized (if needed).
   */
  Payload();

  /** 
   * Desconstructor of the Payload class.
   * 
   * TODO: Discuss whether this is needed, as so far there
   * are no variables or systems that need to be deleted/unloaded.
   */
  ~Payload();

  /** 
   * Gets the current CO2 level of the environment.
   * @return The current CO2 concentration reading of the T67xx sensor
   * 
   * TODO: Discuss how exactly this should be implemented.
   */
  float getCO2Concentration() const;

  /** 
   * Gets the current pressure of the environment
   * using the BME280 sensor using the BME280Driver.
   * @return The current pressure (hPa) reading of the BME280 sensor
   * 
   * TODO: Discuss how exactly this should be implemented.
   */
  float getPressure() const;

  /** 
   * Gets the current temperature of the environment
   * using the BME280 sensor using the BME280Driver.
   * @return The current temperature (celsius) reading of the BME280 sensor
   * 
   * TODO: Discuss how exactly this should be implemented.
   */
  float getTemperature() const;

  /** 
   * Gets the current humidity of the environment
   * using the BME280 sensor using the BME280Driver.
   * (This may not be required).
   * @return The current humidity (from 0.0% to 100.0%) reading of the BME280 sensor
   * 
   * TODO: Discuss how exactly this should be implemented.
   */
  float getHumidity() const;

  /** 
   * Determines whether the environment is ready for the
   * experiment to run, by checking the pressure, temperature,
   * humidity, CO2 level, etc... 
   * 
   * This should be "true" before the experiment is ran.
   * 
   * @returns True if the environment is sufficient for the experiment
   * 
   * TODO: Implement all getters before implementing
   * this function.
   */
  bool isEnvironmentReady() const;

  /** 
   * Turns on the LED inside the chamber.
   *  
   * TODO: Discuss whether this should be an internalized
   * method, and discuss when this should be used within this class.
   */
  void turnOnLED();

  /** 
   * Turns off the LED inside the chamber.
   * 
   * TODO: Discuss whether this should be an internalized
   * method, and discuss when this should be used within this class.
   */
  void turnOffLED();

  /** 
   * Activates the syringe for the germination process.
   * Note: Because, for this specific CubeSat, the germination
   * process will only run once, this syringe will only be able
   * to be activated once.
   * 
   * TODO: Discuss whether this should be an internalized
   * method, and discuss when this should be used within this class.
   */
  void activateSyringe();

  /** 
   * Takes a picture of the environment. This should be compressing the
   * picture prior to returning it.
   * 
   * TODO: Discuss whether this should be an internalized
   * method, and discuss when this should be used within this class.
   * If we do need to implement our own timer for taking pictures at
   * an interval, then this method may be called internally.
   */
  void takePicture();

  /** 
   * Initialize the Payload class. Theoretically, this should
   * be called in the constructor of the Payload class.
   * 
   * TODO: Discuss whatever needs to be initialized.
   */
  mbed_error_status_t initialize();

private:
  /** 
   * Every method is temporarily public. There needs to be a 
   * discussion about what gets exposed to outside code and what
   * needs to be internalized, so that no systems may accidentally
   * break the processes of this class.
   */
};

#endif /* SRC_SUBSYSTEMS_PAYLOAD_H_ */