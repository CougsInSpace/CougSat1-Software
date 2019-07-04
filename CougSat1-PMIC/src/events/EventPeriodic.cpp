#include "Events.h"
#include "tools/CISConsole.h"
#include "tools/CISError.h"
#include "../PMICObjects.h"
#include "../PMICConfiguration.h"


/**
 * @brief Executes the periodic event
 *
 * @return uint8_t error code
 */
uint8_t eventPeriodic()
{
	uint8_t result; // used to contain error code

    // Monitor Battery Temps
	result = battery_temp_monitoring();

	if (result != 0)
	{
		//ERROR("Periodic", "Failed to retreive temperature of 3.3V regulator A");
		return result;
	}

	// Monitor Current of both 3V3 Regulators
	result = regulator_current_monitoring();

	if (result != 0)
	{
		//ERROR("Periodic", "Failed to retreive temperature of 3.3V regulator A");
		return result;
	}
	
	// Monitor Temp of Regulators
	result = regulator_temp_monitoring();

	if (result != 0)
	{
		//ERROR("Periodic", "Failed to retreive temperature of 3.3V regulator A");
		return result;
	}
	
	// Monitor Current of Each Payload
	result = payload_Monitoring();

	// if error occurs with payload monitoring, return error and exiting function
	if (result != 0)
	{
		//ERROR("Periodic", "Failed to retreive temperature of 3.3V regulator A");
		return result;
	}

	return ERROR_NOT_SUPPORTED;
}

/*
	Function: battery_temp_monitoring()
	Input Parameter: None
	Return: uint8_t value, either an error type or 0 for successful monitoring
	Description:
		This function will monitor the temperature levels of the two batteries.
		If the temperature readings are below the minimum temperature allowed to
		safely use the batteries, turn on the heater for that specific battery.
		Will keep looping and checking the temp readings until both batteries are
		at optimal temperature levels
 */
uint8_t battery_temp_monitoring()
{
	uint8_t result;
	double current_BattA_Temp = 0;
	double current_BattB_Temp = 0;
	double current_BattA_voltage = 0;
	double current_BattB_voltage = 0;
	
	do{
		result = thermistorBattA.getTemperature(&current_BattA_Temp);
		if (result != ERROR_SUCCESS)
		{
			//ERROR("Periodic", "Failed to get temperature of battery A");
			return result;
		}

		result = thermistorBattB.getTemperature(&current_BattB_Temp);
		if (result != ERROR_SUCCESS)
		{
			//ERROR("Periodic", "Failed to get temperature of Battery B");
			return result;
		}

		result = nodeBattA.getCurrent(&current_BattA_voltage);
		if (result != ERROR_SUCCESS)
		{
			//ERROR("Periodic", "Failed to get temperature of Battery B");
			return result;
		}

		result = nodeBattB.getCurrent(&current_BattB_voltage);
		if (result != ERROR_SUCCESS)
		{
			//ERROR("Periodic", "Failed to get temperature of Battery B");
			return result;
		}

		// If the current temperature of one of the battery is
		// less than the minimum temperature required to use 
		// batter, turn on heater. Makes sure to power the 
		// heater with battery with the highest voltage level
		// Once the battery reaches 10 degrees higher than min temp
		// turn heater off
		if (current_BattA_Temp < THRES_BATT_TEMP_LOW)
		{
			if (current_BattA_voltage < current_BattB_voltage)
				// power heater A with with battery B
				nodesBatteryHeaters[0].setSwitch(false, true);
			else if (current_BattA_voltage > current_BattB_voltage)
				// Heater powered by battery A
				nodesBatteryHeaters[0].setSwitch(true, false);
			else 
				// Power heater with both batteries
				nodesBatteryHeaters[0].setSwitch(true, true);
		}
		else if (current_BattB_Temp < THRES_BATT_TEMP_LOW)
		{
			// battery B is cold, turn on heater
			if (current_BattB_voltage < current_BattA_voltage)
				// power heater B with battery A
				nodesBatteryHeaters[1].setSwitch(true, false);
			else if (current_BattB_voltage > current_BattA_voltage)
				// Heater powered by battery B
				nodesBatteryHeaters[1].setSwitch(false, true);
			else
				// both batteries power heater
				nodesBatteryHeaters[1].setSwitch(true, true);
		}
		else
		{
			// if temperature reads 10 degrees higher than min temp or if temp reads higher
			// higher than max temp, turn of heaters.
			if( current_BattA_Temp > (THRES_BATT_TEMP_LOW + 10) || current_BattA_Temp > THRES_BATT_TEMP_LOW + 10)
				nodesBatteryHeaters[0].setSwitch(false, false); // Turn off Heater A
			if(current_BattB_Temp > (THRES_BATT_TEMP_HIGH + 10) || current_BattB_Temp > THRES_BATT_TEMP_HIGH)
				nodesBatteryHeaters[1].setSwitch(false, false); // Turn off Heater 
			
		}

	} while( current_BattA_Temp > (THRES_BATT_TEMP_LOW + 10) && current_BattB_Temp > (THRES_BATT_TEMP_LOW + 10));

	return 0;
}

/*
	Function: payload_Monitoring()
	input parameters: none
	return: uint8_t value, either an error type or 0
	description: 
		This function will monitor the payload's current levels. 
		If the current of the payload is 1.5 times greater than the expected current
		switch off the payload. Iterate through all the payloads drawing current
		from the battery and the 3V3 Regulators
 */
uint8_t payload_Monitoring()
{
	double actual_payload_current = 0.00;
	double expected_payload_current = 0.00;
	uint8_t result;

	// Loop through all the payloads (peripherals) and monitor current
	for (int i = 0; i < 12; i++)
	{
		result = nodesPR3V3[i].getCurrent(&actual_payload_current);        // get current of payload
		if(result != ERROR_SUCCESS)
		{
			//ERROR("Periodic", "Failed to retreive temperature of 3.3V regulator A");
			return result;
		}

		result = nodesPR3V3[i].getExpectedCurrent(&expected_payload_current); // get expected current of payload
		if(result != ERROR_SUCCESS)
		{
			//ERROR("Periodic", "Failed to retreive temperature of 3.3V regulator A");
			return result;
		}

		// compare current payload current levels
		// if levels are 1.5 times higher than the expected payload current
		// power down that payload
		if(actual_payload_current == (expected_payload_current * 1.5))
		{
			nodesPR3V3.getSwitch(false, false); 
		}
	}


	for(int i = 0; i < 7; i++ )
	{
		nodesPRBatt[i].getCurrent(&actual_payload_current);
		nodesPRBatt[i].getExpectedCurrent(&expected_payload_current);

		if(actual_payload_current == (expected_payload_current * 1.5))
		{
			nodesPR3V3.setSwitch(false, false);
		}
	}


	return 0;
}

/* 
	Function: regulator_temp_monitoring()
	input parameters: none
	return: uint8_t value, either an error type or 0
	description: 
		This function will monitor the 3V3 Regulator temperature levels.
		If the temperature levels of one of the Regulators is above
		the max temperature allowed by the regulators without damaging it,
		begin to reduce payloads that are drawing current from regulators.
		Will enter a loop that will constantly check temp. levels and reduce
		one payload each iteration until temperatures have return to optimal
		levels. An algorithm will decide which payloads to switch off based
		on priority levels and the current amount being drawn.
*/
uint8_t regulator_temp_monitoring()
{
	double current_REGA_Temp = 0;
	double current_REGB_Temp = 0;
	double payload_current = 0;
	uint8_t result;
	int priority_level = 0, index = 0;
	float payload_level = 0.0;
	float temp = 0.0;

	// keep checking current levels until current levels are optimal
	do
	{
		result = thermistorRegA.getTemperature(&current_REGA_Temp);
		if (result != ERROR_SUCCESS)
		{
			//ERROR("Periodic", "Failed to retreive temperature of 3.3V regulator A");
			return result;
		}

		result = thermistorBattB.getTemperature(&current_REGB_Temp);
		if (result != ERROR_SUCCESS)
		{
			//ERROR("Periodic", "Failed to retreive temperature of 3.3V regulator A");
			return result;
		}

		// If temperature of any of the two regulators are above max operating temperature, turn off the path that correlates to that Regulator to the lowest priority payload
		// for now it will just change the paths to the germination camera and ground camera
		if (current_REGA_Temp > THRES_REG_TEMP_HIGH) // if temp of Regulator A is higher than its operating  temperature, reduce payload to reduce temperature
		{		
			    // if current is still high and index is 12
				// reset index and loop through payloads from
				// the beginning
				if(index > 12)
					index = 0;
				
				// Get current and priority number of payload
				nodesPR3V3[index].get_current(&payload_current);
				nodesPR3V3[index].getPriority(&priority_level);

				payload_level = (priority_level * payload_current); // calculate x Value that will determine if payload gets switched off
				
				// check for outofbounds error
				// if at the last payload and
				// REG3V3A is still above optimal temps
				// simply turn off that payload
				// else compare this payload and next one
				// to decide which one to turn off first
				if( (index) != 12)
				{	
					//get current and priority of next payload 
					nodesPR3V3[index + 1].get_current(&payload_current);
					nodesPR3V3[index + 1].getPriority(&priority_level);
					temp = (priority_level * payload_current);// calculate x Vaule

					// compare payload_level value determine for each payload
					// the one with the highest value will be switched off for REG3V3A
					if(payload_level > temp)
						nodesPR3V3[index].setSwitch(false, true);	
					else
						nodesPR3V3[index + 1].setSwitch(false, true);	
				}
				else
				{
					nodesPR3V3[index].setSwitch(false, true);	
				}
						
		}
		else if (current_REGB_Temp > THRES_REG_TEMP_HIGH) // if temp of Regulator A is higher than its operating  temperature, reduce payload to reduce temperature
		{
			// if current is still high and index is 12
			// reset index and loop through payloads from
			// the beginning
			if(index > 12)
				index = 0;
			
			// Get current and priority number of payload
			nodesPR3V3[index].get_current(&payload_current);
			nodesPR3V3[index].getPriority(&priority_level);

			payload_level = (priority_level * payload_current); // calculate x Value that will determine if payload gets switched of

			if( (index) != 12)
			{	
				//get current and priority of next payload 
				nodesPR3V3[index + 1].get_current(&payload_current);
				nodesPR3V3[index + 1].getPriority(&priority_level);
				temp = (priority_level * payload_current);// calculate x Vaule

				// compare payload_level value determine for each payload
				// the one with the highest value will be switched off for REG3V3A
				if(payload_level > temp)
					nodesPR3V3[index].setSwitch(true, false);	
				else
					nodesPR3V3[index + 1].setSwitch(true, false);	
			}
			else
			{
				nodesPR3V3[index].setSwitch(true, false);	
			}
			
		}
		else if (current_REGA_Temp && current_REGB_Temp > THRES_REG_TEMP_HIGH) // if temp of both Regulators is higher than max operationg temperature, switch off both paths
		{
			// if current is still high and index is 12
			// reset index and loop through payloads from
			// the beginning
			if(index > 12)
				index = 0;
			
			// Get current and priority number of payload
			nodesPR3V3[index].get_current(&payload_current);
			nodesPR3V3[index].getPriority(&priority_level);

			payload_level = (priority_level * payload_current); // calculate x Value that will determine if payload gets switched of
			
			if( (index) != 12)
			{	
				//get current and priority of next payload 
				nodesPR3V3[index + 1].get_current(&payload_current);
				nodesPR3V3[index + 1].getPriority(&priority_level);
				temp = (priority_level * payload_current);// calculate x Vaule

				// compare payload_level value determine for each payload
				// the one with the highest value will be switched off for REG3V3A
				if(payload_level > temp)
					nodesPR3V3[index].setSwitch(true, false);	
				else
					nodesPR3V3[index + 1].setSwitch(true, false);	
			}
			else
			{
				nodesPR3V3[index].setSwitch(true, false);	
			}
		}

		index++;

	} while(current_REGA_Temp < THRES_REG_TEMP_HIGH && current_REGB_Temp < THRES_REG_TEMP_HIGH );
}

/*
	Function: regulator_current_monitoring()
	input parameters: none
	return: uint8_t value, either an error type or 0
	description: 
		This function will monitor the 3V3 Regulator input and ouput current levels.
		If the current levels of one of the Regulators is above
		the max current value allowed by the regulators without damaging it,
		begin to reduce payloads that are drawing current from regulators.
		Will enter a loop that will constantly check current levels and reduce
		one payload each iteration until current levels have return to optimal
		levels. An algorithm will decide which payloads to switch off based
		on priority levels and the current amount being drawn.
 */
uint8_t regulator_current_monitoring()
{
	uint8_t result;

	// Current level leaving the regulators
	double input_current_REG3V3A = 0;
	double input_current_REG3V3B = 0;
	double output_current_REG3V3A = 0;
	double output_current_REG3V3B = 0;
	double payload_current = 0;
	double payload_X_value = 0, payload_X2_value = 0; // used to hold value to compare which payload to switch off
	int priority = 0; // contain priority values
	int index = 0;
	 
	do{
		result = node3V3OutA.getCurrent(&output_current_REG3V3A);
		if (result != ERROR_SUCCESS)
		{
			//ERROR("Periodic", "FAiled to get current of 3.3V Regulator A");
			return result;
		}
		
		result = node3V3InA.getCurrent(&input_current_REG3V3A);
		if (result != ERROR_SUCCESS)
		{
			//ERROR("Periodic", "Failed to get current of 3.3V Regulatar B");
			return result;
		}

		result = node3V3InB.getCurrent(&input_current_REG3V3B);
		if (result != ERROR_SUCCESS)
		{
			//ERROR("Periodic", "Failed to get current level entering 3.3V Regulator B");
			return result;
		}
	
		result = node3V3OutB.getCurrent(&output_current_REG3V3B);
		if (result != ERROR_SUCCESS)
		{
			//ERROR("Periodic", "Failed to get current level entering 3.3V Regulator B");
			return result;
		}

		// both output or input current channels are above 3A
		if ( (output_current_REG3V3A > 3 || input_current_REG3V3A > 3) && (output_current_REG3V3B > 3 || input_current_REG3V3B > 3) )
		{
				// Get current and priority number of payload
				nodesPR3V3[index].get_current(&payload_current);
				nodesPR3V3[index].getPriority(&priority);

				if (index == 12)
					index = 0;

				payload_X_value = (priority * payload_current); // calculate x Value
				
				// check for outofbounds error
				if( index != 12)
				{	
					//get current and priority of next payload 
					nodesPR3V3[index + 1].get_current(&payload_current);
					nodesPR3V3[index + 1].getPriority(&priority);
					
					payload_X2_value = (priority * payload_current);// calculate x Vaule

					// if X1 is greater than X2, save the index value
					// else save index value of payload 2
					if(payload_X_value > payload_X2_value)
						nodesPR3V3[index].setSwitch(false, false);
					else
						nodesPR3V3[index+1].setSwitch(false, false);
				}
				else
				{
					nodesPR3V3[index].setSwitch(false, false);
				}				
		}
		// REG3V3B current is above 3A
		else if(output_current_REG3V3A > 3 || input_current_REG3V3A > 3) 
		{
				// Get current and priority number of payload
				nodesPR3V3[index].get_current(&payload_current);
				nodesPR3V3[index].getPriority(&priority);
				
				if (index == 12)
					index = 0;
				payload_X_value = (priority * payload_current); // calculate x Value
				
				// check for outofbounds error
				if( index != 12)
				{	
					//get current and priority of next payload 
					nodesPR3V3[index + 1].get_current(&payload_current);
					nodesPR3V3[index + 1].getPriority(&priority);
					
					payload_X2_value = (priority * payload_current);// calculate x Vaule

					// if X1 is greater than X2, save the index value
					// else save index value of payload 2
					if(payload_X_value > payload_X2_value)
						nodesPR3V3[index].setSwitch(false, true);
					else
						nodesPR3V3[index].setSwitch(false, true);
				}
				else
				{
					nodesPR3V3[index].setSwitch(false, true);
				}							
		}
		// REG3V3A current is above 3A
		else if (output_current_REG3V3B > 3 || input_current_REG3V3B > 3) 
		{
				// Get current and priority number of payload
				nodesPR3V3[index].get_current(&payload_current);
				nodesPR3V3[index].getPriority(&priority);

				if (index == 12)
					index = 0;

				payload_X_value = (priority * payload_current); // calculate x Value
				
				// check for outofbounds error
				if( (index + 1) != 12)
				{	
					//get current and priority of next payload 
					nodesPR3V3[index + 1].get_current(&payload_current);
					nodesPR3V3[index + 1].getPriority(&priority);
					
					payload_X2_value = (priority * payload_current);// calculate x Vaule

					// if X1 is greater than X2, save the index value
					// else save index value of payload 2
					if(payload_X_value > payload_X2_value)
						nodesPR3V3[index].setSwitch(true, false);
					else
						nodesPR3V3[index+1].setSwitch(true, false);
				}
				else
				{
					nodesPR3V3[index].setSwitch(true, false);
				}
						
		}

		index++;
	}while(output_current_REG3V3A > 3 || input_current_REG3V3A > 3 || output_current_REG3V3B > 3 || input_current_REG3V3B > 3);

	return 0;
}