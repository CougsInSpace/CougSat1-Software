#include "Events.h"
#include "tools/CISError.h"
#include "PMICObjects.h"
#include "PMICConfiguration.h"

/*
	Function: load_shedding()
	Input Parameter: double threshold
	Return: uint8_t value, either an error type or 0 for successful monitoring
	Description:
		This function will be called whenever payloads are needed to be shed.
		Will iterate through payloads and bus and switching off the one with
		the least significance in the system by taking product of the priority
		number and the lowest current drawn by all the payloads and bus, which
		is 0.015A. The produce that is the lowest will be switched off and 
		the expected current of the payload or non-payload will be subtracted
		from the threshold value. ONce the threshold value reaches zero, the
		function will terminate.
 */
uint8_t load_shedding(double threshold)
{
	int index = 0; // used to move through PRBatt[] and PR3V3[]
	int payload_priority_level = 0; // will contain a load priority number
	double magic_num = 0.015; // Lowest current produced by a payload
	double decision_num_one = 0, decision_num_two = 0; // value that will be used to compare which load to shutoff
	double temp_current_reg = 0; // will be used to store a payload current;

	while(threshold != 0)
	{

		if(index != 7) // Will compare loads from the battery load array and 3V3REG load array
		{
			nodesPR3V3[index]->getPriority(&payload_priority_level);
			decision_num_one = (payload_priority_level * magic_num);

			nodesPRBatt[index]->getPriority(&payload_priority_level);
			decision_num_two = (payload_priority_level * magic_num);

			if( decision_num_one > decision_num_two)
			{
				nodesPR3V3[index]->getExpectedCurrent(&temp_current_reg);
				nodesPR3V3[index]->setSwitch(false,false);
			}
			else
			{
				nodesPRBatt[index]->getExpectedCurrent(&temp_current_reg);
				nodesPRBatt[index]->setSwitch(false,false);
			}

			// subtract the current of the load that was switched 
			// off from the threshold value
			threshold -= temp_current_reg;
		}
		else // only compare from 3V3REG load array
		{
			// ensures we do not exceed scope of nodesPR3V3[] 
			if(index != 12)
			{
				// Check one payload and get decision number
				nodesPR3V3[index]->getPriority(&payload_priority_level);
				decision_num_one = (payload_priority_level * magic_num);

				// Check next payload and get decision number
				nodesPR3V3[index + 1]->getPriority(&payload_priority_level);
				decision_num_one = (payload_priority_level * magic_num);

				// if present payload at PR3V3[index] has a greater value than
				// the payload at PR3V3[index + 1], turn that payload off
				// get that payloads expected current
				// else turn the other payload off and increment index
				if( decision_num_one > decision_num_two)
				{
					nodesPR3V3[index]->getExpectedCurrent(&temp_current_reg);
					nodesPR3V3[index]->setSwitch(false,false);
				}
				else
				{
					nodesPR3V3[index + 1]->getExpectedCurrent(&temp_current_reg);
					nodesPR3V3[index + 1]->setSwitch(false,false);
					index++; // skip this payload on next loop and check at (index + 2) instead
				}
			}
			else // if index is 12
			{
					// if at index 12, turn this one off and reset index to 0 to loop throug
					nodesPR3V3[index]->getExpectedCurrent(&temp_current_reg);
					nodesPR3V3[index]->setSwitch(false,false);
			}
			
			// subtract the current of the load that was switched 
			// off from the threshold value
			threshold -= temp_current_reg;
			
		}

		// increment to iterate through payloads and bus arrays
		index++;

		// if looped through all arrays and threshold value does not equal zero, 
		//reset index back t0 the beginning again
		if(index > 12  && threshold != 0)
			index = 0;

	}


	return 0;
}

/**
 * @brief Executes the periodic event
 *
 * @return uint8_t error code
 */
uint8_t eventPeriodic()
{
	// Variables
	bool condition = false;
	uint8_t result;

	/////// variables to store current values and temperature values //////////
	double current_BattA_Temp = 0;
	double current_BattB_Temp = 0;
	double current_REGA_Temp = 0;
	double current_REGB_Temp = 0;
	double output_current_REG3V3A = 0;
	double output_current_REG3V3B = 0;
	double input_current_REG3V3A = 0;
	double input_current_REG3V3B = 0;
	double current_to_shed = 0;
	double batt_load_current_level = 0;
	double reg3v3_load_current_level = 0;
	double load_current = 0;


	//////////////////////// Check Temperature of Battery A ////////////////////////////
	
	/* get temperature of BattA, return error type if it fails to get Temp. */
	result = thermistorBattA.getTemperature(&current_BattA_Temp);
	if (result != ERROR_SUCCESS)
	{
		//ERROR("Periodic", "Failed to get temperature of battery A");
		return result;
	}

	/*if temperature of Battery A exceeds the highest temperature allowed without damage to batter
	  Turn off heater if it was ever on and check to see if temp is really hot or just hot.
	  if really hot, call load_shedding and shed all payloads and bus, else only shed a certain amount.
	  if the battery temp is below a minimum temp, turn heaters on and if Temp is within a reasonable
	  temperature, turn heater off.  */
	if(current_BattA_Temp > THRES_BATT_TEMP_HIGH)
	{
		nodesBatteryHeaters[0]->setSwitch(false, false);
		if( current_BattA_Temp > THRES_BATT_MAX_HIGH_TEMP)
			load_shedding(max_threshold);
		else
			load_shedding(temp_threshold);
	}
	else if (current_BattA_Temp < THRES_BATT_TEMP_LOW)
	{
		nodesBatteryHeaters[0]->setSwitch(true, true);
	}
	else if (current_BattA_Temp >= ACCEPTABLE_BATT_TEMP)
	{
		nodesBatteryHeaters[0]->setSwitch(false, false);
	}

	//////////////////////// Check Temperature of Battery B ////////////////////////////

	/* Same as Battery A Temp monitoring*/
	result = thermistorBattB.getTemperature(&current_BattB_Temp);
	if(result != ERROR_SUCCESS)
	{
		//ERROR("Periodic", "Failed to get temperature of battery A");
		return result;
	}

	if(current_BattB_Temp > THRES_BATT_TEMP_HIGH)
	{
		nodesBatteryHeaters[1]->setSwitch(false,false);
		if( current_BattB_Temp > THRES_BATT_MAX_HIGH_TEMP)
			load_shedding(max_threshold);
		else
			load_shedding(temp_threshold);
	}
	else if (current_BattB_Temp < THRES_BATT_TEMP_LOW)
	{
		nodesBatteryHeaters[1]->setSwitch(true,true);
	}
	else if(current_BattA_Temp >= ACCEPTABLE_BATT_TEMP)
	{
		nodesBatteryHeaters[1]->setSwitch(false,false);
	}
	
	//////////////////////// Check Temperature of 3V3 Regulator A ////////////////////////////
	
	/* Get temperature reading from thermistor of Regulator A. Return Error type if failed to get reading */
	result = thermistorRegA.getTemperature(&current_REGA_Temp);
	if (result != ERROR_SUCCESS)
	{
		//ERROR("Periodic", "Failed to retreive temperature of 3.3V regulator A");
		return result;
	}

	/* if temperature of regulator A is above a certain Temperature, call load shedding and shed a certain amount
	   amount of loads */
	if (current_REGA_Temp > THRES_REG_TEMP_HIGH)
		load_shedding(temp_threshold);

	
	//////////////////////// Check Temperature of 3V3 Regulator B ////////////////////////////

	/* same as above */
	result = thermistorRegB.getTemperature(&current_REGB_Temp);
	if (result != ERROR_SUCCESS)
	{
		//ERROR("Periodic", "Failed to retreive temperature of 3.3V regulator A");
		return result;
	}

	if (current_REGB_Temp > THRES_REG_TEMP_HIGH)
		load_shedding(temp_threshold);

	//////////////////////// Check current level of 3V3 Regulator A ////////////////////////////

	/* Get input and output currents of Regulator A. Return Error Types if failed to get current levels */
	result = node3V3OutA.getCurrent(&output_current_REG3V3A);
	if (result != ERROR_SUCCESS)
	{
		//ERROR("Periodic", "Failed to retreive temperature of 3.3V regulator A");
		return result;
	}

	result = node3V3InA.getCurrent(&input_current_REG3V3A);
	if (result != ERROR_SUCCESS)
	{
		//ERROR("Periodic", "Failed to retreive temperature of 3.3V regulator A");
		return result;
	}

	/* if either input or output current is higher than expected, subtract expectre current from 
	   recored current and thats how much current needs to be shed by the load shedding function */
	if(output_current_REG3V3A > REG_EXPECTED_CURRENT)
	{
		current_to_shed = (output_current_REG3V3A - REG_EXPECTED_CURRENT);
		load_shedding(current_to_shed);
	}
	else if (input_current_REG3V3A > REG_EXPECTED_CURRENT)
	{
		current_to_shed = (input_current_REG3V3A - REG_EXPECTED_CURRENT);
		load_shedding(current_to_shed);
	}


	//////////////////////// Check current level of 3V3 Regulator B ////////////////////////////
	
	/* SAME AS ABOVE */
	result = node3V3OutB.getCurrent(&output_current_REG3V3B);
	if (result != ERROR_SUCCESS)
	{
		//ERROR("Periodic", "Failed to retreive temperature of 3.3V regulator A");
		return result;
	}

	result = node3V3InB.getCurrent(&input_current_REG3V3B);
	if (result != ERROR_SUCCESS)
	{
		//ERROR("Periodic", "Failed to retreive temperature of 3.3V regulator A");
		return result;
	}

	if(output_current_REG3V3B > REG_EXPECTED_CURRENT)
	{
		current_to_shed = (output_current_REG3V3B - REG_EXPECTED_CURRENT);
		load_shedding(current_to_shed);
	}
	else if (input_current_REG3V3B > REG_EXPECTED_CURRENT)
	{
		current_to_shed = (input_current_REG3V3B - REG_EXPECTED_CURRENT);
		load_shedding(current_to_shed);
	}

	//////////////////////// Check current levels of payload (PLEASE CHECK IF THIS WILL WORK!) ////////////////////////////

	// get the present current of each payload and add them up
	// if total current exceeds the 3A Load that the 3v3 Reg could
	// provide, call the load_shedding function. the input value
	// will be the difference of the total current and 3A
	for( int load_index = 0; load_index < 13; load_index++)
	{
		result = nodesPR3V3[load_index] ->getCurrent(&load_current);
		if (result != ERROR_SUCCESS)
		{
			//ERROR("Periodic", "Failed to retreive temperature of 3.3V regulator A");
			return result;
		}

		reg3v3_load_current_level += load_current;
	}

	if(load_current > REG_EXPECTED_CURRENT)
	{
		load_shedding(load_current - REG_EXPECTED_CURRENT); // amount of current to shed
	}


	// Go through all the payloads drawing current directly from battery
	// add up all the current measurements
	// if total current exceeds max current that the battery can provide
	// call laod shedding function where the input parameter will be the difference
	// between the 4A and the total current 
	result = nodesPRBatt[0] ->getCurrent(&load_current);
	if (result != ERROR_SUCCESS)
	{
		//ERROR("Periodic", "Failed to retreive temperature of 3.3V regulator A");
		return result;
	}

	batt_load_current_level += load_current;
	
	result = nodesPRBatt[5] ->getCurrent(&load_current);
	if (result != ERROR_SUCCESS)
	{
		//ERROR("Periodic", "Failed to retreive temperature of 3.3V regulator A");
		return result;
	}

	batt_load_current_level += load_current;

	result = nodesPRBatt[6]->getCurrent(&load_current);
	if (result != ERROR_SUCCESS)
	{
		//ERROR("Periodic", "Failed to retreive temperature of 3.3V regulator A");
		return result;
	}

	batt_load_current_level += load_current;

	if(batt_load_current_level > BATT_EXPECTED_CURRENT)
		load_shedding(batt_load_current_level - BATT_EXPECTED_CURRENT);
	
/////////////////////////////////////////////////////////////////////////////////////////////////





}