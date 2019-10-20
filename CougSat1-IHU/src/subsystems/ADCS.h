#ifndef SRC_SUBSYSTEMS_ADCS_H_
#define SRC_SUBSYSTEMS_ADCS_H_

#include "Subsystem.h"
#include <mbed.h>
#include <tuple>

const uint8_t I2C_ADCS_ADDRESS = 0xAD;
const uint8_t I2C_DATA_LENGTH  = 7;

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
  mbed_error_status_t requestLocation();

  std::tuple<int32_t, int32_t> getLocation();

protected:
  char tx_buffer[I2C_DATA_LENGTH] = {};
  char rx_buffer[I2C_DATA_LENGTH] = {};

  event_callback_t transfer_complete_callback;

private:
  I2C & i2c;

  bool i2c_busy = false;

  int32_t latitude  = 0;
  int32_t longitude = 0;

  void receiveDataHandler(int);

  void receiveLocation(int);
};

#endif /* SRC_SUBSYSTEMS_ADCS_H_ */