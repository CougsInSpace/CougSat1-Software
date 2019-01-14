/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file EventADC.cpp
 * @author Bradley Davis
 * @date 7 Nov 2018
 * @brief Event to handle updating the ADCs
 *
 */
#include "Events.h"
#include "tools/CISConsole.h"
#include "tools/CISError.h"
#include "PMICObjects.h"

/**
 * @brief Reads the next channel in each ADC queue
 *
 * @return uint8_t error code
 */
uint8_t eventADC() {
  uint8_t result = ERROR_SUCCESS;
  result = adcEPS0.readNextActiveChannel();
  if(result != ERROR_SUCCESS){
    DEBUG("Event-ADC", "Failed to read next active channel of EPS0: 0x%02X", result);
    return result;
  }
  
  result = adcEPS1.readNextActiveChannel();
  if(result != ERROR_SUCCESS){
    DEBUG("Event-ADC", "Failed to read next active channel of EPS1: 0x%02X", result);
    return result;
  }

  result = adcEPS2.readNextActiveChannel();
  if(result != ERROR_SUCCESS){
    DEBUG("Event-ADC", "Failed to read next active channel of EPS2: 0x%02X", result);
    return result;
  }
  
  result = adcEPS3.readNextActiveChannel();
  if(result != ERROR_SUCCESS){
    DEBUG("Event-ADC", "Failed to read next active channel of EPS3: 0x%02X", result);
    return result;
  }
  
  result = adcEPS4.readNextActiveChannel();
  if(result != ERROR_SUCCESS){
    DEBUG("Event-ADC", "Failed to read next active channel of EPS4: 0x%02X", result);
    return result;
  }
  
  result = adcEPS5.readNextActiveChannel();
  if(result != ERROR_SUCCESS){
    DEBUG("Event-ADC", "Failed to read next active channel of EPS5: 0x%02X", result);
    return result;
  }
  
  result = adcPV0.readNextActiveChannel();
  if(result != ERROR_SUCCESS){
    DEBUG("Event-ADC", "Failed to read next active channel of PV0: 0x%02X", result);
    return result;
  }
  
  result = adcPV1.readNextActiveChannel();
  if(result != ERROR_SUCCESS){
    DEBUG("Event-ADC", "Failed to read next active channel of PV1: 0x%02X", result);
    return result;
  }
  
  result = adcPV2.readNextActiveChannel();
  if(result != ERROR_SUCCESS){
    DEBUG("Event-ADC", "Failed to read next active channel of PV2: 0x%02X", result);
    return result;
  }
  
  result = adcPV3.readNextActiveChannel();
  if(result != ERROR_SUCCESS){
    DEBUG("Event-ADC", "Failed to read next active channel of PV3: 0x%02X", result);
    return result;
  }

  return result;
}