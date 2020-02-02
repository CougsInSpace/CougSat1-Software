#include "Venus838FLPx.h"

/**
 * @brief Construct a new Venus838FLPx::Venus838FLPx object
 *
 * @param serial connected to the GPS
 * @param reset pin of the GPS
 * @param pulse pin of the GPS
 */
Venus838FLPx::Venus838FLPx(Serial & serial, PinName reset, PinName pulse) :
  serial(serial), reset(reset), pulse(pulse) {}

/**
 * @brief Destroy the Venus838FLPx::Venus838FLPx object
 *
 */
Venus838FLPx::~Venus838FLPx() {}

/**
 * @brief Read a GPS message and update fields
 *
 * @param data struct to return result
 * @param blocking will wait until data is present if true
 * @return mbed_error_status_t
 */
mbed_error_status_t read(GPSData_t & data, bool blocking = true) {
  
  mbed_error_status_t error = MBED_SUCCESS;
  nmeaString[83] = {'\0'};

  for (uint8_t i = 0; i < 6; ++i){
    serial.gets(nmeaString, 83);

    switch (nmeaString[5])
    {
    case 'G':
      error = parseGGA(data, (nmeaString +7));
      break;

      case 'L':
      error = parseGLL(data, (nmeaString +7));
      break;

      case 'S':
      error = parseGSA(data, (nmeaString +7));
      break;
    
      case 'M':
      error = parseRMC(data, (nmeaString +7));
      break;

      case 'T':
      error = parseVTG(data, (nmeaString +7));
      break;

      case 'D':
      error = parseZDA(data, (nmeaString +7));
      break;

    
    default:
      error = INVALID_DATA
      break;
    }
  }
}

  mbed_error_status_t Venus838FLPx::parseGGA(GPSData_t & data, char *nmeaString){
    data.time = atof(strtok(nmeaString,",")) *1000 ;
    data.latitude = atof(strtok(NULL,","));
    if(*strtok(NULL,",") == 'S'){ // if in souther hemisphere save as negative value
       data.latitude *= -1;
    }
    data.longitude = atof(strtok(NULL,","));
    if(*strtok(NULL,",") == 'E'){ // if in eastern hemisphere save as negative value
       data.longitude *= -1;
       return MBED_SUCCESS;
    }
  }
  mbed_error_status_t Venus838FLPx::parseGLL(GPSData_t & data, char *nmeaString){
    
    data.latitude = atof(strtok(nmeaString,","));
    if(*strtok(NULL,",") == 'S'){ // if in souther hemisphere save as negative value
       data.latitude *= -1;
    }
    data.longitude = atof(strtok(NULL,","));
    if(*strtok(NULL,",") == 'E'){ // if in eastern hemisphere save as negative value
       data.longitude *= -1;
    }

    data.time = atof(strtok(NULL,",")) *1000 ;
    return MBED_SUCCESS;
  }
  mbed_error_status_t Venus838FLPx::parseGSA(GPSData_t & data, char *nmeaString){
    return MBED_SUCCESS;
  }
  mbed_error_status_t Venus838FLPx::parseRMC(GPSData_t & data, char *nmeaString){
    data.time = atof(strtok(nmeaString,",")) *1000 ;
    strtok(NULL,",");
    data.latitude = atof(strtok(NULL,","));
    if(*strtok(NULL,",") == 'S'){ // if in souther hemisphere save as negative value
       data.latitude *= -1;
    }
    data.longitude = atof(strtok(NULL,","));
    if(*strtok(NULL,",") == 'E'){ // if in eastern hemisphere save as negative value
       data.longitude *= -1;
    }
    data.groundSpeed = data.longitude = atof(strtok(NULL,","));
    
    return MBED_SUCCESS;
  }
  mbed_error_status_t Venus838FLPx::parseVTG(GPSData_t & data, char *nmeaString){
    return MBED_SUCCESS;
  }
  mbed_error_status_t Venus838FLPx::parseZDA(GPSData_t & data, char *nmeaString){
    return MBED_SUCCESS;
  }