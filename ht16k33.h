/*
  ht16k33.h

*/

#ifndef HT16K33_H
#define HT16K33_H

#include <msp430.h>
#include <stdint.h>

// HT16K33 address definition
#define ADRESSE_HT16K33 0x70 // quand les adress A2 A1 et A0 sont flottants (000)
// registres reliés à MISC
#define ADRESSE_REG_INT_FLAG 0x60 // utilisé pour test de routine de validation
#define ADRESSE_REG_OSCILLATOR 0x20 // on ou off oscillateur
#define ADRESSE_REG_START 0x00 // adresse de départ pour l'écriture
#define ADRESSE_REG_DISPLAY_BLINK_SETUP 0x80 // pour controler le display à on ou off, et controler le blink
#define ADRESSE_REG_DISPLAY_DIMMING_SET 0xE0 // définir le taux de pulse (duty)
 
// Commandes pour registres
#define MASK_OSCILLATOR_OFF 0x00 // utiliser conjointement avec REG_OSCILLATOR
#define MASK_OSCILLATOR_ON 0x01 // utiliser conjointement avec REG_OSCILLATOR
 
#define MASK_DISPLAY_OFF 0x00 // Display status OFF
#define MASK_DISPLAY_ON 0x01 // Display status OFF
 
#define MASK_BLINK_OFF 0x00 // Blinking frequency
#define MASK_BLINK_2HZ 0x02
#define MASK_BLINK_1HZ 0x04
#define MASK_BLINK_0_5HZ 0x06
 
#define MASK_DIMMING_LOWEST 0x00 // dimming 1/16 duty
#define MASK_DIMMING_MEDIUM 0x07 // dimming 8/16 duty
#define MASK_DIMMING_HIGHEST 0x0F // dimming 16/16 duty
 
#define MASK_DOT_ON 0x80
 
#define MASK_COLUMN 0x02

/*
 *  Function prototypes
 */
void init_ht(void);
void display_ht(unsigned short);

/*
 *  Hardwired defines
 */

/*---------------------------------------------------------
 *         ASSIGN HERE the I2C address used 
 *---------------------------------------------------------
 */
#define HT16K33_ADDR 0x10
 
/*
 * HT16K33 related defines
 */

#endif
