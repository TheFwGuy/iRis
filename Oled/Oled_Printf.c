/******************************************************************************
 * File Name   :  oledPrint.c
 * Author      :  43oh - - MSP430 News Projects and Forums.
 *             :  Modified version of printf provided by:
 *             :  oPossum[ http://www.43oh.com/forum/viewtopic.php?f=10&t=1732]
 * Description :  printf extension for the SSD1306 OLED
 * Date        :  October 21, 2011.
 *****************************************************************************/
#include "stdarg.h"
#include "Oled_Config.h"
#include "Oled_SSD1306.h"
#include "Oled_Printf.h"




static 	char column = 0;
static 	char row = 0;
static	char columnIncrementSmallFont = 0;
static	char columnIncrementLargeFont = 0;
static	enum e_columnIncrementFlag 
					columnIncrementFlag		 = NO_FONT;

void putc(unsigned char character)
{
   if (columnIncrementFlag == SMALL_FONT)
   {
   	Fill_RAM_FONT_SMALL(character-32,columnIncrementSmallFont,row);
   	columnIncrementSmallFont = columnIncrementSmallFont+SMALL_FONT_WIDTH;
   }

   if (columnIncrementFlag == LARGE_FONT)
   {
   	Fill_RAM_FONT(character,columnIncrementLargeFont,row,LARGE_FONT_SPAN,LARGE_FONT_WIDTH);
   	columnIncrementLargeFont = columnIncrementLargeFont+LARGE_FONT_WIDTH;
   }   	
}
                     

void puts(char *s)
{
  while(*s) 
  {
    if (columnIncrementFlag == SMALL_FONT)
    {
      Fill_RAM_FONT_SMALL(*s-32,columnIncrementSmallFont,row);
      columnIncrementSmallFont = columnIncrementSmallFont+SMALL_FONT_WIDTH;
    }

    if (columnIncrementFlag == LARGE_FONT)
    {
      Fill_RAM_FONT(*s,columnIncrementLargeFont,row,LARGE_FONT_SPAN,LARGE_FONT_WIDTH);
      columnIncrementLargeFont = columnIncrementLargeFont+LARGE_FONT_WIDTH;
    }   
    *s++;
  }
}

void OLED_PrintfSetColumnRow(char setColumn, char setRow, char IncrementFlag)
{
	row = setRow;
	columnIncrementFlag = (enum e_columnIncrementFlag)IncrementFlag;
	
	if (columnIncrementFlag == SMALL_FONT)
		columnIncrementSmallFont = setColumn;
	
	if (columnIncrementFlag == LARGE_FONT)
		columnIncrementLargeFont = setColumn;  
}

static const unsigned long dv[] = {
//  4294967296      // 32 bit unsigned max
    1000000000,     // +0
     100000000,     // +1
      10000000,     // +2
       1000000,     // +3
        100000,     // +4
//       65535      // 16 bit unsigned max     
         10000,     // +5
          1000,     // +6
           100,     // +7
            10,     // +8
             1,     // +9
};

static void xtoa(unsigned long x, const unsigned long *dp)
{
    char c;
    unsigned long d;
    if(x) {
        while(x < *dp) ++dp;
        do {
            d = *dp++;
            c = '0';
            while(x >= d) ++c, x -= d;
            putc(c);
        } while(!(d & 1));
    } else
        putc('0');
}

static void puth(unsigned n)
{
    static const char hex[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    putc(hex[n & 15]);
}

void OLED_Printf(char *format, ...)
{
    char c;
    int i;
    long n;
    
    va_list a;
    va_start(a, format);
    while(c = *format++) {
        if(c == '%') {
            switch(c = *format++) {
                case 's':                       // String
                    puts(va_arg(a, char*));
                    break;
                case 'c':                       // Char
                    putc(va_arg(a, char));
                    break;
                case 'i':                       // 16 bit Integer
                case 'd':                       // 16 bit Integer
                case 'u':                       // 16 bit Unsigned
                    i = va_arg(a, int);
                    if(c == 'i' && i < 0) i = -i, putc('-');
                    xtoa((unsigned)i, dv + 5);
                    break;
                case 'l':                       // 32 bit Long
                case 'n':                       // 32 bit uNsigned loNg
                    n = va_arg(a, long);
                    if(c == 'l' &&  n < 0) n = -n, putc('-');
                    xtoa((unsigned long)n, dv);
                    break;
                case 'x':                       // 16 bit heXadecimal
                    i = va_arg(a, int);
                    puth(i >> 12);
                    puth(i >> 8);
                    puth(i >> 4);
                    puth(i);
                    break;
                case 0: return;
                default: goto bad_fmt;
            }
        } else
        {
				bad_fmt:    putc(c);
		                
        }
    }
    va_end(a);
}
