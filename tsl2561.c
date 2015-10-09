/*
 *  tsl2561.c 
 *  Copyright (c) 2015 - TheFwGuy
 *
 *  This code handles  a tsl2561 light sensor via I2C
 */
#include <msp430.h>
#include <signal.h>
#include "usi_i2c.h"
#include "tsl2561.h"

/*
 *  Expects I/O
 *   P1.6  -> I2C SCL
 *   P1.7  -> I2C SDA
 */

/*
 *  Global variables
 */
unsigned char _tsl2561Initialised = 0;
tsl2561IntegrationTime_t _tsl2561IntegrationTime = TSL2561_INTEGRATIONTIME_13MS;
tsl2561Gain_t _tsl2561Gain = TSL2561_GAIN_0X;

uint16_t seqid[] = TSL2561_READID;
uint16_t seqcn[] = TSL2561_CONFIG;
uint16_t seqwq[] = TSL2561_WAKEUP;
uint16_t seqsl[] = TSL2561_SLEEP;


/*
 * Private function prototypes
 */
static int luxcalc(uint16_t, uint16_t);
 
 
/*
 ****************************** Start Code *******************************
 */

uint16_t seq1[10];
uint8_t recseq[10];

/**
 *  @fn init_tsl2561
 *  @author TheFwGuy
 *  @brief Init TSL2561
 *
 *  @param None
 *  @return None
 */
void init_tsl2561(void)
{
   /*
    *  Read Tsl2561 ID
    */
   recseq[0] = 0;
   while(!i2c_done());
   i2c_send_sequence((uint16_t *)seqid, TSL2561_READID_LEN, (uint8_t *)recseq, 0);

//  if (recseq[0] == 0)
//   {
//      /* No chipset present - TBD what to do */
//   } 

   wakeup_tsl2561();
   config_tsl2561();
   sleep_tsl2561();
}

/**
 *  @fn config_tsl2561
 *  @author TheFwGuy
 *  @brief Configure TSL2561
 *
 *  @param Integration time
 *  @param Gain
 *  @return None
 */
void config_tsl2561()
{
   seqcn[2] = _tsl2561IntegrationTime | _tsl2561Gain;

   while(!i2c_done());
   i2c_send_sequence((uint16_t *)seqcn, TSL2561_CONFIG_LEN, (uint8_t *)recseq, 0);
   
}

/**
 *  @fn wakeup_tsl2561
 *  @author TheFwGuy
 *  @brief Wakeup TSL2561
 *
 *  @param None
 *  @return None
 */
void wakeup_tsl2561(void)
{
   while(!i2c_done());
   i2c_send_sequence((uint16_t *)seqwq, TSL2561_WAKEUP_LEN, (uint8_t *)recseq, 0);

   __delay_cycles(500000);		/* Approx 35 ms */

}

/**
 *  @fn sleep_tsl2561
 *  @author TheFwGuy
 *  @brief Put TSL2561 in sleep
 *
 *  @param None
 *  @return None
 */
void sleep_tsl2561(void)
{
   while(!i2c_done());
   i2c_send_sequence((uint16_t *)seqsl, TSL2561_SLEEP_LEN, (uint8_t *)recseq, 0);
}

/**
 *  @fn read_tsl2561
 *  @author TheFwGuy
 *  @brief Read the TSL2561
 *
 *  @param Reading (TSL2561_FULLSPECTRUM, TSL2561_VISIBLE or TSL2561_INFRARED)
 *  @return Intensity (LUX)
 */
int read_tsl2561(unsigned char reading)
{
   uint16_t ch0 = 0;
	uint16_t ch1 = 0;

#if defined(READWORD)	
   uint16_t seqread_CH0[] = TSL2561_READ16_CH0;
   uint16_t seqread_CH1[] = TSL2561_READ16_CH1;
#else
   uint8_t result = 0;
   uint16_t seqread_lsb_CH0[] = TSL2561_READ8_CH0_LSB;
   uint16_t seqread_lsb_CH1[] = TSL2561_READ8_CH1_LSB;
   uint16_t seqread_msb_CH0[] = TSL2561_READ8_CH0_MSB;
   uint16_t seqread_msb_CH1[] = TSL2561_READ8_CH1_MSB;
#endif

	/* Wake up sensor */
   wakeup_tsl2561();

	switch(reading)
	{
		case TSL2561_FULLSPECTRUM:
#if defined(READWORD)	
         while(!i2c_done());
         i2c_send_sequence((uint16_t *)seqread_CH0, TSL2561_READ16_CH0_LEN, (uint8_t *)recseq, 0);
 			ch0 = 256 * recseq[0] + recseq[1];
         while(!i2c_done());
         i2c_send_sequence((uint16_t *)seqread_CH1, TSL2561_READ16_CH1_LEN, (uint8_t *)recseq, 0);
			ch1 = 256 * recseq[0] + recseq[1];
#else
         while(!i2c_done());
         i2c_send_sequence((uint16_t *)seqread_lsb_CH0, TSL2561_READ8_CH0_LSB_LEN, (uint8_t *)&result, 0);
			ch0 = result;
         while(!i2c_done());
         i2c_send_sequence((uint16_t *)seqread_msb_CH0, TSL2561_READ8_CH0_MSB_LEN, (uint8_t *)&result, 0);
			ch0 += (result * 256);

         while(!i2c_done());
         i2c_send_sequence((uint16_t *)seqread_lsb_CH1, TSL2561_READ8_CH1_LSB_LEN, (uint8_t *)&result, 0);
			ch1 = result;
         while(!i2c_done());
         i2c_send_sequence((uint16_t *)seqread_msb_CH1, TSL2561_READ8_CH1_MSB_LEN, (uint8_t *)&result, 0);
			ch1 += (result * 256);

#endif
			break;
	}
	
   sleep_tsl2561();

   return (ch0+ch1);
//	return(luxcalc(ch0, ch1));
}

/*
 *  ------------------------------ Private functions -------------------------
 */
static int luxcalc(uint16_t ch0, uint16_t ch1)
{
   unsigned long chScale;
   unsigned long channel1;
   unsigned long channel0;
   unsigned long ratio = 0;
   unsigned long ratio1 = 0;
   unsigned int b, m;
   unsigned long temp;
   uint32_t lux;
	
   switch (_tsl2561IntegrationTime)
   {
      case TSL2561_INTEGRATIONTIME_13MS:
         chScale = TSL2561_LUX_CHSCALE_TINT0;
         break;
		case TSL2561_INTEGRATIONTIME_101MS:
         chScale = TSL2561_LUX_CHSCALE_TINT1;
         break;
		case TSL2561_INTEGRATIONTIME_402MS:
      default: /* No scaling ... integration time = 402ms */
         chScale = (1 << TSL2561_LUX_CHSCALE);
         break;
   }

   /* Scale for gain (1x or 16x) */
   if(!_tsl2561Gain) 
      chScale = chScale << 4;

   /* scale the channel values */
   channel0 = (ch0 * chScale) >> TSL2561_LUX_CHSCALE;
   channel1 = (ch1 * chScale) >> TSL2561_LUX_CHSCALE;

   /* find the ratio of the channel values (Channel1/Channel0) */
   if(channel0 != 0) 
      ratio1 = (channel1 << (TSL2561_LUX_RATIOSCALE+1)) / channel0;

   /* round the ratio value */
   ratio = (ratio1 + 1) >> 1;

#ifdef TSL2561_PACKAGE_CS
   if ((ratio >= 0) && (ratio <= TSL2561_LUX_K1C))
      {b=TSL2561_LUX_B1C; m=TSL2561_LUX_M1C;}
   else if (ratio <= TSL2561_LUX_K2C)
      {b=TSL2561_LUX_B2C; m=TSL2561_LUX_M2C;}
   else if (ratio <= TSL2561_LUX_K3C)
      {b=TSL2561_LUX_B3C; m=TSL2561_LUX_M3C;}
   else if (ratio <= TSL2561_LUX_K4C)
      {b=TSL2561_LUX_B4C; m=TSL2561_LUX_M4C;}
   else if (ratio <= TSL2561_LUX_K5C)
      {b=TSL2561_LUX_B5C; m=TSL2561_LUX_M5C;}
   else if (ratio <= TSL2561_LUX_K6C)
      {b=TSL2561_LUX_B6C; m=TSL2561_LUX_M6C;}
   else if (ratio <= TSL2561_LUX_K7C)
      {b=TSL2561_LUX_B7C; m=TSL2561_LUX_M7C;}
   else if (ratio > TSL2561_LUX_K8C)
      {b=TSL2561_LUX_B8C; m=TSL2561_LUX_M8C;}
#else
   if ((ratio >= 0) && (ratio <= TSL2561_LUX_K1T))
      {b=TSL2561_LUX_B1T; m=TSL2561_LUX_M1T;}
   else if (ratio <= TSL2561_LUX_K2T)
      {b=TSL2561_LUX_B2T; m=TSL2561_LUX_M2T;}
   else if (ratio <= TSL2561_LUX_K3T)
      {b=TSL2561_LUX_B3T; m=TSL2561_LUX_M3T;}
   else if (ratio <= TSL2561_LUX_K4T)
      {b=TSL2561_LUX_B4T; m=TSL2561_LUX_M4T;}
   else if (ratio <= TSL2561_LUX_K5T)
      {b=TSL2561_LUX_B5T; m=TSL2561_LUX_M5T;}
   else if (ratio <= TSL2561_LUX_K6T)
      {b=TSL2561_LUX_B6T; m=TSL2561_LUX_M6T;}
   else if (ratio <= TSL2561_LUX_K7T)
      {b=TSL2561_LUX_B7T; m=TSL2561_LUX_M7T;}
   else if (ratio > TSL2561_LUX_K8T)
      {b=TSL2561_LUX_B8T; m=TSL2561_LUX_M8T;}
#endif

   temp = ((channel0 * b) - (channel1 * m));

   /* do not allow negative lux value */
   if (temp < 0) 
      temp = 0;

   /* round lsb (2^(LUX_SCALE-1)) */
   temp += (1 << (TSL2561_LUX_LUXSCALE-1));

   /* strip off fractional portion */
   lux = temp >> TSL2561_LUX_LUXSCALE;

   return lux;		
}
