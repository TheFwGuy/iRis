/******************************************************************************
 * File Name   :  OLED_SSD1306.c
 * Author      :  43oh - MSP430 News Projects and Forums.
               :  (http://www.43oh.com)
 * Description :  Lowlevel driver for the OLED SSD1306 
 * Date        :  October 21, 2011.
 *****************************************************************************/
#include <msp430g2452.h>
#include "CourierNew_14x15.h"
#include "CourierNew_5x7.h"
#include "Oled_Config.h"
#include "OLED_SSD1306.h"

/* Function    : SSD1306PinSetup()
 * Description : Sets up the pins to the OLED display.
 * Input       : None
 * Output      : None
 * 
 */
void SSD1306PinSetup( void )
{
   // Set the following pins as outputs
   P2DIR |= (OLED_CS | OLED_RES);

   // 100ms delay at 16Mhz clock
   __delay_cycles(1600000);
}

/* Function    : SSD1306Init()
 * Description : Initializes the OLED display
 * Input       : None
 * Output      : None
 */
void SSD1306Init( void )
{
   // Pull all pins low except CS
   P2OUT |= OLED_CS;
   P2OUT &= ~OLED_DC;
   P2OUT &= ~OLED_RW;
   P2OUT &= ~OLED_RD;

   P2OUT &= ~OLED_RES;

   // Delay for 200ms at 16Mhz
   __delay_cycles(3200000);

   P2OUT |= OLED_RES;

   SSD1306SendCommand(0xae);//--turn off oled panel 
   SSD1306SendCommand(0x00);//---set low column address 
   SSD1306SendCommand(0x10);//---set high column address 
   SSD1306SendCommand(0x40);//--set start line address 
   SSD1306SendCommand(0x81);//--set contrast control register 
   SSD1306SendCommand(0xcf); 
   SSD1306SendCommand(0xa1);//--set segment re-map 95 to 0 
   SSD1306SendCommand(0xa6);//--set normal display 
   SSD1306SendCommand(0xa8);//--set multiplex ratio(1 to 64) 
   SSD1306SendCommand(0x3f);//--1/64 duty 
   SSD1306SendCommand(0xd3);//-set display offset 
   SSD1306SendCommand(0x00);//-not offset 
   SSD1306SendCommand(0xd5);//--set display clock divide ratio/oscillator frequency 
   SSD1306SendCommand(0x80);//--set divide ratio 
   SSD1306SendCommand(0xd9);//--set pre-charge period 
   SSD1306SendCommand(0xf1); 
   SSD1306SendCommand(0xda);//--set com pins hardware configuration 
   SSD1306SendCommand(0x12); 
   SSD1306SendCommand(0xdb);//--set vcomh 
   SSD1306SendCommand(0x40); 
   SSD1306SendCommand(0x8d);//--set Charge Pump enable/disable 
   SSD1306SendCommand(0x14);//--set(0x10) disable 
   SSD1306SendCommand(0xaf);//--turn on oled panel
   Fill_RAM(0x00);				// Clear Screen
}

/* Function    : SSD1306SendCommand( char c )
 * Description : Sends a command to the OLED display
 * Input       : Command
 * Output      : None
 */
void SSD1306SendCommand( char c )
{ 
   unsigned int data = c;

   USISRL = data << 7;             
   USISRH = data>>1;     
   P2OUT &= ~OLED_CS; 
   USICNT |= 9;                              
   while (!(USIIFG & USICTL1));            
   P2OUT |= OLED_CS;  
}

/* Function    : SSD1306SendData( char c )
 * Description : Sends data to the OLED display
 * Input       : Data
 * Output      : None
 */
void SSD1306SendData( char c )
{
   unsigned int data = c;

   data |= 0x100;	
   USISRL = data << 7;  
   USISRH = data>>1;   
   P2OUT &= ~OLED_CS; 
   USICNT |= 9;           
   while (!(USIIFG & USICTL1));          
   P2OUT |= OLED_CS;  
}

/* Function    : Fill_RAM( unsigned char c )
 * Description : Fills the OLED with one particular data
 * Input       : Data
 * Output      : None
 */
void Fill_RAM(unsigned char Data)
{
   unsigned char i,j;

   for(i=0;i<8;i++)
   {
      Set_Start_Page(i);
      Set_Start_Column(0x00);

      for(j=0;j<128;j++)
      {
         SSD1306SendData(Data);
      }
   }
}

/* Function    : Fill_RAM_PAGE
 * Description : Fills an OLED page with a particular data
 * Input       : page no.[0-7], Data
 * Output      : None
 */
void Fill_RAM_PAGE(unsigned char page, unsigned char Data)
{
   unsigned char j;

   Set_Start_Page(page);
   Set_Start_Column(0x00);

   for(j=0;j<128;j++)
   {
      SSD1306SendData(Data);
   }
}

/* Function    : Fill_RAM_FONT_SMALL
 * Description : Writes a charcter on the OLED display at a column, row.
 *               with a small font.
 * Input       : character, column, row
 * Output      : None
 */
void Fill_RAM_FONT_SMALL(  unsigned char number, 
                           unsigned char column,
                           unsigned char row )
{
	int j=0;
	Set_Start_Page(row);
	Set_Start_Column(column);	
	
	for(j=0;j<SMALL_FONT_WIDTH;j++)
	{
		SSD1306SendData(data_table_SMALL[j+(number*SMALL_FONT_WIDTH)]);
	}
}

/* Function    : Fill_RAM_FONT
 * Description : Writes a charcter on the OLED display at a column, row.
 *               with a large font.
 * Input       : character, column, row, span and width
 * Output      : None
 */
void Fill_RAM_FONT(  unsigned char number, 
                     unsigned char column,
                     unsigned char row,
                     unsigned char span, 
                     unsigned char width )
{
int i,j,test; 
int adder=0;
int jump = 0;

   jump = (number - 32);
	for(i=row;i>=row-span+1;i--)
	{
		Set_Start_Page(i);
		Set_Start_Column(column);
	
		for(j=0;j<width;j++)
		{
			test = 	j+(adder*width)+(jump*28);
			SSD1306SendData(data_table_LARGE[test]);
		}
		adder++;
	}

}

/* Function    : Set_Start_Column
 * Description : Sets a start column to start writing at.
 * Input       : column[0-127]
 * Output      : None
 */
void Set_Start_Column(unsigned char d)
{
	// Set Lower Column Start Address for Page Addressing Mode. Default => 0x00
   SSD1306SendCommand(0x00+d%16);		
	
	// Set Higher Column Start Address for Page Addressing Mode. Default => 0x10
   SSD1306SendCommand(0x10+d/16);		
						
}

/* Function    : Set_Start_Page
 * Description : Sets a start page to start writing at.
 * Input       : column[0-7]
 * Output      : None
 */
void Set_Start_Page(unsigned char d)
{
	// Set Page Start Address for Page Addressing Mode. Default => 0xB0 (0x00)
   SSD1306SendCommand(0xB0|d);			
						
}


#if 0
void Set_Addressing_Mode(unsigned char d)
{
	// Set Memory Addressing Mode
   SSD1306SendCommand(0x20);		
	//   Default => 0x02
   SSD1306SendCommand(d);			
   //     0x00 => Horizontal Addressing Mode
   //     0x01 => Vertical Addressing Mode
   //     0x02 => Page Addressing Mode
}


void Set_Column_Address(unsigned char a, unsigned char b)
{
	// Set Column Address
   SSD1306SendCommand(0x21);		
	//   Default => 0x00 (Column Start Address)
   SSD1306SendCommand(a);			
	//   Default => 0x7F (Column End Address)
   SD1306SendCommand(b);			
}


void Set_Page_Address(unsigned char a, unsigned char b)
{
	// Set Page Address
   SSD1306SendCommand(0x22);		
	//   Default => 0x00 (Page Start Address)
   SSD1306SendCommand(a);			
	//   Default => 0x07 (Page End Address)
   SSD1306SendCommand(b);			
}


void Set_Start_Line(unsigned char d)
{
	// Set Display Start Line
   SSD1306SendCommand(0x40|d);			
	//   Default => 0x40 (0x00)					
}


void Set_Contrast_Control(unsigned char d)
{
	// Set Contrast Control
   SSD1306SendCommand(0x81);			
	//   Default => 0x7F
   SSD1306SendCommand(d);			
}


void Set_Charge_Pump(unsigned char d)
{
	// Set Charge Pump
   SSD1306SendCommand(0x8D);			
	//   Default => 0x10.
   SSD1306SendCommand(0x10|d);			
	//     0x10 (0x00) => Disable Charge Pump
	//     0x14 (0x04) => Enable Charge Pump
}


void Set_Segment_Remap(unsigned char d)
{
	// Set Segment Re-Map
   SSD1306SendCommand(0xA0|d);			
   //   Default => 0xA0
   //     0xA0 (0x00) => Column Address 0 Mapped to SEG0
   //     0xA1 (0x01) => Column Address 0 Mapped to SEG127
}


void Set_Entire_Display(unsigned char d)
{
	// Set Entire Display On / Off
   SSD1306SendCommand(0xA4|d);			
   //   Default => 0xA4
   //     0xA4 (0x00) => Normal Display
   //     0xA5 (0x01) => Entire Display On
}


void Set_Inverse_Display(unsigned char d)
{
	// Set Inverse Display On/Off
   SSD1306SendCommand(0xA6|d);			
	//   Default => 0xA6
	//     0xA6 (0x00) => Normal Display
	//     0xA7 (0x01) => Inverse Display On
}


void Set_Multiplex_Ratio(unsigned char d)
{
	// Set Multiplex Ratio
   SSD1306SendCommand(0xA8);			
	SSD1306SendCommand(d);			
   //   Default => 0x3F (1/64 Duty)
}


void Set_Display_On_Off(unsigned char d)	
{
	// Set Display On/OffSSD1306
   SendCommand(0xAE|d);			
	//   Default => 0xAE
	//     0xAE (0x00) => Display Off
	//     0xAF (0x01) => Display On
}


void Set_Common_Remap(unsigned char d)
{
	// Set COM Output Scan Direction
   SSD1306SendCommand(0xC0|d);			
   //   Default => 0xC0
   //     0xC0 (0x00) => Scan from COM0 to 63
   //     0xC8 (0x08) => Scan from COM63 to 0
}


void Set_Display_Offset(unsigned char d)
{
	// Set Display Offset
   SSD1306SendCommand(0xD3);			
	//   Default => 0x00
   SSD1306SendCommand(d);			
}


void Set_Display_Clock(unsigned char d)
{
	// Set Display Clock Divide Ratio / Oscillator Frequency
   SSD1306SendCommand(0xD5);			
	SSD1306SendCommand(d);		
   //   Default => 0x80
	//     D[3:0] => Display Clock Divider
	//     D[7:4] => Oscillator Frequency
}


void Set_Precharge_Period(unsigned char d)
{
	// Set Pre-Charge Period
   SSD1306SendCommand(0xD9);			
	SSD1306SendCommand(d);			
   //   Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
	//     D[3:0] => Phase 1 Period in 1~15 Display Clocks
	//     D[7:4] => Phase 2 Period in 1~15 Display Clocks
}


void Set_Common_Config(unsigned char d)
{
	// Set COM Pins Hardware Configuration
   SSD1306SendCommand(0xDA);			
	SSD1306SendCommand(0x02|d);			
   //   Default => 0x12 (0x10)
	//     Alternative COM Pin Configuration
	//     Disable COM Left/Right Re-Map
}


void Set_VCOMH(unsigned char d)
{
	// Set VCOMH Deselect Level
   SSD1306SendCommand(0xDB);			
	SSD1306SendCommand(d);			
   //   Default => 0x20 (0.77*VCC)
}
#endif
