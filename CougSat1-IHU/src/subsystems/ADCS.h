#ifndef SRC_SUBSYSTEMS_ADCS_H_
#define SRC_SUBSYSTEMS_ADCS_H_

#include "Subsystem.h"
#include <mbed.h>

const uint8_t I2C_DATA_SIZE  = 9;

enum ADCS_COMMAND {
    ADCS_CMD_LOCATION_REQ,
    ADCS_CMD_ORIENTATION_REQ,
    ADCS_CMD_TEMPERATURE_REQ,
    ADCS_CMD_COIL_CONTROL_REQ,
    ADCS_CMD_COIL_CURRENT_REQ,
    ADCS_CMD_GENERAL_ROTATION_CMD,
    ADCS_CMD_CAMERA_ORIENTATION_CMD,
    ADCS_CMD_ANTENNA_ORIENTATION_CMD,
    ADCS_CMD_ROAST_THE_CHICKEN_CMD,
    ADCS_CMD_DRIFT_CMD,
    ADCS_CMD_TWO_LINE_ELEMENTS_CMD,
    ADCS_CMD_END_MANEUVER_CMD
};

class ADCS : public Subsystem {
public:
  ADCS(const ADCS &) = delete;
  ADCS & operator=(const ADCS &) = delete;

  ADCS(I2C & i2c);
  ~ADCS();

  mbed_error_status_t initialize();
  mbed_error_status_t receiveBuffer(int8_t command, char* rx_buffer, size_t buffer_size);

private:
  I2C & i2c;

  int32_t location[2] =        { 0 };
  int16_t orientation[3] =     { 0 };
  int8_t coil_temperature[3] = { 0 };
  int8_t adcs_temperature =    0;
  int8_t gps_temperature =     0;
  int8_t coil_control[3] =     { 0 };
  int8_t coil_current[3] =     { 0 };
};

#endif /* SRC_SUBSYSTEMS_ADCS_H_ */