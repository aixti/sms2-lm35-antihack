#include <stddef.h>
#use delay(clock=12000000)

#define LCD_RS          PIN_C5
//#define LCD_RW          PIN_A1
#define LCD_EN          PIN_C4

#define LCD_D4          PIN_C0
#define LCD_D5          PIN_C1
#define LCD_D6          PIN_C2
#define LCD_D7          PIN_C3

// misc display defines-
#define Line_1          0x80
#define Line_2          0xC0
#define Clear_Scr       0x01

// prototype statements
#separate void LCD_Init ( void );// ham khoi tao LCD
#separate void LCD_SetPosition ( unsigned int cX );//Thiet lap vi tri con tro
#separate void LCD_PutChar ( unsigned int cX );// Ham viet1kitu/1chuoi len LCD
#separate void LCD_PutCmd ( unsigned int cX) ;// Ham gui lenh len LCD
#separate void LCD_PulseEnable ( void );// Xung kich hoat
#separate void LCD_SetData ( unsigned int cX );// Dat du lieu len chan Data
// D/n Cong
#use standard_io ( B )
#use standard_io (A)


//khoi tao LCD**********************************************
#separate void LCD_Init ( void )
    {
    LCD_SetData ( 0x00 );
    delay_ms(200);       /* wait enough time after Vdd rise >> 15ms */
    output_low ( LCD_RS );// che do gui lenh
    LCD_SetData ( 0x03 );   /* init with specific nibbles to start 4-bit mode */
    LCD_PulseEnable();
    LCD_PulseEnable();
    LCD_PulseEnable();
    LCD_SetData ( 0x02 );   /* set 4-bit interface */
    LCD_PulseEnable();      /* send dual nibbles hereafter, MSN first */
    LCD_PutCmd ( 0x2C );    /* function set (all lines, 5x7 characters) */
    LCD_PutCmd ( 0b00001100);    /* display ON, cursor off, no blink */
    LCD_PutCmd ( 0x06 );    /* entry mode set, increment & scroll left */
    LCD_PutCmd ( 0x01 );    /* clear display */
    }

#separate void LCD_SetPosition ( unsigned int cX )
    {
    /* this subroutine works specifically for 4-bit Port A */
    LCD_SetData ( swap ( cX ) | 0x08 );
    LCD_PulseEnable();
    LCD_SetData ( swap ( cX ) );
    LCD_PulseEnable();
    }

#separate void LCD_PutChar ( unsigned int cX )
    {
    /* this subroutine works specifically for 4-bit Port A */
        output_high ( LCD_RS );
        LCD_PutCmd( cX );
        output_low ( LCD_RS );
    }

#separate void LCD_PutCmd ( unsigned int cX )
    {
    /* this subroutine works specifically for 4-bit Port A */
    LCD_SetData ( swap ( cX ) );     /* send high nibble */
    LCD_PulseEnable();
    LCD_SetData ( swap ( cX ) );     /* send low nibble */
    LCD_PulseEnable();
    }
#separate void LCD_PulseEnable ( void )
    {
    output_high ( LCD_EN );
    delay_us ( 3 );         // was 10
    output_low ( LCD_EN );
    delay_ms ( 3 );         // was 5
    }

#separate void LCD_SetData ( unsigned int cX )
    {
    output_bit ( LCD_D4, cX & 0x01 );
    output_bit ( LCD_D5, cX & 0x02 );
    output_bit ( LCD_D6, cX & 0x04 );
    output_bit ( LCD_D7, cX & 0x08 );
    }
