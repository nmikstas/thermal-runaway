/*
 * USART_Control.h
 *
 * Created: 12/29/2013 4:30:38 PM
 *  Author: Nick Mikstas
 */

#ifndef USART_CONTROL_H_
#define USART_CONTROL_H_

#include <avr/io.h>
#include "Thermal_Runaway.h"
#include "Helper.h"
#include "EEPROM_Control.h"

//Maximum length of rx array length.
#define RX_ARRAY_LENGTH 7

//Used to determine return value error codes.
#define NO_ERROR 0
#define ERROR 1

void TX_Byte(uint8_t data);                                     //This function transmits a single byte to the USART.
void TX_String(char *data);                                     //This function transmits a string of data out the UART.
void TX_Hex(uint8_t hex_byte);                                  //This function prints the hex value of a number out the USART.
void USART_Item_Number(const uint8_t number);                   //Write the item number in the terminal menu.
void USART_Number(const uint16_t number);                       //Write a number in the terminal menu.
void USART_Cal();                                               //Write "Calibration (+/-) (0 to 99)".
void USART_Print_Cal(const uint8_t polarity, const uint8_t value); //Write calibration value to terminal.
void USART_Channel();                                           //Write "Channel " in the terminal window.
void USART_E_or_D();                                            //Write " (E or D): " in the terminal window.
void USART_En_or_Dis(const uint8_t tenabled);                   //Write "Enabled\r" or "Disabled\r" in the terminal window.
void USART_Carriage_Return();                                   //Write "\r" in the terminal window.
void USART_Trip_Mask();                                         //Write "Trip Mask " in the terminal window.
void USART_Temp_Set();                                          //Write "Temperature Setpoint " in the terminal window.
void USART_Temp_Show(uint16_t adc, uint8_t precision);          //Write a Celsius or Fahrenheit integer in the terminal window.
void USART_T();                                                 //Write "T" in terminal window.
void USART_Colon();                                             //Write ":" in terminal window.
void USART_Space();                                             //Write " " in terminal window.
void USART_Min_Max();                                           //Write absolute min value to absolute max value in terminal window.
void USART_Alarm();                                             //Write "Alarm" in the terminal window.
void USART_Alarm_State(uint8_t en, uint16_t this_temp, uint8_t this_state); //Write the alarm state in the terminal window.
void USART_ERR();                                               //Print "ERR" in terminal window.
void USART_OK();                                                //Print "OK" in terminal window.
void USART_Write_Menu();                                        //Write the configuration menu in the terminal window.
void USART_Tokenizer(uint8_t rx_byte);                          //This function tokenizes the user input.
uint8_t Selection1(uint8_t *rx_array);                          //Menu selection 1.
uint8_t Selection2(uint8_t *rx_array);                          //Menu selection 2.
uint8_t Selection3(uint8_t *rx_array);                          //Menu selection 3.
uint8_t Selection4(uint8_t *rx_array);                          //Menu selection 4.
uint8_t Selection5(uint8_t *rx_array);                          //Menu selection 5.
uint8_t Selection6(uint8_t *rx_array);                          //Menu selection 6.
uint8_t Selection7(uint8_t *rx_array);                          //Menu selection 7.
uint8_t Selection8(uint8_t *rx_array);                          //Menu selection 8.
uint8_t Selection9(uint8_t *rx_array);                          //Menu selection 9.
uint8_t Selection10(uint8_t *rx_array);                         //Menu selection 10.
uint8_t Selection11(uint8_t *rx_array);                         //Menu selection 11.
uint8_t Selection12(uint8_t *rx_array);                         //Menu selection 12.
uint8_t Selection13(uint8_t *rx_array);                         //Menu selection 13.
uint8_t Selection14(uint8_t *rx_array);                         //Menu selection 14.
uint8_t Selection15(uint8_t *rx_array);                         //Menu selection 15.
uint8_t Selection16(uint8_t *rx_array);                         //Menu selection 16.
uint8_t Selection17(uint8_t *rx_array);                         //Menu selection 17.
uint8_t Selection18(uint8_t *rx_array);                         //Menu selection 18.
uint8_t Selection19(uint8_t *rx_array);                         //Menu selection 19.
uint8_t Selection20(uint8_t *rx_array);                         //Menu selection 20.
uint8_t Selection21();                                          //Menu selection 21.
uint8_t Convert_1_Hex(uint8_t *p, uint16_t *value);             //Takes one hex value from an array and converts to a numeric value.
uint8_t Convert_2_Hex(uint8_t *p, uint16_t *value);             //Takes two hex values from an array and converts to a numeric value.
uint8_t Convert_1_Num(uint8_t *p, uint16_t *value);             //Takes one decimal value from an array and converts to a numeric value.
uint8_t Convert_2_Num(uint8_t *p, uint16_t *value);             //Takes two decimal values from an array and converts to a numeric value.
uint8_t Convert_3_Num(uint8_t *p, uint16_t *value);             //Takes three decimal values from an array and converts to a numeric value.
uint8_t Check_Hex_Range(const uint8_t value);                   //Check if character is a valid hex value.
uint8_t Check_Int_Range(const uint8_t value);                   //Check if character is a valid decimal value.
#endif /* USART_CONTROL_H_ */
