#ifndef _SRC_INTERFACES_CDH_H_
#define _SRC_INTERFACES_CDH_H_

#include <mbed.h>

enum class CDHCommand_t {
  TURN_OFF        = 0x00,
  TURN_ON         = 0x01,
  VOLTAGE_REQ     = 0x02,
  CURRENT_REQ     = 0x03,
  TEMP_REQ        = 0x04,
  POWER_CHAN_STAT = 0x05,
  PV_CHAN_STAT    = 0x06
};

#define CDH_ADDR_PV ((uint8_t)0x00)      // 0x00 to 0x07
#define CDH_ADDR_BATT ((uint8_t)0x08)    // 0x08 to 0x09
#define CDH_ADDR_REG ((uint8_t)0x0A)     // 0x0A to 0x0B
#define CDH_ADDR_PR_3V3 ((uint8_t)0x0C)  // 0x0C to 0x18
#define CDH_ADDR_PR_BATT ((uint8_t)0x19) // 0x19 to 0x1F
#define CDH_ADDR_PV_3V3 ((uint8_t)0x20)  // 0x20 to 0x23
#define CDH_ADDR_BH ((uint8_t)0x24)      // 0x24 to 0x25
#define CDH_ADDR_DEPLOY ((uint8_t)0x26)  // 0x26 only
#define CDH_ADDR_PMIC ((uint8_t)0x27)    // 0x27 only
#define CDH_ADDR_MPPT ((uint8_t)0x28)    // 0x28 to 0x2F
#define CDH_ADDR_PCB ((uint8_t)0x30)     // 0x30 to 0x33

#define CDH_PC_BIT_3V3 (0)     // PowerChannelStatus[25:0]
#define CDH_PC_BIT_BATT (26)   // PowerChannelStatus[39:26]
#define CDH_PC_BIT_REG (40)    // PowerChannelStatus[47:40]
#define CDH_PC_BIT_BH (48)     // PowerChannelStatus[51:48]
#define CDH_PC_BIT_DEPLOY (52) // PowerChannelStatus[53:52]

class CDH {
public:
  CDH(I2CSlave & i2c);

  bool              hasMessage();
  mbed_error_code_t processMessage();

private:
  mbed_error_code_t processMsgPowerChange(char * msgBody);
  mbed_error_code_t processMsgVoltageRequest(char * msgBody);
  mbed_error_code_t processMsgCurrentRequest(char * msgBody);
  mbed_error_code_t processMsgTemperatureRequest(char * msgBody);
  mbed_error_code_t processMsgPowerChannelStatus(char * msgBody);
  mbed_error_code_t processMsgSolarChannelStatus(char * msgBody);

  I2CSlave & i2c;
};

#endif /* _SRC_INTERFACES_CDH_H_ */