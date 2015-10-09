#include <Wire.h>
#include "TSL2561.h"

TSL2561 tsl(TSL2561_ADDR_FLOAT); 

const int light_switch_pin = 13;

int sensitivity = 16; // 1x or 16x
int int_time = 101; // 13, 101, 402 ms
int num_measurements = 0;

boolean transmit = false;

void light_switch (int switch_state)
{
	if (switch_state == 0)
		digitalWrite(light_switch_pin, LOW);
	else if (switch_state == 111)
		digitalWrite(light_switch_pin, HIGH);
}

void set_sensitivity(int s)
{
	switch (s)
	{
	case 0: // power down
		tsl.disable();
		break;
	case 1: // 1x
		sensitivity = s;
		tsl.setGain(TSL2561_GAIN_0X);    // set no gain (for bright situtations)
		break;
	case 16: // 16x
		sensitivity = s;
		tsl.setGain(TSL2561_GAIN_16X);   // set 16x gain (for dim situations)
		break;
	default:
		break;
	}
}

void set_timing (int i)
{
	switch(i)
	{
	case 13:
		int_time = i;
		tsl.setTiming(TSL2561_INTEGRATIONTIME_13MS);  // shortest integration time (bright light)
		break;
	case 101:
		int_time = i;
		tsl.setTiming(TSL2561_INTEGRATIONTIME_101MS);  // medium integration time (medium light)
		break;
	case 402:
		int_time = i;
		tsl.setTiming(TSL2561_INTEGRATIONTIME_402MS);  // longest integration time (dim light)
		break;
	default:
		break;
	}
}

void setup()
{
	pinMode(light_switch_pin, OUTPUT);

	light_switch(0);
	set_sensitivity(sensitivity);
	set_timing(int_time);

	Serial.begin(9600); // connect to the serial port
	delay(100);
  
	if (tsl.begin())
	{
		Serial.println("Found sensor");
	}
	else
	{
		Serial.println("No sensor?");
		while (1);
	}
}

void loop()
{
	uint16_t x = 0;
	char cmd = '\0';
	char argbuf[4] = "xxx";
	int arg = 0;

	if (Serial.available() >= 4)
	{
		cmd = Serial.read();
		for (int i=0; i<3; i++)
			argbuf[i] = Serial.read();

		arg = atoi(argbuf);

		switch (cmd)
		{
		case 't':
			transmit = true;
			num_measurements = arg;
			break;
		case 'l':  //arg: 000 is off, 111 is on
			light_switch(arg);
			break;
		case 's':
			set_sensitivity(arg);
			break;
		case 'i':
			set_timing(arg);
			break;
		default: // catches bad commands
			break;
		}
	}

	if (transmit == true)
	{
		//x = tsl.getLuminosity(TSL2561_VISIBLE);    // full spectrum - infrared
		x = tsl.getLuminosity(TSL2561_FULLSPECTRUM);
		//x = tsl.getLuminosity(TSL2561_INFRARED);

		Serial.println(x, DEC);

		num_measurements--;
		if (num_measurements <= 0)
		{
			transmit = false;
			set_sensitivity(0);
			//light_switch(0);

			//MATLAB's fgetl doesn't see EOF correctly with serial, so send
			//an explicit indicator that there is no more data.
			Serial.print("EOT\n");
		}
		delay(200);
	}
}
