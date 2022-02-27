#include "CDH.h"

#include <CISConsole.h>
#include "PMICObjects.h"

/**
 * @brief Construct a new CDH::CDH object
 *
 * @param i2c connected to the C&DH
 */
CDH::CDH(I2CSlave & i2c) : i2c(i2c) {}

/**
 * @brief Checks if a message is available to read
 *
 * @return true is the PMIC is addressed
 */
bool CDH::hasMessage() {
  int state = i2c.receive();
  return state == I2CSlave::ReadAddressed || state == I2CSlave::WriteAddressed;
}

/**
 * @brief Process the message coming from the C&DH
 *
 * @return mbed_error_code_t error code
 */
mbed_error_status_t CDH::processMessage() {
  // Get message
  int state = i2c.receive();
  uint8_t result;

  // Switch on commandID and fire off the proper function
  switch (state) {
    case I2CSlave::ReadAddressed:
      // Write back the buffer from the master
      result = i2c.write(buf, bufferSize);
      if (result != 0)    // not all data written
        return MBED_ERROR_WRITE_FAILED;
      
      LOG("CDH", "Written to master (addressed): %s\n", buf);
      return MBED_SUCCESS;

    case I2CSlave::WriteAddressed:
      CDHCommand_t identifier;
      char *writtenBuffer = nullptr, *message = nullptr;

      // Read data
      result = i2c.read(writtenBuffer, 1);
      if (result != 0)    // no data read
        return MBED_ERROR_READ_FAILED;
      
      identifier = (CDHCommand_t) *writtenBuffer;
      result = i2c.read(message, 1);
      LOG("CDH", "Read from master (addressed): %s\n", writtenBuffer);

      // Switch on identifier and send message to correct function
      switch (identifier) {
        case CDHCommand_t::TURN_OFF:
          return processMsgPowerChange(message, false);
        case CDHCommand_t::TURN_ON:
          return processMsgPowerChange(message, true);
        case CDHCommand_t::VOLTAGE_REQ:
          return processMsgVoltageRequest(message);
        case CDHCommand_t::CURRENT_REQ:
          return processMsgCurrentRequest(message);
        case CDHCommand_t::TEMP_REQ:
          return processMsgTemperatureRequest(message);
        case CDHCommand_t::POWER_CHAN_STAT:
          return processMsgPowerChannelStatus();
        case CDHCommand_t::PV_CHAN_STAT:
          return processMsgSolarChannelStatus();
      }
      break;
  }

  return MBED_ERROR_UNSUPPORTED;
}

/**
 * @brief Process the power change message
 *
 * @param msgBody array to the incoming message body
 * @return mbed_error_code_t error code
 */
mbed_error_status_t CDH::processMsgPowerChange(char * msgBody, bool on) {
  // Get message in byte form
  uint8_t message = *msgBody;

  // Set buffer variables
  bufferSize = 1;
  *buf = 0x00;

  // Switch possible messages
  switch (message) {
    case 0x00:  // IHU
      (*iNodesPR3V3[0]).setSwitch(on);
      *buf = 0x01;
      break;
    case 0x01:  // IFJR
      (*iNodesPR3V3[5]).setSwitch(on);
      *buf = 0x01;
      break;
    case 0x02:  // ADCS
      (*iNodesPR3V3[2]).setSwitch(on);
      *buf = 0x01;
      break;
    case 0x03:  // ADCS Coils
      (*iNodesPRBatt[5]).setSwitch(on);
      *buf = 0x01;
      break;
    case 0x04:  // Comms
      (*iNodesPR3V3[6]).setSwitch(on);
      *buf = 0x01;
      break;
    case 0x05:  // Comms Amplifier
      (*iNodesPRBatt[6]).setSwitch(on);
      *buf = 0x01;
      break;
    case 0x06:  // Payload 0
      (*iNodesPR3V3[7]).setSwitch(on);
      *buf = 0x01;
      break;
    case 0x07:  // Payload 1
      (*iNodesPR3V3[8]).setSwitch(on);
      *buf = 0x01;
      break;
    case 0x08:  // Payload 2
      (*iNodesPR3V3[9]).setSwitch(on);
      *buf = 0x01;
      break;
  }
  
  if (*buf == 0x01) {
    return MBED_SUCCESS;
  }

  return MBED_ERROR_UNSUPPORTED;
}

/**
 * @brief Process the voltage request message
 *
 * @param msgBody array to the incoming message body
 * @return mbed_error_code_t error code
 */
mbed_error_status_t CDH::processMsgVoltageRequest(char * msgBody) {
  // Get message in byte form
  uint8_t message = *msgBody;
  double voltage = 0.0;
  mbed_error_status_t error = MBED_ERROR_UNSUPPORTED;

  // Switch possible messages
  switch (message) {
    case 0x00:  // Solar Panel 0A
      error = (*vNodesPV[0]).updateAndGet(voltage);
      break;
    case 0x01:  // Solar Panel 0B
      error = (*vNodesPV[1]).updateAndGet(voltage);
      break;
    case 0x02:  // Solar Panel 1A
      error = (*vNodesPV[2]).updateAndGet(voltage);
      break;
    case 0x03:  // Solar Panel 1B
      error = (*vNodesPV[3]).updateAndGet(voltage);
      break;
    case 0x04:  // Solar Panel 2A
      error = (*vNodesPV[4]).updateAndGet(voltage);
      break;
    case 0x05:  // Solar Panel 2B
      error = (*vNodesPV[5]).updateAndGet(voltage);
      break;
    case 0x06:  // Solar Panel 3A
      error = (*vNodesPV[6]).updateAndGet(voltage);
      break;
    case 0x07:  // Solar Panel 3B
      error = (*vNodesPV[7]).updateAndGet(voltage);
      break;
    case 0x08:  // Battery A
      error = vNodeBattA.updateAndGet(voltage);
      break;
    case 0x09:  // Battery B
      error = vNodeBattB.updateAndGet(voltage);
      break;
    case 0x0A:  // 3.3V Regulator A
      error = vNode3V3.updateAndGet(voltage);
      break;
  }

  // Convert voltage
  uint16_t reply = voltage * (double)150 * 0.000001;

  // Set buffer
  bufferSize = 2;
  *buf = reply;

  return error;
}

/**
 * @brief Process the current request message
 *
 * @param msgBody array to the incoming message body
 * @return mbed_error_code_t error code
 */
mbed_error_status_t CDH::processMsgCurrentRequest(char * msgBody) {
  // Get message in byte form
  uint8_t message = *msgBody;
  double current = 0.0;
  mbed_error_status_t error = MBED_ERROR_UNSUPPORTED;

  // For case statements
  double currentBattAIn = 0.0, currentBattAOut = 0.0;
  double currentBattBIn = 0.0, currentBattBOut = 0.0;

  // Switch possible messages
  switch (message) {
    case 0x00:  // Solar Panel 0A
      error = (*iNodesPV[0]).updateAndGet(current);
      break;
    case 0x01:  // Solar Panel 0B
      error = (*iNodesPV[1]).updateAndGet(current);
      break;
    case 0x02:  // Solar Panel 1A
      error = (*iNodesPV[2]).updateAndGet(current);
      break;
    case 0x03:  // Solar Panel 1B
      error = (*iNodesPV[3]).updateAndGet(current);
      break;
    case 0x04:  // Solar Panel 2A
      error = (*iNodesPV[4]).updateAndGet(current);
      break;
    case 0x05:  // Solar Panel 2B
      error = (*iNodesPV[5]).updateAndGet(current);
      break;
    case 0x06:  // Solar Panel 3A
      error = (*iNodesPV[6]).updateAndGet(current);
      break;
    case 0x07:  // Solar Panel 3B
      error = (*iNodesPV[7]).updateAndGet(current);
      break;
    case 0x08:  // Battery A
      currentBattAIn = 0.0;
      currentBattAOut = 0.0;
      error = iNodeBattInA.updateAndGet(currentBattAIn);
      if (error != MBED_SUCCESS)
        break;
      error = iNodeBattOutA.updateAndGet(currentBattAOut);
      current = currentBattAOut - currentBattAIn;
      break;
    case 0x09:  // Battery B
      currentBattBIn = 0.0;
      currentBattBOut = 0.0;
      error = iNodeBattInB.updateAndGet(currentBattBIn);
      if (error != MBED_SUCCESS)
        break;
      error = iNodeBattOutB.updateAndGet(currentBattBOut);
      current = currentBattBOut - currentBattBIn;
      break;
    case 0x0A:  // 3.3V Regulator
      error = iNode3V3Out.updateAndGet(current);
      break;
    case 0x0C:  // PR_3.3V-0
      error = (*iNodesPR3V3[0]).updateAndGet(current);
      break;
    case 0x0D:  // PR_3.3V-1
      error = (*iNodesPR3V3[1]).updateAndGet(current);
      break;
    case 0x0E:  // PR_3.3V-2
      error = (*iNodesPR3V3[2]).updateAndGet(current);
      break;
    case 0x0F:  // PR_3.3V-3
      error = (*iNodesPR3V3[3]).updateAndGet(current);
      break;
    case 0x10:  // PR_3.3V-4
      error = (*iNodesPR3V3[4]).updateAndGet(current);
      break;
    case 0x11:  // PR_3.3V-5
      error = (*iNodesPR3V3[5]).updateAndGet(current);
      break;
    case 0x12:  // PR_3.3V-6
      error = (*iNodesPR3V3[6]).updateAndGet(current);
      break;
    case 0x13:  // PR_3.3V-7
      error = (*iNodesPR3V3[7]).updateAndGet(current);
      break;
    case 0x14:  // PR_3.3V-8
      error = (*iNodesPR3V3[8]).updateAndGet(current);
      break;
    case 0x15:  // PR_3.3V-9
      error = (*iNodesPR3V3[9]).updateAndGet(current);
      break;
    case 0x16:  // PR_3.3V-10
      error = (*iNodesPR3V3[10]).updateAndGet(current);
      break;
    case 0x17:  // PR_3.3V-11
      error = (*iNodesPR3V3[11]).updateAndGet(current);
      break;
    case 0x18:  // PR_3.3V-12
      error = (*iNodesPR3V3[12]).updateAndGet(current);
      break;
    case 0x19:  // PR_BATT-0
      error = (*iNodesPRBatt[0]).updateAndGet(current);
      break;
    case 0x1A:  // PR_BATT-1
      error = (*iNodesPRBatt[1]).updateAndGet(current);
      break;
    case 0x1B:  // PR_BATT-2
      error = (*iNodesPRBatt[2]).updateAndGet(current);
      break;
    case 0x1C:  // PR_BATT-3
      error = (*iNodesPRBatt[3]).updateAndGet(current);
      break;
    case 0x1D:  // PR_BATT-4
      error = (*iNodesPRBatt[4]).updateAndGet(current);
      break;
    case 0x1E:  // PR_BATT-5
      error = (*iNodesPRBatt[5]).updateAndGet(current);
      break;
    case 0x1F:  // PR_BATT-6
      error = (*iNodesPRBatt[6]).updateAndGet(current);
      break;
    case 0x20:  // PV_3.3V-0
      //?
      break;
    case 0x21:  // PV_3.3V-1
      break;
    case 0x22:  // PV_3.3V-2
      break;
    case 0x23:  // PV_3.3V-3
      break;
    case 0x24:  // PR_BH-0
      error = (*iNodesBatteryHeaters[0]).updateAndGet(current);
      break;
    case 0x25:  // PR_BH-1
      error = (*iNodesBatteryHeaters[1]).updateAndGet(current);
      break;
    case 0x26:  // PR_DEPLOY-1
      error = (*iNodesDeployables[0]).updateAndGet(current);
      break;
    case 0x27:  // PR_DEPLOY-2
      error = (*iNodesDeployables[1]).updateAndGet(current);
      break;
    case 0x28:  // EPS
      error = iNodePR3V3_EPS.updateAndGet(current);
      break;
    case 0x29:  // MPPT 0
      error = (*iNodesPVIn[0]).updateAndGet(current);
      break;
    case 0x2A:  // MPPT 1
      error = (*iNodesPVIn[1]).updateAndGet(current);
      break;
    case 0x2B:  // MPPT 2
      error = (*iNodesPVIn[2]).updateAndGet(current);
      break;
    case 0x2C:  // MPPT 3
      error = (*iNodesPVIn[3]).updateAndGet(current);
      break;
    case 0x2D:  // MPPT 4
      error = (*iNodesPVIn[4]).updateAndGet(current);
      break;
    case 0x2E:  // MPPT 5
      error = (*iNodesPVIn[5]).updateAndGet(current);
      break;
    case 0x2F:  // MPPT 6
      error = (*iNodesPVIn[6]).updateAndGet(current);
      break;
    case 0x30:  // MPPT 7
      error = (*iNodesPVIn[7]).updateAndGet(current);
      break;
  }

  // Convert current
  int16_t reply = current * (double)150 * 0.000001;

  // Set buffer
  bufferSize = 2;
  *buf = reply;

  return error;
}

/**
 * @brief Process the temperature request message
 *
 * @param msgBody array to the incoming message body
 * @return mbed_error_code_t error code
 */
mbed_error_status_t CDH::processMsgTemperatureRequest(char * msgBody) {
  // Get message in byte form
  uint8_t message = *msgBody;
  double temp = 0.0;
  mbed_error_status_t error = MBED_ERROR_UNSUPPORTED;

  // Switch possible messages
  switch (message) {
    case 0x08:  // Battery A
      error = thermistorBattA.get(temp);
      break;
    case 0x09:  // Battery B
      error = thermistorBattB.get(temp);
      break;
    case 0x0A:  // 3.3V Regulator A
      error = thermistorRegA.get(temp);
      break;
    case 0x0B:  // 3.3V Regulator B
      error = thermistorRegB.get(temp);
      break;
    case 0x27:  // PMIC
      error = thermistorPMIC.get(temp);
      break;
    case 0x29:  // MPPT 0
      error = (*thermistorsMPPT[0]).get(temp);
      break;
    case 0x2A:  // MPPT 1
      error = (*thermistorsMPPT[1]).get(temp);
      break;
    case 0x2B:  // MPPT 2
      error = (*thermistorsMPPT[2]).get(temp);
      break;
    case 0x2C:  // MPPT 3
      error = (*thermistorsMPPT[3]).get(temp);
      break;
    case 0x2D:  // MPPT 4
      error = (*thermistorsMPPT[4]).get(temp);
      break;
    case 0x2E:  // MPPT 5
      error = (*thermistorsMPPT[5]).get(temp);
      break;
    case 0x2F:  // MPPT 6
      error = (*thermistorsMPPT[6]).get(temp);
      break;
    case 0x30:  // MPPT 7
      error = (*thermistorsMPPT[7]).get(temp);
      break;
    case 0x31:  // PCB -X -Y
      error = (*thermistorsEPS[9]).get(temp);
      break;
    case 0x32:  // PCB -X +Y
      error = (*thermistorsEPS[10]).get(temp);
      break;
    case 0x33:  // PCB +X -Y
      error = (*thermistorsEPS[11]).get(temp);
      break;
    case 0x34:  // PCB +X +Y
      error = (*thermistorsEPS[12]).get(temp);
      break;
  }

  // Convert temperature
  int8_t reply = (int8_t)temp;

  // Set buffer
  bufferSize = 1;
  *buf = reply;

  return error;
}

/**
 * @brief Process the power channel status message
 *
 * @param msgBody array to the incoming message body
 * @return mbed_error_code_t error code
 */
mbed_error_status_t CDH::processMsgPowerChannelStatus() {
  // Set buffer variables
  bufferSize = 7;
  *buf = 0x00000000000000;
  int64_t temp = 0;
  bool on = false;
  
  // bit manipulate all the values into place

  // Inodes, getSwitch();

  for (int i = 0; i < 28; i++) {                         // Only 37 ? not the 56 requested
    temp = 0;
    // get status
    switch (i) 
    {
      case 0:
        on = (*iNodesPR3V3[0]).getSwitch();
        break;
      case 1:
        on = (*iNodesPR3V3[1]).getSwitch();
        break;
      case 2:
        on = (*iNodesPR3V3[2]).getSwitch();
        break;
      case 3:
        on = (*iNodesPR3V3[3]).getSwitch();
        break;
      case 4:
        on = (*iNodesPR3V3[4]).getSwitch();
        break;
      case 5:
        on = (*iNodesPR3V3[5]).getSwitch();
        break;
      case 6:
        on = (*iNodesPR3V3[6]).getSwitch();
        break;
      case 7:
        on = (*iNodesPR3V3[7]).getSwitch();
        break;
      case 8:
        on = (*iNodesPR3V3[8]).getSwitch();
        break;
      case 9:
        on = (*iNodesPR3V3[9]).getSwitch();
        break;
      case 10:
        on = (*iNodesPR3V3[10]).getSwitch();
        break;
      case 11:
        on = (*iNodesPR3V3[11]).getSwitch();
        break;
      case 12:
        on = (*iNodesPR3V3[12]).getSwitch();
        break;
      case 13:
        on = (*iNodesPRBatt[0]).getSwitch();
        break;
      case 14:
        on = (*iNodesPRBatt[1]).getSwitch();
        break;
      case 15:
        on = (*iNodesPRBatt[2]).getSwitch();
        break;
      case 16:
        on = (*iNodesPRBatt[3]).getSwitch();
        break;
      case 17:
        on = (*iNodesPRBatt[4]).getSwitch();
        break;
      case 18:
        on = (*iNodesPRBatt[5]).getSwitch();
        break;
      case 19:
        on = (*iNodesPRBatt[6]).getSwitch();
        break;
      case 20://
        //?? on = (*iNodesPV3V3[0]).getSwitch();
        break;
      case 21://
        break;
      case 22://
        break;
      case 23://
        break;
      case 24:
        //on = iNodeBH_0.getSwitch();
        break;
      case 25:
        //on = iNodeBH_1.getSwitch();
        break;
      case 26:
        //on = iNodeDeploy_0.getSwitch();
        break;
      case 27:
        //on = iNodeDeploy_1.getSwitch();
        break;
    }
    
    if (on)
      temp = 1;
    // bit manipulate into the correct position
    temp = temp << i;
    // OR onto the buffer
    *buf = *buf | temp;    
  }

  return MBED_SUCCESS;
}

/**
 * @brief Process the solar panel channel status message
 *
 * @param msgBody array to the incoming message body
 * @return mbed_error_code_t error code
 */
mbed_error_status_t CDH::processMsgSolarChannelStatus() {
  // Set buffer variables
  bufferSize = 2;
  *buf = 0x0000;
  int16_t temp = 0;
  bool on = false;

  // Bit manipulate all the values into place
  for (int i = 0; i < 8; i++) {                         // Only 8 ? not the 16 requested
    temp = 0;
    // get status
    on = (*iNodesPVIn[i]).getSwitch();
    if (on)
      temp = 1;
    // bit manipulate into the correct position
    temp = temp << i;
    // OR onto the buffer
    *buf = *buf | temp;    
  }

  return MBED_SUCCESS;
}