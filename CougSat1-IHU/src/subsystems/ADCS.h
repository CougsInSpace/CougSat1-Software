#ifndef SRC_SUBSYSTEMS_ADCS_H_
#define SRC_SUBSYSTEMS_ADCS_H_

#include "Subsystem.h"
#include <mbed.h>


typedef enum ADCS_COMMAND {
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
    ADCS_CMD_END_MANEUVER
};

class ADCS : public Subsystem {
public:
  ADCS(const ADCS &) = delete;
  ADCS & operator=(const ADCS &) = delete;

  ADCS(I2C & i2c);
  ~ADCS();

  mbed_error_status_t initialize();

private:
  I2C & i2c;
};

#endif /* SRC_SUBSYSTEMS_ADCS_H_ */