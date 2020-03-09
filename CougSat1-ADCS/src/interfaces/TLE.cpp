#include "TLE.h"

/**
 * @brief
 *
 * @param element
 * @param data
 * @return true
 * @return false if the string is too short or the checksum fails
 */
int parseData(char * element, TLEData * data) {
  int  line1Checksum = 0;
  int  line2Checksum = 0;
  char temp[16]      = "";
  char exponent[16]  = "";

  if (strlen(element) != 138) {
    return 1;
  }

  for (int i = 0; i < 68; i++) {
    if (element[i] == '-') {
      line1Checksum++;
    } else {
      char temp[2];
      temp[0] = element[i];
      temp[1] = '\0';
      line1Checksum += atoi(temp);
    }
  }
  line1Checksum = line1Checksum % 10;

  for (int i = 69; i < 137; i++) {
    if (element[i] == '-') {
      line2Checksum++;
    } else {
      char temp[2];
      temp[0] = element[i];
      temp[1] = '\0';
      line2Checksum += atoi(temp);
    }
  }
  line2Checksum = line2Checksum % 10;

  if (line1Checksum != int(element[68] - '0') ||
      line2Checksum != int(element[137] - '0')) {
    return 2;
  }
  // Line 1
  // Satellite catalog number
  std::copy(element + 2, element + 7, temp);
  temp[5]                      = '\0';
  data->satelliteCatalogNumber = atoi(temp);

  // Classification
  data->classification = element[7];

  // International Designator (last two digits of launch year)
  std::copy(element + 9, element + 11, temp);
  temp[2]                           = '\0';
  data->internationalDesignatorYear = atoi(temp);

  // International Designator (launch number of the year)
  std::copy(element + 11, element + 14, temp);
  temp[3]                             = '\0';
  data->internationalDesignatorLaunch = atoi(temp);

  // International Designator (piece of the launch)
  std::copy(element + 14, element + 17, temp);
  temp[3] = '\0';
  strncpy(data->internationalDesignatorLaunchPiece, temp, 3);

  // Epoch Year (last two digits of year)
  std::copy(element + 18, element + 20, temp);
  temp[2]         = '\0';
  data->epochYear = atoi(temp);

  // Epoch (day of the year and fractional portion of the day)
  std::copy(element + 20, element + 32, temp);
  temp[12]       = '\0';
  data->epochDay = atof(temp);

  // First Derivative of Mean Motion aka the Ballistic Coefficient
  std::copy(element + 33, element + 43, temp);
  temp[10]                        = '\0';
  data->firstDerivativeMeanMotion = atof(temp);

  // Second Derivative of Mean Motion (decimal point assumed)
  temp[1] = '.';
  temp[0] = element[44];
  temp[2] = element[45];
  temp[3] = element[46];
  temp[4] = element[47];
  temp[5] = element[48];
  temp[6] = element[49];
  temp[7] = '\0';
  std::copy(element + 50, element + 52, exponent);
  exponent[2]                      = '\0';
  data->secondDerivativeMeanMotion = ((atof(temp)) * pow(10, atof(exponent)));

  // Drag Term aka Radiation Pressure Coefficient or BSTAR (decimal point
  // assumed)
  temp[1] = '.';
  temp[0] = element[53];
  temp[2] = element[54];
  temp[3] = element[55];
  temp[4] = element[56];
  temp[5] = element[57];
  temp[6] = element[58];
  temp[7] = '\0';
  std::copy(element + 59, element + 61, exponent);
  exponent[2]    = '\0';
  data->dragTerm = ((atof(temp)) * pow(10, atof(exponent)));

  // Element set number. Incremented when a new TLE is generated for this
  // object.
  std::copy(element + 64, element + 68, temp);
  temp[4]                = '\0';
  data->elementSetNumber = atoi(temp);

  // Line 2
  // Inclination (degrees)
  std::copy(element + 77, element + 85, temp);
  temp[8]           = '\0';
  data->inclination = atof(temp);

  // Right Ascension of the Ascending Node (degrees)
  std::copy(element + 86, element + 94, temp);
  temp[8]              = '\0';
  data->rightAscension = atof(temp);

  // Eccentricity (decimal point assumed)
  temp[0]            = '.';
  temp[1]            = element[95];
  temp[2]            = element[96];
  temp[3]            = element[97];
  temp[4]            = element[98];
  temp[5]            = element[99];
  temp[6]            = element[100];
  temp[7]            = element[101];
  temp[8]            = '\0';
  data->eccentricity = atof(temp);

  // Argument of Perigee (degrees)
  std::copy(element + 103, element + 111, temp);
  temp[8]       = '\0';
  data->perigee = atof(temp);

  // Mean Anomaly (degrees)
  std::copy(element + 112, element + 120, temp);
  temp[8]           = '\0';
  data->meanAnomaly = atof(temp);

  // Mean Motion (revolutions per day)
  std::copy(element + 121, element + 132, temp);
  temp[11]         = '\0';
  data->meanMotion = atof(temp);

  // Revolution number at epoch (revolutions)
  std::copy(element + 132, element + 137, temp);
  temp[5]           = '\0';
  data->revolutions = atoi(temp);

  return 0;
}