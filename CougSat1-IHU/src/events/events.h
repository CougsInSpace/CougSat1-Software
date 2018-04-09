/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file events.h
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief Constains declaration of all events
 *
 * Each event is a method that is put on the eventQueue and called from there
 */

#ifndef SRC_EVENTS_EVENTS_H_
#define SRC_EVENTS_EVENTS_H_

#include <mbed.h>

//Number of events in the cisEventList array
#define EVENTS_SIZE 9

//Indexes of events in the cisEventList array
#define EVENT_SHUTDOWN 0
#define EVENT_IHU_PERIODIC 1
#define EVENT_TALK_TO_RCS 2
#define EVENT_PMIC_MESSAG 3
#define EVENT_ADCS_MESSAGE 4
#define EVENT_RCS_MESSAGE 5
#define EVENT_IHU_MESSAGE 6
#define EVENT_IFJR_MESSAGE 7
#define EVENT_SEND_TO_GND 8

//Definitions of all events
void shutdown(uint8_t *dataBlock, uint16_t dataLength);
void ihuPeriodic(uint8_t *dataBlock, uint16_t dataLength);
void talkToRCS(uint8_t *dataBlock, uint16_t dataLength);
void pmicMessage(uint8_t *dataBlock, uint16_t dataLength);
void adcsMessage(uint8_t *dataBlock, uint16_t dataLength);
void rcsMessage(uint8_t *dataBlock, uint16_t dataLength);
void ihuMessage(uint8_t *dataBlock, uint16_t dataLength);
void ifjrMessage(uint8_t *dataBlock, uint16_t dataLength);
void sendToGround(uint8_t *dataBlock, uint16_t dataLength);

//Array of pointer to every event
extern void (*cisEventList[EVENTS_SIZE])(uint8_t *dataBlock,
    uint16_t dataLength);

#endif /* SRC_EVENTS_EVENTS_H_ */
