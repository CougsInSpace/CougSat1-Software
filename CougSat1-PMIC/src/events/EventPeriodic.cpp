#include "Events.h"
#include <CISConsole.h>
#include <PMICObjects.h>

double SHED_ALL  = 2.69;
double SHED_HALF = 1.51;
double SHED_SOME = 0.48;

// Shed loads and bus connected to 3V3 Regulators
mbed_error_status_t Load_Shedding3V3(double Current_2_Shed) {
  while (Current_2_Shed > 0) {
    double maxAggregate = 0;
    double Current_Retrieved;
    // 1. Create a PowerNode type pointer
    PowerNode * MaxLoad = nullptr;

    // 2. Loop through nodePR3V3
    for (int i = 0; i < 13; i++) {
      // 3. compare Aggregate value with Max Aggregate value
      if (nodesPR3V3[i]->getAggregatePriority() > maxAggregate) {
        // 4. If value is greater, make it the new maxAggregate
        maxAggregate = nodesPR3V3[i]->getAggregatePriority();
        MaxLoad      = nodesPR3V3[i]; // 5. set PowerNode ptr equal to this node
      }
    }

    // 6. Get Current of this node
    MaxLoad->getCurrent(Current_Retrieved); // Get Current of Max Load
    // 7. Subtract current from Current_2_Shed
    Current_2_Shed -= Current_Retrieved;
    // 8. Turn off this payload
    MaxLoad->setSwitch(false);
  }

  return 0;
}

// Shed loads and bus connected to Batteries
mbed_error_status_t Load_SheddingBatt(double Current_2_Shed) {
  while (Current_2_Shed > 0) {
    double maxAggregate      = 0;
    double Current_Retrieved = 0;
    // 1. Create a PowerNode type pointer
    PowerNode * MaxLoad = nullptr;

    // 2. Loop through nodePR3V3
    for (int i = 0; i < 7; i++) {
      // 3. compare Aggregate value with Max Aggregate value
      if (nodesPRBatt[i]->getAggregatePriority() > maxAggregate) {
        // 4. If value is greater, make it the new maxAggregate
        maxAggregate = nodesPRBatt[i]->getAggregatePriority();
        MaxLoad = nodesPRBatt[i]; // 5. set PowerNode ptr equal to this node
      }
    }

    // 6. Get Current of this node
    MaxLoad->getCurrent(Current_Retrieved); // Get Current of Max Load
    // 7. Subtract current from Current_2_Shed
    Current_2_Shed -= Current_Retrieved;
    // 8. Turn off this payload
    MaxLoad->setSwitch(false);
  }

  return 0;
}

/**
 * @brief Executes the periodic event
 *
 * @return mbed_error_status_t error code
 */
mbed_error_status_t eventPeriodic() {
  // Check each module or peripheral that needs to be monitored
  // if any of the measured values is out of the norm, call load_shedding()
  // function and pass in a current amount to shed
  mbed_error_status_t result = 0;

  /************************* Monitor Batteries ********************/
#ifndef BATT
  double Measured_Batt_Current;
  double Measured_Batt_Temp;
  // Check current output of each battery
  result = nodeBattA.getCurrent(Measured_Batt_Current);
  // Add error Check

  if (Measured_Batt_Current > EXPECTED_BATT_CURRENT)
    result = Load_SheddingBatt((Measured_Batt_Current - EXPECTED_BATT_CURRENT));
  // add error check

  result = nodeBattB.getCurrent(Measured_Batt_Current);
  // add error check

  if (Measured_Batt_Current > EXPECTED_BATT_CURRENT)
    result = Load_SheddingBatt((Measured_Batt_Current - EXPECTED_BATT_CURRENT));
  // add error check

  // Check Temperature of each battery

  // BATTERY A
  result = thermistorBattA.getTemperature(Measured_Batt_Temp);
  // add error check

  if (Measured_Batt_Temp < MIN_BATT_TEMP)
    nodesBatteryHeaters[0]->setSwitch(true);
  else if (Measured_Batt_Temp >= ACCEPTABLE_BATT_TEMP &&
           Measured_Batt_Temp < MAX_BATT_TEMP)
    nodesBatteryHeaters[0]->setSwitch(false);
  else if (Measured_Batt_Temp > MAX_BATT_TEMP) {
    nodesBatteryHeaters[0]->setSwitch(false);

    // check if battery is too hot
    if (Measured_Batt_Temp > OVERHEATED_BATT_TEMP)
      result = Load_SheddingBatt(SHED_ALL);
    // error check?
    else
      result = Load_SheddingBatt(SHED_HALF);
  }

  // BATTERY B
  result = thermistorBattB.getTemperature(Measured_Batt_Temp);
  // add error check

  if (Measured_Batt_Temp < MIN_BATT_TEMP)
    nodesBatteryHeaters[1]->setSwitch(true);
  else if (Measured_Batt_Temp >= ACCEPTABLE_BATT_TEMP &&
           Measured_Batt_Temp < MAX_BATT_TEMP)
    nodesBatteryHeaters[1]->setSwitch(false);
  else if (Measured_Batt_Temp > MAX_BATT_TEMP) {
    nodesBatteryHeaters[1]->setSwitch(false);

    // check if battery is too hot
    if (Measured_Batt_Temp > OVERHEATED_BATT_TEMP)
      result = Load_SheddingBatt(SHED_ALL);
    // error check?
    else
      result = Load_SheddingBatt(SHED_HALF);
  }

#endif // BATT
  /****************************************************************/
  /************************ Monitor Regulators ********************/
#ifndef REGULATOR
  double Measured_REG_Current_IN;
  double Measured_REG_Current_OUT;

  // REGULATOR A
  result = node3V3InA.getCurrent(Measured_REG_Current_IN);
  // Error Check
  result = node3V3OutA.getCurrent(Measured_REG_Current_OUT);
  // Error Check

  if (Measured_REG_Current_IN > EXPECTED_REG_CURRENT)
    result = Load_Shedding3V3((Measured_REG_Current_IN - EXPECTED_REG_CURRENT));
  // Error Check

  if (Measured_REG_Current_OUT > EXPECTED_REG_CURRENT)
    result = Load_Shedding3V3(Measured_REG_Current_OUT - EXPECTED_REG_CURRENT);
  // Error Check

  // Regulator B
  result = node3V3InB.getCurrent(Measured_REG_Current_IN);
  // Error Check
  result = node3V3OutB.getCurrent(Measured_REG_Current_OUT);
  // Error Check

  if (Measured_REG_Current_IN > EXPECTED_REG_CURRENT)
    result = Load_Shedding3V3((Measured_REG_Current_IN - EXPECTED_REG_CURRENT));
  // Error Check

  if (Measured_REG_Current_OUT > EXPECTED_REG_CURRENT)
    result = Load_Shedding3V3(Measured_REG_Current_OUT - EXPECTED_REG_CURRENT);
  // Error Check

  // Check Temperatures of Regulators
  double Measured_REG_TEMP;

  // REGULATOR A
  result = thermistorRegA.getTemperature(Measured_REG_TEMP);
  // Error Check

  if (Measured_REG_TEMP > MAX_3V3_REG_TEMP)
    result = Load_Shedding3V3(SHED_HALF);
  // Error Check

  // REGULATOR B
  result = thermistorRegB.getTemperature(Measured_REG_TEMP);
  // ERROR CHECK

  if (Measured_REG_TEMP > MAX_3V3_REG_TEMP)
    result = Load_Shedding3V3(SHED_HALF);
    // Error Check

#endif // !REGULATOR
       /****************************************************************/
       /*********************** Monitor Payloads ***********************/
#ifndef PAYLOADS
  double Total_BATT_Load_Current = 0;
  double Total_REG_Load_Current  = 0;
  double measured_current        = 0;
  double toShed                  = 0;

  // 1. Add up the current that is being drawn by each payload for REG and Batt
  // 2. Afterwards, check if total current exceeds expected total current
  // 3. If true, subtract expected current from total measured current
  //    a. pass this new value into load_shedding()
  // 4.   If false, move on

  /// Check nodePR3V3
  nodesPR3V3[0]->getCurrent(measured_current);
  Total_REG_Load_Current += measured_current;

  nodesPR3V3[1]->getCurrent(measured_current);
  Total_REG_Load_Current += measured_current;

  nodesPR3V3[2]->getCurrent(measured_current);
  Total_REG_Load_Current += measured_current;

  nodesPR3V3[3]->getCurrent(measured_current);
  Total_REG_Load_Current += measured_current;

  nodesPR3V3[4]->getCurrent(measured_current);
  Total_REG_Load_Current += measured_current;

  nodesPR3V3[5]->getCurrent(measured_current);
  Total_REG_Load_Current += measured_current;

  nodesPR3V3[6]->getCurrent(measured_current);
  Total_REG_Load_Current += measured_current;

  nodesPR3V3[7]->getCurrent(measured_current);
  Total_REG_Load_Current += measured_current;

  nodesPR3V3[8]->getCurrent(measured_current);
  Total_REG_Load_Current += measured_current;

  nodesPR3V3[9]->getCurrent(measured_current);
  Total_REG_Load_Current += measured_current;

  nodesPR3V3[10]->getCurrent(measured_current);
  Total_REG_Load_Current += measured_current;

  nodesPR3V3[11]->getCurrent(measured_current);
  Total_REG_Load_Current += measured_current;

  nodesPR3V3[12]->getCurrent(measured_current);
  Total_REG_Load_Current += measured_current;

  if (Total_REG_Load_Current > PR3V3_TOTAL_CURRENT) {
    toShed = Total_REG_Load_Current - PR3V3_TOTAL_CURRENT;
    Load_Shedding3V3(toShed);
  }

  // PRBATT
  nodesPRBatt[0]->getCurrent(measured_current);
  Total_BATT_Load_Current += measured_current;

  nodesPRBatt[5]->getCurrent(measured_current);
  Total_BATT_Load_Current += measured_current;

  nodesPRBatt[6]->getCurrent(measured_current);
  Total_BATT_Load_Current += measured_current;
  if (Total_REG_Load_Current > PRBATT_TOTAL_CURRENT) {
    toShed = Total_REG_Load_Current - PRBATT_TOTAL_CURRENT;
    Load_Shedding3V3(toShed);
  }

#endif // !PAYLOADS
  /****************************************************************/
}