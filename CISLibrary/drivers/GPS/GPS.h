#ifndef _LIBRARY_DRIVER_GPS_GPS_H_
#define _LIBRARY_DRIVER_GPS_GPS_H_

#include <mbed.h>

struct GPSData_t {
  double   latitude    = nan("");
  double   longitude   = nan("");
  double   altitude    = nan("");
  double   groundSpeed = nan("");
  uint32_t time        = 0;
};

class GPS {
public:
  GPS(const GPS &) = delete;
  GPS & operator=(const GPS &) = delete;

  /**
   * @brief Construct a new GPS object
   *
   */
  GPS() {}

  /**
   * @brief Destroy the GPS object
   *
   */
  virtual ~GPS() {};

  /**
   * @brief Read a GPS message and update fields
   *
   * @param data struct to return result
   * @param blocking will wait until data is present if true
   * @return mbed_error_status_t
   */
  virtual mbed_error_status_t read(GPSData_t & data, bool blocking = true) = 0;
};

#endif /* _LIBRARY_DRIVER_GPS_GPS_H_ */