#ifndef _SRC_COMM_STRUCTS_H_
#define _SRC_COMM_STRUCTS_H_

#include <mbed.h>

#define LocationDataID 0x00
#define OrientationDataID 0x01
#define TemperatureDataID 0x02

struct LocationDataRequest {
  uint8_t commandID = 0xFF;
};

struct LocationDataReply {
  float latitude;
  float longitude;
};

struct OrientationDataRequest {
  uint8_t commandID;
};

struct OrientationDataReply {
  uint16_t roll;
  uint16_t pitch;
  uint16_t yaw;
};

struct TemperatureDataRequest {
  uint8_t commandID;
};

struct TemperatureDataReply {
  int8_t coilXTemp;
  int8_t coilYTemp;
  int8_t coilZTemp;
  int8_t ADCSTemp;
  int8_t GPSXTemp;
};

struct CoilControlDataRequest {
  uint8_t commandID;
};

struct CoilControlDataReply {
  int8_t coilXPWM;
  int8_t coilYPWM;
  int8_t coilZPWM;
};

struct CoilControlDataRequest {
  uint8_t commandID;
};

struct CoilControlDataReply {
  int8_t coilXCurrent;
  int8_t coilYCurrent;
  int8_t coilZCurrent;
};

struct OrientationControlRequest {
  uint8_t  commandID;
  uint16_t roll;
  uint16_t pitch;
  uint16_t yaw;
};

struct OrientationControlReply {
  mbed_error_status_t errorCode;
};

struct EarthManeuverCameraRequest {
  uint8_t commandID;
  int32_t latitude;
  int32_t longitude;
};

struct EarthManeuverCameraReply {
  mbed_error_status_t errorCode;
};

struct EarthManeuverAntennaRequest {
  uint8_t commandID;
  int32_t latitude;
  int32_t longitude;
};

struct EarthManeuverAntennaReply {
  mbed_error_status_t errorCode;
};

struct SpaceManeuverRequest {
  uint8_t commandID;
  int32_t rightAscension;
  int32_t declination;
};

struct spaceManeuverReply {
  mbed_error_status_t errorCode;
};

#endif /* _SRC_COMM_STRUCTS_H_*/