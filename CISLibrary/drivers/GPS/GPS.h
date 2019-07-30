#ifndef _LIBRARY_DRIVER_GPS_GPS_H_
#define _LIBRARY_DRIVER_GPS_GPS_H_

#include "CISError.h"

#include <mbed.h>

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
  ~GPS() {};

  /**
   * @brief Read a GPS message and update fields
   *
   * @param blocking will wait until data is present
   * @return CISResult_t error code
   */
  virtual CISResult_t update(bool blocking = true) = 0;

  /**
   * @brief Get the latitude
   *
   * @return double degrees [-90.0, 90.0]
   */
  double getLatitude() {
    return latitude;
  }

  /**
   * @brief Get the longitude
   *
   * @return double degrees [-180.0, 180.0]
   */
  double getLongitude() {
    return longitude;
  }

  /**
   * @brief Get the altitude
   *
   * @return double altitude in meters
   */
  double getAltitude() {
    return altitude;
  }

  /**
   * @brief Get the speed over the ground
   *
   * @return double speed in meters per seconds
   */
  double getGroundSpeed() {
    return groundSpeed;
  }

  /**
   * @brief Get the time
   *
   * @return uint32_t seconds since EPOCH
   */
  uint32_t getTime() {
    return time;
  }

protected:
  double   latitude;
  double   longitude;
  double   altitude;
  double   groundSpeed;
  uint32_t time;
};

#endif /* _LIBRARY_DRIVER_GPS_GPS_H_ */