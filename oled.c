/*
 *  ht16k33.c 
 *  Copyright (c) 2015 - TheFwGuy
 *
 *  This code handles an HT16K33 based LCD display via I2C
 */
#include <msp430.h>
#include <signal.h>
#include "usi_i2c.h"


/*
 *  Expects I/O
 *   P1.6  -> I2C SCL
 *   P1.7  -> I2C SDA
 */

/*
 * Private function prototypes
 */
 
/*
 ****************************** Start Code *******************************
 */

uint8_t recseq[10];

/**
 *  @fn init_oled
 *  @author TheFwGuy
 *  @brief Init the OLED display
 *
 *  @param None
 *  @return None
 */
void init_oled(void)
{
//   uint16_t seqit[] = HT16K33_WAKEUP;
//   i2c_send_sequence((uint16_t *)seqit, HT16K33_WAKEUP_LEN, (uint8_t *)recseq, 0);

}

/**
 *  @fn display_oled
 *  @author TheFwGuy
 *  @brief display a value on the display
 *
 *  @param value
 *  @return none
 */
void display_oled(unsigned short value)
{
   
}

/*
 *  ------------------------------ Private functions -------------------------
 */
