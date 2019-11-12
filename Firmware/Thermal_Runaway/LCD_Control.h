/*
 * LCD_Control.h
 *
 * Created: 12/18/2013 10:10:19 PM
 *  Author: Nick Mikstas
 */

#ifndef LCD_CONTROL_H_
#define LCD_CONTROL_H_

#include <avr/io.h>
#include "Thermal_Runaway.h"

//LCD control signal bit numbers.
#define RST 1
#define RS  2
#define SCL 0
#define SI  3

//LCD control signal port numbers.
#define prtRST PORTB
#define prtRS  PORTD
#define prtSCL PORTB
#define prtSI  PORTD

//Function prototypes.
void write_command(uint8_t d);      //The following function writes command byte d to the LCD.
void write_byte(uint8_t d);         //The following function writes byte d to LCD to be displayed.
void write_string(char *p);         //The following function writes a string of bytes to the LCD.
void print_hex(unsigned char b);    //This function prints the hex value of a number on the display.
void init_lcd();                    //initialize the LCD.

#endif /* LCD_CONTROL_H_ */
