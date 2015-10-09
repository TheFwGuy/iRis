/*
  digenc.h

*/

#ifndef DIGENC_H
#define DIGENC_H

#include "common.h"

/*
 *  Function prototypes
 */
void init_encoder(void);
void read_encoder(void);

/*
 *  Export global variables
 */
extern unsigned short EncCounter;
extern unsigned char  Pushbutton;

/*
 *  Hardware related defines
 */
#define  ENCODER_PORT	P1IN

#define	ENCODER_A		BIT4
#define	ENCODER_B		BIT5
#define	ENCODER_SW		BIT3

#define	ENCODER_MASK	(ENCODER_A+ENCODER_B)
#define	ENCODER_SHIFT	4		/* Number of shift to normalize the reading */

/*
 *  Encoder related defines
 */
#define	CLOCKWISE		0
#define	ANTICLOCKWISE	1

#define	ENCODER_NOCHANGE	0
#define	ENCODER_INCREMENT	1
#define	ENCODER_DECREMENT	2
#define	ENCODER_ERROR		3
 
#endif
