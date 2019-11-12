/*
 * EEPROM_Control.h
 *
 * Created: 12/31/2013 3:11:09 PM
 *  Author: Nick Mikstas
 */

#ifndef EEPROM_CONTROL_H_
#define EEPROM_CONTROL_H_

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include "Thermal_Runaway.h"

//EEPROM Addresses for various settings.
#define CH_1_ENABLED 0
#define CH_2_ENABLED 2
#define CH_3_ENABLED 4
#define CH_4_ENABLED 6
#define CH_1_TRP_MSK 8
#define CH_2_TRP_MSK 10
#define CH_3_TRP_MSK 12
#define CH_4_TRP_MSK 14
#define C_OR_F 16
#define T_SP_LOW 18
#define T_SP_COLD 20
#define T_SP_WARM 22
#define T_SP_HOT 24
#define T_SP_TRIP 26
#define T_SP_HIGH 28
#define T_SP_ERROR 30
#define CH_1_CAL 32
#define CH_1_POLARITY 34
#define CH_2_CAL 36
#define CH_2_POLARITY 38
#define CH_3_CAL 40
#define CH_3_POLARITY 42
#define CH_4_CAL 44
#define CH_4_POLARITY 46
#define EEPROM_PARK 999

//Factory defaults for all set points.
#define DEFAULT_CH_1_EN ENABLED
#define DEFAULT_CH_2_EN ENABLED
#define DEFAULT_CH_3_EN ENABLED
#define DEFAULT_CH_4_EN ENABLED
#define DEFAULT_CH_1_TM 1
#define DEFAULT_CH_2_TM 2
#define DEFAULT_CH_3_TM 4
#define DEFAULT_CH_4_TM 8
#define DEFAULT_C_OR_F CELSIUS
#define DEFAULT_T_SP_LOW 93
#define DEFAULT_T_SP_COLD 168
#define DEFAULT_T_SP_WARM 279
#define DEFAULT_T_SP_HOT 354
#define DEFAULT_T_SP_TRIP 419
#define DEFAULT_T_SP_HIGH 652
#define DEFAULT_T_SP_ERROR 10
#define DEFAULT_CAL 0
#define DEFAULT_POLARITY POSITIVE

void EEPROM_Write_Word(uint16_t *address, uint16_t data);       //This function writes a 16-bit value to the EEPROM.
uint16_t EEPROM_Read_Word(uint16_t *address);                   //This function reads a 16-bit value from the EEPROM.
void EEPROM_Reset_to_Default();                                 //This function sets all the set points back to factory default.
void EEPROM_Read_All();                                         //This function reads all set points from the EEPROM.

#endif /* EEPROM_CONTROL_H_ */
