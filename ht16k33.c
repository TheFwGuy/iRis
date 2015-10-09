/*
 *  ht16k33.c 
 *  Copyright (c) 2015 - TheFwGuy
 *
 *  This code handles an HT16K33 based LCD display via I2C
 */
#include <msp430.h>
#include <signal.h>
#include "usi_i2c.h"
#include "ht16k33.h"

/*
 *  Expects I/O
 *   P1.6  -> I2C SCL
 *   P1.7  -> I2C SDA
 */

/*
 * Private function prototypes
 */
static const uint8_t iNumberTable[32] = 
{
   0x3F, // 0 0
   0x06, // 1 1
   0x5B, // 2 2
   0x4F, // 3 3
   0x66, // 4 4
   0x6D, // 5 5
   0x7D, // 6 6
   0x07, // 7 7
   0x7F, // 8 8
   0x6F, // 9 9
   0x5F, // a 10
   0x77, // A 11
   0x7C, // b 12
   0x39, // C 13
   0x5E, // d 14
   0x79, // E 15
   0x71, // F 16
   0x74, // h 17
   0x76, // H 18
   0x30, // I 19
   0x1E, // J 20
   0x38, // L 21
   0x54, // n 22
   0x5C, // o 23
   0x73, // P 24
   0x50, // r 25
   0x6D, // S 26
   0x78, // t 27
   0x1C, // u 28
   0x3E, // U 29
   0x6E, // y 30
   0x00, // nothing... 31
 }; 
 
/*
 ****************************** Start Code *******************************
 */

uint8_t recseq[10];

/**
 *  @fn init_ht
 *  @author TheFwGuy
 *  @brief Init the HT16k33 chip
 *
 *  @param None
 *  @return None
 */
void init_ht(void)
{
//   uint16_t seqit[] = HT16K33_WAKEUP;
//   i2c_send_sequence((uint16_t *)seqit, HT16K33_WAKEUP_LEN, (uint8_t *)recseq, 0);

}

/**
 *  @fn display_ht
 *  @author TheFwGuy
 *  @brief display a value on ht display
 *
 *  @param value
 *  @return none
 */
void display_ht(unsigned short value)
{
   
}

/*
 *  ------------------------------ Private functions -------------------------
 */
