/******************************************************************************
 * File Name   :  OLED_SSD1306.h
 * Author      :  43oh - MSP430 News Projects and Forums.
               :  (http://www.43oh.com)
 * Description :  Header file for OLED_SSD1306.c 
 * Date        :  October 21, 2011.
 *****************************************************************************/

 #ifndef OLED_SSD1306_H_
#define OLED_SSD1306_H_

// Chip Select
#define OLED_CS	0x01 // P1.0

// Data/Command Select			
#define OLED_DC	0x20 // GND

// Read/Write
#define OLED_RW	0x02 // P1.1

#define OLED_RD	0x04 // P1.2

//Reset
#define OLED_RES	0x02 // P1.4

#define SSD1306_LCDWIDTH 128
#define SSD1306_LCDHEIGHT 64


#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

void SSD1306PinSetup( void );
void SSD1306SendCommand( char c );
void SSD1306SendData( char c );
void SSD1306Init( void );
void SSD1306Display(void);

void Set_Start_Column(unsigned char d);
void Set_Addressing_Mode(unsigned char d);
void Set_Column_Address(unsigned char a, unsigned char b);
void Set_Page_Address(unsigned char a, unsigned char b);
void Set_Start_Line(unsigned char d);
void Set_Contrast_Control(unsigned char d);
void Set_Charge_Pump(unsigned char d);
void Set_Segment_Remap(unsigned char d);
void Set_Entire_Display(unsigned char d);
void Set_Inverse_Display(unsigned char d);
void Set_Multiplex_Ratio(unsigned char d);
void Set_Display_On_Off(unsigned char d);
void Set_Start_Page(unsigned char d);
void Set_Common_Remap(unsigned char d);
void Set_Display_Offset(unsigned char d);
void Set_Display_Clock(unsigned char d);
void Set_Precharge_Period(unsigned char d);
void Set_Common_Config(unsigned char d);
void Set_VCOMH(unsigned char d);
void Fill_RAM(unsigned char Data);
void Fill_RAM_PAGE(unsigned char page, unsigned char Data);
void Fill_RAM_FONT(unsigned char number, unsigned char column,unsigned char row,unsigned char span, unsigned char width);
void Fill_RAM_FONT_SMALL(unsigned char number, unsigned char column,unsigned char row);

#endif /*SSD1306_OLED_H_*/
