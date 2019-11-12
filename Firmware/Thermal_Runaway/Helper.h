/*
 * Helper.h
 *
 * Created: 12/24/2013 8:41:02 PM
 *  Author: Nick Mikstas
 */

#ifndef HELPER_H_
#define HELPER_H_

#include <avr/io.h>
#include "Thermal_Runaway.h"

#define FALSE 0
#define TRUE 1

//USART defines.
#define FOSC 8000000    //Clock frequency.
#define BAUD 9600       //Desired baud rate.
#define UBRR FOSC/16/BAUD-1

void init_micro();                                              //Initialize the micro-controller.
void Wait_ms(uint16_t delay);                                   //This function waits for delay milliseconds before exiting.
void Buzzer_On();                                               //Turn on audible alarm.
void Buzzer_Off();                                              //Turn off audible alarm.
uint16_t ADC_From_Celsius(const float celsius);
uint16_t ADC_From_Farenheit(const float farenheit);
float Celsius_From_ADC(const uint16_t adc);                     //Returns an equivalent celsius value from an ADC value.
float Farenheit_From_ADC(const uint16_t adc);
float Farenheit_From_Celsius(const float celsius);
float Celsius_From_Farenheit(const float farenheit);
uint8_t UInt16_From_Float(const float f, uint16_t *i);
uint8_t String_From_UInt(uint16_t i, char *s);
uint8_t String_From_Float(float f, char *s, uint8_t precision); //Converts a floating point number to a string

#endif /* HELPER_H_ */
