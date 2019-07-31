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
	int index = 0;
	int payload_priority_level = 0;
	double magic_num = 0.015; // Lowest current produced by a payload
	double decision_num_one = 0, decision_num_two = 0;
	double temp_current_reg = 0; // will be used to store a payload current;

	while(threshold != 0)
	{
		if(index != 7) // Will compare loads from the battery load array and 3V3REG load array
		{
			nodesPR3V3[index].getPriority(&payload_priority_level);
			decision_num_one = (payload_priority_level * magic_num);

			nodesPRBatt[index].getPriority(&payload_priority_level);
			decision_num_two = (payload_priority_level * magic_num);

			if( decision_num_one > decision_num_two)
			{
				nodesPR3V3[index].getExpectedCurrent(&temp_current_reg);
				nodesPR3V3[index].setSwitch(false,false);
			}
			else
			{
				nodesPRBatt[index].getExpectedCurrent(&temp_current_reg);
				nodesPRBatt[index].setSwitch(false,false);
			}

			// subtract the current of the load that was switched 
			// off from the threshold value
			threshold -= temp_current_reg;
		}
		else // only compare from 3V3REG load array
		{
			
			
		}
		
		


		index++;

		// if looped through all arrays, reset index back to
		// the beginning again
		if(index > 12)
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


	//////////////////////// Check Temperature of Battery A ////////////////////////////
	result = thermistorBattA.getTemperature(&current_BattA_Temp);
	if (result != ERROR_SUCCESS)
	{
		//ERROR("Periodic", "Failed to get temperature of battery A");
		return result;
	}

	if(current_BattA_Temp > THRES_BATT_TEMP_HIGH)
	{
		nodesBatteryHeaters[0].setSwitch(false, false);
		if( current_BattA_Temp > THRES_BATT_MAX_HIGH_TEMP)
			load_shedding(max_threshold);
		else
			load_shedding(temp_threshold);
	}
	else if (current_BattA_Temp < THRES_BATT_TEMP_LOW)
	{
		nodesBatteryHeaters[0].setSwitch(true, true);
	}
	else if (current_BattA_Temp >= ACCEPTABLE_BATT_TEMP)
	{
		nodesBatteryHeaters[0].setSwitch(false, false);
	}

	//////////////////////// Check Temperature of Battery B ////////////////////////////
	result = thermistorBattB.getTemperature(&current_BattB_Temp);
	if(result != ERROR_SUCCESS)
	{
		//ERROR("Periodic", "Failed to get temperature of battery A");
		return result;
	}

	if(current_BattB_Temp > THRES_BATT_TEMP_HIGH)
	{
		nodesBatteryHeaters[1].setSwitch(false,false);
		if( current_BattB_Temp > THRES_BATT_MAX_HIGH_TEMP)
			load_shedding(max_threshold);
		else
			load_shedding(temp_threshold);
	}
	else if (current_BattB_Temp < THRES_BATT_TEMP_LOW)
	{
		nodesBatteryHeaters[1].setSwitch(true,true);
	}
	else if(current_BattA_Temp >= ACCEPTABLE_BATT_TEMP)
	{
		nodesBatteryHeaters[1].setSwitch(false,false);
	}
	
	//////////////////////// Check Temperature of 3V3 Regulator A ////////////////////////////
	result = thermistorRegA.getTemperature(&current_REGA_Temp);
	if (result != ERROR_SUCCESS)
	{
		//ERROR("Periodic", "Failed to retreive temperature of 3.3V regulator A");
		return result;
	}

	if (current_REGA_Temp > THRES_REG_TEMP_HIGH)
		load_shedding(temp_threshold);

	
	//////////////////////// Check Temperature of 3V3 Regulator B ////////////////////////////
	result = thermistorRegB.getTemperature(&current_REGB_Temp);
	if (result != ERROR_SUCCESS)
	{
		//ERROR("Periodic", "Failed to retreive temperature of 3.3V regulator A");
		return result;
	}

	if (current_REGB_Temp > THRES_REG_TEMP_HIGH)
		load_shedding(temp_threshold);

	//////////////////////// Check current level of 3V3 Regulator A ////////////////////////////

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

	//////////////////////// Check current levels of payload ////////////////////////////



	
		
	return ERROR_NOT_SUPPORTED;
}