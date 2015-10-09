/*
  oled.h

*/

#ifndef OLED_H
#define OLED_H

#include <msp430.h>
#include <stdint.h>
#include "Oled/Oled_SSD1306.h"
#include "Oled/Oled_Printf.h"
#include "Oled/Oled_Config.h"

/*
 *  Function prototypes
 */
void init_oled(void);
void display_oled(unsigned short);

/*
 *  Hardwired defines
 */

/*---------------------------------------------------------
 *         ASSIGN HERE the I2C address used 
 *---------------------------------------------------------
 */
#define OLED_ADDR 0x10
 

#endif
