/*
 *  digenc.c 
 *  Copyright (c) 2015 - TheFwGuy
 *
 *  This code handles a digital encoder 
 */
#include <msp430.h>
#include <signal.h>
#include "digenc.h"

/*
 *  Uses I/O
 *   P1.3  -> Encoder SW (pushbutton) - input
 *   P1.4  -> Encoder A - input
 *   P1.5  -> Encoder B - input
 */

/*
 * Private function prototypes
 */

/*
 *  Global variables
 */
unsigned short EncCounter  = PWMSTART;
unsigned char  Pushbutton  = 0;

 
/*
 ****************************** Start Code *******************************
 */

/**
 *  @fn init_encoder
 *  @author TheFwGuy
 *  @brief init the encoder related variables
 *
 *  @param None
 *  @return None
 */
void init_encoder()
{
	EncCounter = PWMSTART; 
}

/**
 *  @fn Port1int - read encoder
 *  @author TheFwGuy
 *  @brief Read the encoder
 *    This function is called under I/O interrupt everytime the A or the B signals
 *    change state from low to high.
 *    The previous_reading variable holds the previous SIGNAL reading, NOT the
 *    the previous Gray code readout !
 *    
 *
 *  @param None
 *  @return None
 */
__attribute__((__interrupt__(PORT1_VECTOR)))
void Port1int (void)
{
   static unsigned char previous_ab_reading = 0;
	unsigned char reading;
	unsigned char ab_reading;
	unsigned char action = ENCODER_NOCHANGE;
	
	/*
	 *  Encoder reading
	 */
	if(P1IFG & (ENCODER_MASK))
	{
		__delay_cycles(6000);

		reading = ENCODER_PORT;	/* Acquire A and B */

		/*
		 *  Normalize the reading bringing the A nad B signals to position 0 and 1
		 */
		ab_reading = (reading & ENCODER_MASK)>>ENCODER_SHIFT;
		
		if(ab_reading == 0x3)
		{
			if(previous_ab_reading == 0x1)
			{
				action = ENCODER_INCREMENT;
				COUNT_TOGGLE;
//				DIRECTION_CLOCKWISE;
			}
			else if(previous_ab_reading == 0x2)
			{
				action = ENCODER_DECREMENT;
				COUNT_TOGGLE;
//				DIRECTION_ANTICLOCKWISE;
			}
		}
		previous_ab_reading = ab_reading;

		/*
		 *  Counter management
		 *  The counter is allowed to count from 0 to PWMSTOP only
		 */
		switch(action)
		{
			case ENCODER_INCREMENT:
				if(EncCounter < PWMSTOP)
					EncCounter++;
				break;
			
			case ENCODER_DECREMENT:
				if(EncCounter > PWMSTART)
					EncCounter--;
				break;
		}	

/*
 *  Trying to change interrupt direction 
 */
#if 0		
      if(P1IES & (ENCODER_MASK))		
		{
      	P1IE  = ENCODER_MASK+ENCODER_SW;	/* Interrupt enabled on Encoder A/B input and switch */
		   P1IES = 0x00;				/* Interrupt on low to high transiction */
		}
		else
		{
      	P1IE  = ENCODER_MASK+ENCODER_SW;	/* Interrupt enabled on Encoder A/B input and switch */
			P1IES = ENCODER_MASK;
		}
#endif		
	}
	
	/*
	 *  Pushbutton reading
	 */
	if(P1IFG & ENCODER_SW)
	{
		//DEBUG_TOGGLE;
		__delay_cycles(6000);
		
		Pushbutton= ON;
		//DEBUG_TOGGLE;
	}
	
//	__delay_cycles(6000);

	P1IFG = 0x00;
}


/*
 *  ------------------------------ Private functions -------------------------
 */
