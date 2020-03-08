#ifndef TLE_H
#define TLE_H
#include <mbed.h>

struct TLEData {
  // Line 1
  int    satelliteCatalogNumber;
  char   classification;
  int    internationalDesignatorYear;
  int    internationalDesignatorLaunch;
  char   internationalDesignatorLaunchPiece[3];
  int    epochYear;
  double epochDay;
  double firstDerivativeMeanMotion;
  double secondDerivativeMeanMotion;
  double dragTerm;
  int    elementSetNumber;

  // Line 2
  double inclination;
  double rightAscension;
  double eccentricity;
  double perigee;
  double meanAnomaly;
  double meanMotion;
  int    revolutions;
};

bool parseData(char * element, TLEData * data);
#endif /*TLE_H*/
