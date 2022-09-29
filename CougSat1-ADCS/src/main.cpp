//#include <CISConsole.h>
#include "stdio.h"
//#include "photodiode.h"
#include "ADCS.h"
#include <mbed.h>
#include <iostream>


/**
 * Program start routine
 * @return error code
 */
int main(void) {
  ADCS jump(.25);
  adcs.startThread();


  //  while(true) {
  //      volttest = test.getVoltages();
  //      std::cout << (int) (100 * test.get_pos_x()) << endl;
  //      std::cout << (int) (100 * test.get_pos_y()) << endl;
  //      std::cout << (int) (100 * test.get_pos_z()) << endl;
  //      std::cout << "testy4" << endl;

  //      //printf("%f,", test.get_pos_x());
  //      //printf("%f,", test.get_pos_y());
  //      //printf("%f", 0.1/*test.get_pos_z()*/);
  //      //printf("test");
  //      printf("\n");
  //      ThisThread::sleep_for(1s);
  //  }
}




// //#include <CISConsole.h>
// #include "ADCS.h"
// #include <mbed.h>


// /**
//  * @brief Initializes the all of the subclasses of the ADCS
//  *
//  * @return mbed_error_status_t
//  */

// /**
//  * Program start routine
//  * @return error code
//  */
// int main(void) {
//   ADCS adcs;
//   adcs.startThread();
// }
