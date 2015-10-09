/*
 *  iris.c 
 *  Copyright (c) 2015 - TheFwGuy
 *
 *  This code is to handle the iris project 
 *  The board used is a LaunchPad, using a MSP430F2013 and the code is 
 *  compiled using MSPGCC 4.9 using the -mmcu=msp430g2452  parameter
 *
 *  The code reads a rotary digital encoder and uses the two LED on board
 *  to indicate the status.
 *  The red LED will change state at every count, i.e. every time the 
 *  shaft is turned.
 *  The green LED will indicate the direction. ON clockwise, OFF counterclockwise
 *
 *  This program uses the timer to generate the PWM !
 */
#include "iris.h"
#include "digenc.h"
#include "usi_i2c.h"
#include "tsl2561.h"
#include "ht16k33.h"

#include "Oled/Oled_SSD1306.h"
#include "Oled/Oled_Printf.h"
#include "Oled/Oled_Config.h"


/*
 *  Set I/O
 *   P1.0  -> Count indicator(OUTPUT - red LED)
 *   P1.1  -> Debug - output
 *   P1.2  -> PWM - output
 *   P1.3  -> Encoder SW (pushbutton) - input
 *   P1.4  -> Encoder A - input
 *   P1.5  -> Encoder B - input
 *   P1.6  -> I2C SCL
 *   P1.7  -> I2C SDA
 */


/*
 *  Global variables
 */
unsigned char  Port1Shadow = 0;
unsigned char  TogglePosition = 1;

extern unsigned char _tsl2561Initialised;

/*
 ****************************** Start Code *******************************
 */

int main(void) 
{
   int count;
   int readSens;

   init();		/* Initialize the system */

#if defined LIGHT_SENSOR
	init_tsl2561();
#endif

//   wakeup_tsl2561();

	while(1)
	{	
#if defined LIGHT_SENSOR
      /*
       *  By default the light sensor is read. The encoder determine the position of the servo.
       *  Pushing the pushbutton changes the mode.
       *  Encoder vs. light sensor
       */
      if(Pushbutton == ON)
		{
			switch(TogglePosition)
			{
				default:
				case 0:
			      EncCounter = PWMSTART;
					TogglePosition = 1;
					break;
				 
				 case 1:
//			      EncCounter = PWMSTOP;
				   TogglePosition = 0;
				   break;
			}
				
	   	Pushbutton = OFF;
		}

      if(TogglePosition)
      {
         if(i2c_done())
         {
            readSens = read_tsl2561(TSL2561_FULLSPECTRUM);

            if(readSens < 4)
       	      EncCounter = PWMSTART;
            else if(readSens < 10)
               EncCounter = PWMSTART+10;
            else if(readSens < 40)
               EncCounter = PWMSTART+20;
            else if(readSens < 200)
               EncCounter = PWMSTART+30;
            else
               EncCounter = PWMSTOP;
         }
      }
#else
      /*
       *  The light sensor is not used. 
       *  The encoder determine the position of the servo.
       *  Pushing the pushbutton changes the end-to-end 
       */
      if(Pushbutton == ON)
		{
			switch(TogglePosition)
			{
				default:
				case 0:
			      EncCounter = PWMSTART;
					TogglePosition = 1;
					break;
				 
				 case 1:
			      EncCounter = PWMSTOP;
				   TogglePosition = 0;
				   break;
			}
				
	   	Pushbutton = OFF;
		}
#endif

		TA0CCR1 = EncCounter;

   	/*
	    *  Update the Output port 
	    */
	   LED_PORT = Port1Shadow;		
	}
}

/**
 *  @fn Init
 *  @author TheFwGuy
 *  @brief Init clock, timer and initialize variables
 *
 *  @param None
 *  @return None
 */
void init(void)
{
   WDTCTL = WDTPW + WDTHOLD;  /* Stop watchdog timer */

   /*
    *  Set DCO
    */
   DCOCTL  = CALDCO_16MHZ;		/* Set MCLK and SMCLK to DCO */
   BCSCTL1 = CALBC1_16MHZ;  	/* Set up 16 Mhz using internal calibration value */

	BCSCTL1 |= DIVA_0;		/* ACLK */
   BCSCTL3 |= XCAP_3;		/* 12.5pF cap- setting for 32768Hz crystal */
	
	P1SEL |= PWM;			/* Assign PWM to timer output */
	P1DIR |= PWM;
	P1OUT = 0;           /* Force out low */

//   P1DIR |= COUNT_LED+INDICATOR_LED+DEBUGSIG;  		/* Set outputs */
   P1DIR |= COUNT_LED+DEBUGSIG;  		/* Set outputs */
	P1REN = ENCODER_MASK;  	/* Enable pull up resistor on Encoder A/B input */
	P1IE  = ENCODER_MASK+ENCODER_SW;	/* Interrupt enabled on Encoder A/B input and switch */
	P1IES = 0x00;				/* Interrupt on low to high transiction */

   /*
    *  Set variables
    */
	Port1Shadow = 0;              /* Force all Output to zero */

   /*
    *  Set USI for I2C
    */
   i2c_init(USIDIV_7, USISSEL_2);

   /*
    *  Set Timer
    */
	TA0CCR0 = TMRVALUE;
	TA0CCTL1 |= OUTMOD_7;
	TA0CCR1 = EncCounter;
	TA0CTL |= TASSEL_1 + MC_1;		/* Uses ACLK */

   _BIS_SR(GIE);              /* Enable interrupt */
}


