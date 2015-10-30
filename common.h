/*
  common.h

*/

#ifndef COMMON_H
#define COMMON_H

#include <msp430.h>
#include <signal.h>
#include <stdint.h>

/*
 *  Export global variables
 */
extern unsigned char  Port1Shadow;
extern unsigned char  TogglePosition; 

/*
 *  Behavior control
 *
 *  If defined LIGHT_SENSOR the light sensor control the motor - if not defined 
 *  the pushbutton works to control the motor
 */
#define     LIGHT_SENSOR

/*
 *  Generic defines
 */
#define		OFF				0
#define 		ON 				1

/*
 *  Hardware related defines
 */
#define 	LED_PORT			P1OUT
#define  ENCODER_PORT	P1IN

#define 	COUNT_LED   	BIT0
#define 	INDICATOR_LED  BIT6

#define  DEBUGSIG			BIT1
#define  PWM				BIT2	/* Must be this one for the PWM !! */

/*
 *  I/O managment defines
 */
#define COUNT_ON  Port1Shadow |= COUNT_LED
#define COUNT_OFF Port1Shadow &= ~COUNT_LED
#define COUNT_TOGGLE Port1Shadow ^= COUNT_LED

#define DIRECTION_CLOCKWISE  Port1Shadow|= INDICATOR_LED
#define DIRECTION_ANTICLOCKWISE  Port1Shadow &= ~INDICATOR_LED
#define DIRECTION_TOGGLE Port1Shadow ^= INDICATOR_LED
/*
 *  The macro DEBUG_TOGGLE update both the shadow and the port at the same time
 */
#define DEBUG_TOGGLE Port1Shadow ^= DEBUGSIG; P1OUT ^= DEBUGSIG

/*
 *  PWM managment defines
 */
#define PWMSTART  22   /* original 30 */  
#define PWMSTOP   74  /* original 64 */  

/*
 *  Timer related defines
 */
#define TMRVALUE 660		/* Orig 330 */

#endif
