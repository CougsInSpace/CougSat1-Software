/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file EventPeriodic.cpp
 * @author Bradley Davis
 * @date 7 Nov 2018
 * @brief Event to handle the periodic functions
 *
 */
#include "Events.h"
#include "tools/CISConsole.h"
#include "tools/CISError.h"


/**
 * @brief Executes the periodic event
 *
 * @return uint8_t error code
 */
uint8_t eventPeriodic()
{

	// From powerrail psuedocode, implement control algorithm
	// Control input switching to power

	// Monitor battery
	long BattVoltA = 0; // voltage level of battery A
	long BattVoltB = 0; // Voltage level of battery B
	long BattCurrA = 0; // Current leaving Battery A
	long BattCurrB = 0; // Current Leaving Battery B
	long PLCurr = 0; // Temp variable that will hold a current level value
	long temp1 = 0; // Temperature of Battery A
	long temp2 = 0; // Temperature of Battery B
	int Pnum = 0;
	int i = 13;

	int index = 0;
	uint8_t result;

	/**************************** Monitor voltages, current, and Temperature of the batteries*********/
	
	
	/*******************************************
	 * Monitor voltage levels of batteries.    *
	 * Use nodeVBattA and nodeVBattB to get    *
	 * access to getVoltage() to get voltage   *
	 * readings. Balance voltage levels of the *
	 * batteries so that each battery has the  *
	 * same voltage level. 
	 ****************************************/
	result = nodeVBattA.getVoltage(&BattVoltA);
	if (result != ERROR_SUCCESS)
	{
		ERROR("Periodic", "Failed to get voltage of Battery A");
		return result;
	}
	
	result = nodeVBattB.getVoltage(&BattVoltB);
	if (result != ERROR_SUCCESS)
	{
		ERROR("Periodic", "Failed to get voltage of Battery B");
		return result;
	}

	if (BattVoltA > BattVoltB)
	{
		// if voltage of battery A is greater than the voltage of battery of B, redirect current to voltage B
		// interate through nodesPVOut[] and set each one to go to battery B

		while (nodesPVOut[index] < 8)
		{
			nodesPVOut[index].setSwitch(false, true); // turn pathA off and pathB on
			index++;
		}
	}
	else if (BattVoltB > BattVoltA)
	{
		// else if voltage of battery B is greater than the voltage of battery of A, redirect current to voltage A
		while (nodesPVOut[index] < 8)
		{
			nodesPVOut[index].setSwitch(true, false); // turn pathA on and pathB off
			index++;
		}
	}
	else
	{
		// else both batteries are balanced and redirect both 
		while (nodesPVOut[index] < 8)
		{
			nodesPVOut[index].setSwitch(true, true); // turn pathA on and pathB on
			index++;
		}
	}

	
	/*******************************************
	 * Monitor Current of the Batteries 	   *
	 * Use NodeBattA and NodeBattB to access   *
	 * getCurrent() to get battery current     *
	 * readings. If current is above 4A,       *
	 * reduce payloads until current drops     *
	 * below 4A or equals 4A            	   *
	 ****************************************/
	double BattPR;
	double RegPR;
	int priority1 = 0;
	int priority2 = 0;
	float X1 = 0; X2 = 0;
	int x_index = 0; // index for PR3V3
	int battI = 0; // index for PRBatt[]
	result = nodeBattA.getCurrent(&BattCurrA);
	if (result != ERROR_SUCCESS)
	{
		ERROR("Periodic", "Failed to get current exiting Batttery A");
		return result;
	}

	result = nodeBattB.getCurrent(&BattCurrB);
	if (result != ERROR_SUCCESS)
	{
		ERROR("Periodic", "Failed to get current exiting Battery A");
		return result;
	}


	// monitor current and reduce payload distribution
	// by turing off payloads that draw current 
	if (BattCurrA > 4 && BattCurrB > 4)
	{
			for(int i = 0; i < 12; i++)
			{
				// iterate through payload arrays
				// use sizeof nodesPRBatt[] as a condition
				// to make sure there is not out of bounds error
				if(i < 7)
				{
					// get current and priority number
					// of each payload
					nodesPR3V3[i].getcurrent(&RegPR);
					nodesPR3V3[i].getPriority(&priority1);

					nodesPRBatt[i].getCurrent(&BattPR);
					nodesPRBatt[i].getPriority(&priority2);

					// calculate x Value
					X1 = (priority1 * RegPR);     
					X2 = (priority2 * priority2); 

					if(X1 < X2)
						nodesPR3V3[i].setSwitch(false, false);
					else
						nodesPRBatt[i].setSwitch(false,false);
									
				}
				else
				{
					nodesPR3V3[i].getcurrent(&RegPR);
					nodesPR3V3[i].getPriority(&priority1);
					X1 = (priority1 * RegPR); 
					if( (i+1) != 12)
					{		
						nodesPR3V3[i + 1].getcurrent(&RegPR);
						nodesPR3V3[i + 1].getPriority(&priority1);				
						X2 = (priority1 * RegPR);// calculate x Vaule

						// if X1 is greater than X2, save the index value
						// else save index value of payload 2
						if(X1 > X2)
							x_index = i;
						else
							x_index = i+1;
					}
					nodesPV3V3[x_index].setSwitch(false,false);
				}
			}
	}
	else if (BattCurrA > 4)
	{
		for(int i = 0; i < 12; i++)
			{
				// iterate through payload arrays
				// use sizeof nodesPRBatt[] as a condition
				// to make sure there is not out of bounds error
				if(i < 7)
				{
					// get current and priority number
					// of each payload
					nodesPR3V3[i].getcurrent(&RegPR);
					nodesPR3V3[i].getPriority(&priority1);

					nodesPRBatt[i].getCurrent(&BattPR);
					nodesPRBatt[i].getPriority(&priority2);
					
					// calculate x Value
					X1 = (priority1 * RegPR);     
					X2 = (priority2 * priority2); 

					if(X1 < X2)
						nodesPR3V3[i].setSwitch(false, true);
					else
						nodesPRBatt[i].setSwitch(false,true);
									
				}
				else
				{
					nodesPR3V3[i].getcurrent(&RegPR);
					nodesPR3V3[i].getPriority(&priority1);
					X1 = (priority1 * RegPR); 
					if( (i+1) != 12)
					{		
						nodesPR3V3[i + 1].getcurrent(&RegPR);
						nodesPR3V3[i + 1].getPriority(&priority1);				
						X2 = (priority1 * RegPR);// calculate x Vaule

						// if X1 is greater than X2, save the index value
						// else save index value of payload 2
						if(X1 > X2)
							x_index = i;
						else
							x_index = i+1;
					}
					nodesPV3V3[x_index].setSwitch(false,true);
				}
			}
	}
	else if (BattCurrB > 4)
	{
		for(int i = 0; i < 12; i++)
			{
				// iterate through payload arrays
				// use sizeof nodesPRBatt[] as a condition
				// to make sure there is not out of bounds error
				if(i < 7)
				{
					// get current and priority number
					// of each payload
					nodesPR3V3[i].getcurrent(&RegPR);
					nodesPR3V3[i].getPriority(&priority1);

					nodesPRBatt[i].getCurrent(&BattPR);
					nodesPRBatt[i].getPriority(&priority2);
					
					// calculate x Value
					X1 = (priority1 * RegPR);     
					X2 = (priority2 * priority2); 

					if(X1 < X2)
						nodesPR3V3[i].setSwitch(true, false);
					else
						nodesPRBatt[i].setSwitch(true,false);
									
				}
				else
				{
					nodesPR3V3[i].getcurrent(&RegPR);
					nodesPR3V3[i].getPriority(&priority1);
					X1 = (priority1 * RegPR); 
					if( (i+1) != 12)
					{		
						nodesPR3V3[i + 1].getcurrent(&RegPR);
						nodesPR3V3[i + 1].getPriority(&priority1);				
						X2 = (priority1 * RegPR);// calculate x Vaule

						// if X1 is greater than X2, save the index value
						// else save index value of payload 2
						if(X1 > X2)
							x_index = i;
						else
							x_index = i+1;
					}
					nodesPV3V3[x_index].setSwitch(true,false);
				}
			}
	}



	
	/*******************************************
	 * Monitor Temperature of the Batteries    *
	 * Use thermistorBattA/B and getTemperature*
	 * to get temperature of both batteries    *
	 * Turn on heater if temperature is below  *
	 * TEMP_MIN. Turn of heaters when          *
	 * temperature is above (TEMP_MIN + 10)	   *
	 ****************************************/
	result = thermistorBattA.getTemperature(&temp1);
	if (result != ERROR_SUCCESS)
	{
		ERROR("Periodic", "Failed to get temperature of battery A");
		return result;
	}

	result = thermistorBattB.getTemperature(&temp2);
	if (result != ERROR_SUCCESS)
	{
		ERROR("Periodic", "Failed to get temperature of Battery B");
		return result;
	}

	if (temp1 < TEMP_MIN)
	{
		// battery A is cold, turn on heater
		if (BattVoltA < BattVoltB)
			// power heater A with with battery B
			nodesBatteryHeaters[0].setSwitch(false, true);
		else if (BattVoltA > BattVoltB)
			// Heater powered by battery A
			nodesBatteryHeaters[0].setSwitch(true, false);
		else
			nodesBatteryHeaters[0].setSwitch(true, true);
	}
	else if (temp2 < TEMP_MIN)
	{
		// battery B is cold, turn on heater
		if (BattVoltB < BattVoltA)
			// power heater B with battery A
			nodesBatteryHeaters[1].setSwitch(true, false);
		else if (BattVoltB > BattVoltA)
			// Heater powered by battery B
			nodesBatteryHeater[1].setSwitch(false, true);
		else
			// both batteries power heater
			nodeBatteryHeaters[1].setSwitch(true, true);
	}
	else
	{
		// if temperature reads 10 degrees higher than min temp or if temp reads higher
		// higher than max temp, turn of heaters.
		if( temp1 > (TEMP_MIN + 10) || temp1 > TEMP_MAX)
			nodesBatteryHeaters[0].setSwitch(false, false); // Turn off Heater A
		if(temp2 > (TEMP_MAX + 10) || temp2 > TEMP_MAX)
			nodesBatteryHeaters[1].setSwitch(false, false); // Turn off Heater 
		
	}



	/************************************  Monitor Current and Temperature of 3.3V Regulator   ************************************/
	// Current level leaving the regulators
	long Reg3v3A = 0;
	long Reg3v3B = 0;
	long Reg3v3A_out = 0;
	long Reg3v3B_out = 0;

	long RegTmpA = 0;
	long RegTmpB = 0;

	long LoadI = 0;
	int priority = 0;
	 

	/*******************************************
	 * Monitor input/output current of both    *
	 * Regulators. Use getCurrent() to get     *
	 * current levels of the regulators using  *
	 * regulator objects. Reduce payloads if   *
	 * current is above 3A. 
	 * ****************************************/
	for(int x = 0; x < 12; x++)
	{
		result = node3V3OutA.getCurrent(&Reg3v3A_out);
		if (result != ERROR_SUCCESS)
		{
			ERROR("Periodic", "FAiled to get current of 3.3V Regulator A");
			return result;
		}
		
		result = node3V3InA.getCurrent(&Reg3v3A);
		if (result != ERROR_SUCCESS)
		{
			ERROR("Periodic", "Failed to get current of 3.3V Regulatar B");
			return result;
		}

		result = node3V3InB.getCurrent(&Reg3v3B);
		if (result != ERROR_SUCCESS)
		{
			ERROR("Periodic", "Failed to get current level entering 3.3V Regulator B");
			return result;
		}
	
		result = node3V3OutB.getCurrent(&Reg3v3B_out);
		if (result != ERROR_SUCCESS)
		{
			ERROR("Periodic", "Failed to get current level entering 3.3V Regulator B");
			return result;
		}

		// both output or input current channels are above 3A
		if ( (Reg3v3A_out > 3 || Reg3v3A > 3) && (Reg3v3B_out > 3 || Reg3v3B > 3) )
		{
			for(int i = 0; i < 12; i++)
			{
				// Get current and priority number of payload
				nodesPR3V3[i].get_current(&LoadI);
				nodesPR3V3[i].getPriority(&priority);

				X1 = (priority * LoadI); // calculate x Value
				
				// check for outofbounds error
				if( (i+1) != 12)
				{	
					//get current and priority of next payload 
					nodesPR3V3[i + 1].get_current(&LoadI);
					nodesPR3V3[i + 1].getPriority(&priority);
					
					X2 = (priority * LoadI);// calculate x Vaule

					// if X1 is greater than X2, save the index value
					// else save index value of payload 2
					if(X1 > X2)
						x_index = i;
					else
						x_index = i+1;
				}
				// Turn off payload at index x_index;
				nodesPR3V3[x_index].setSwitch(false, false);
			}
			
		}
		// REG3V3B current is above 3A
		else if(Reg3v3A_out > 3 || Reg3v3A > 3) 
		{
			for(int i = 0; i < 12; i++)
			{
				// Get current and priority number of payload
				nodesPR3V3[i].get_current(&LoadI);
				nodesPR3V3[i].getPriority(&priority);

				X1 = (priority * LoadI); // calculate x Value
				
				// check for outofbounds error
				if( (i+1) != 12)
				{	
					//get current and priority of next payload 
					nodesPR3V3[i + 1].get_current(&LoadI);
					nodesPR3V3[i + 1].getPriority(&priority);
					
					X2 = (priority * LoadI);// calculate x Vaule

					// if X1 is greater than X2, save the index value
					// else save index value of payload 2
					if(X1 > X2)
						x_index = i;
					else
						x_index = i+1;
				}
				// Turn off payload at index x_index;
				nodesPR3V3[x_index].setSwitch(false, true);
			}

			
		}
		// REG3V3A current is above 3A
		else if (Reg3v3B_out > 3 || Reg3v3B > 3) 
		{
			for(int i = 0; i < 12; i++)
			{
				// Get current and priority number of payload
				nodesPR3V3[i].get_current(&LoadI);
				nodesPR3V3[i].getPriority(&priority);

				X1 = (priority * LoadI); // calculate x Value
				
				// check for outofbounds
				if( (i+1) != 12)
				{	
					//get current and priority of next payload 
					nodesPR3V3[i + 1].get_current(&LoadI);
					nodesPR3V3[i + 1].getPriority(&priority);
					
					X2 = (priority * LoadI);// calculate x Vaule

					// if X1 is greater than X2, save the index value
					// else save index value of payload 2
					if(X1 > X2)
						x_index = i;
					else
						x_index = i+1;
				}
				nodesPR3V3[x_index].setSwitch(true, false);
			}			
		}
	}
	
	/*******************************************
	* Monitor Temperature of Regulators		   *
	* Use getTemperature() to get temperature  *
	* reading from thermistor. Reduce payloads *
	* to reduce temperature.
	*******************************************/
	for(int x = 0; x < 12; x++)
	{
		// compare temperature readings of Regulators
		result = thermistorRegA.getTemperature(&RegTmpA);
		if (result != ERROR_SUCCESS)
		{
			ERROR("Periodic", "Failed to retreive temperature of 3.3V regulator A");
			return result;
		}

		result = thermistorBattB.getTemperature(&RegTmpB);
		if (result != ERROR_SUCCESS)
		{
			ERROR("Periodic", "Failed to retreive temperature of 3.3V regulator A");
			return result;
		}


		// If temperature of any of the two regulators are above max operating temperature, turn off the path that correlates to that Regulator to the lowest priority payload
		// for now it will just change the paths to the germination camera and ground camera
		
		if (RegTmpA > REG_TEMP_MAX) // if temp of Regulator A is higher than its operating  temperature, reduce payload to reduce temperature
		{
			for(int i = 0; i < 12; i++)
			{
				// Get current and priority number of payload
				nodesPR3V3[i].get_current(&LoadI);
				nodesPR3V3[i].getPriority(&priority);

				X1 = (priority * LoadI); // calculate x Value
				
				// check for outofbounds error
				if( (i+1) != 12)
				{	
					//get current and priority of next payload 
					nodesPR3V3[i + 1].get_current(&LoadI);
					nodesPR3V3[i + 1].getPriority(&priority);
					
					X2 = (priority * LoadI);// calculate x Vaule

					// if X1 is greater than X2, save the index value
					// else save index value of payload 2
					if(X1 > X2)
						x_index = i;
					else
						x_index = i+1;
				}
				// Turn off payload at index x_index;
				nodesPR3V3[x_index].setSwitch(false, true);
			}
			
		}
		else if (RegTmpB > REG_TEMP_MAX) // if temp of Regulator A is higher than its operating  temperature, reduce payload to reduce temperature
		{
			for(int i = 0; i < 12; i++)
			{
				// Get current and priority number of payload
				nodesPR3V3[i].get_current(&LoadI);
				nodesPR3V3[i].getPriority(&priority);

				X1 = (priority * LoadI); // calculate x Value
				
				// check for outofbounds error
				if( (i+1) != 12)
				{	
					//get current and priority of next payload 
					nodesPR3V3[i + 1].get_current(&LoadI);
					nodesPR3V3[i + 1].getPriority(&priority);
					
					X2 = (priority * LoadI);// calculate x Vaule

					// if X1 is greater than X2, save the index value
					// else save index value of payload 2
					if(X1 > X2)
						x_index = i;
					else
						x_index = i+1;
				}
				// Turn off payload at index x_index;
				nodesPR3V3[x_index].setSwitch(true, false);

			}
			
		}
		else if (RegTmpA && RegTmpB > REG_TEMP_MAX) // if temp of both Regulators is higher than max operationg temperature, switch off both paths
		{
			for(int i = 0; i < 12; i++)
			{
				// Get current and priority number of payload
				nodesPR3V3[i].get_current(&LoadI);
				nodesPR3V3[i].getPriority(&priority);

				X1 = (priority * LoadI); // calculate x Value
				
				// check for outofbounds error
				if( (i+1) != 12)
				{	
					//get current and priority of next payload 
					nodesPR3V3[i + 1].get_current(&LoadI);
					nodesPR3V3[i + 1].getPriority(&priority);
					
					X2 = (priority * LoadI);// calculate x Vaule

					// if X1 is greater than X2, save the index value
					// else save index value of payload 2
					if(X1 > X2)
						x_index = i;
					else
						x_index = i+1;
				}
			}
			// Turn off payload at index x_index;
			nodesPR3V3[x_index].setSwitch(false, false);
		}
	}
	

	/******************** Monitor Current of Each Payload *****************************/

	double currentL = 0.00; // stores current level
	double Ecurrent = 0.00; // Expected Current level
	
	// loop through all payloads and see if it is drawing to much current
	for(int loadI = 0; loadI < 12; loadI++)
	{
		nodesPR3V3[i].getCurrent(&currentL);        // get current of payload
		nodesPR3V3[i].getExpectedCurrent(&Ecurrent); // get expected current of payload

		if(currentL == (Ecurrent * 1.5))
		{
			nodesPR3V3.getSwitch(false, false); 
		}
	}

	for(int loadI = 0; loadI < 7; loadI++ )
	{
		nodesPRBatt[loadI].getCurrent(&currentL);
		nodesPRBatt[loadI].getExpectedCurrent(&Ecurrent);

		if(currentL == (Ecurrent * 1.5))
		{
			nodesPR3V3.setSwitch(false, false);
		}
	}

	return ERROR_NOT_SUPPORTED;
}