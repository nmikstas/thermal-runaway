/*
 * Thermal_Runaway.h
 *
 * Created: 12/18/2013 9:58:42 PM
 *  Author: Nick Mikstas
 */

#ifndef THERMAL_RUNAWAY_H_
#define THERMAL_RUNAWAY_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "Helper.h"
#include "LCD_Control.h"
#include "USART_Control.h"
#include "EEPROM_Control.h"

//Firmware major and minor number.
#define FIRMWARE_MAJOR 0
#define FIRMWARE_MINOR 1

//Defines used for enabling/disabling temperature channels.
#define DISABLED 0
#define ENABLED 1

//Define used for specifying units of measure as Celsius or Fahrenheit.
#define CELSIUS 0
#define FAHRENHEIT 1

//Defines used to determine which bits are active in the
//temperature channel trip mask.
#define RELAY1_BIT_MASK 0x01
#define RELAY2_BIT_MASK 0x02
#define RELAY3_BIT_MASK 0x04
#define RELAY4_BIT_MASK 0x08

//Time until trip occurs after critical temperature reached.
//1 second is about 10 counts.
#define TRIP_COUNT 100

//True/false definitions.
#define FALSE 0
#define TRUE 1

//Error codes for individual state machines.
#define ER_NONE 0
#define ER_YES 1

//Polarity used for calibrations
#define POSITIVE 0
#define NEGATIVE 1

//Error levels.
#define ERR_NONE 0
#define ERR_MIN 1
#define ERR_MAJ 2
#define ERR_TRIP 3

//Absolute maximum and minimum values allowed in set points.
#define ABSOLUTE_MIN 19     //ADC value (2C).
#define ABSOLUTE_MAX 838    //ADC value (90C).

//State machine states.
#define STATE_NORMAL 0
#define STATE_COLD_MINOR 1
#define STATE_HOT_MINOR 2
#define STATE_HOT_MAJOR 3
#define STATE_TRIP_TIMER 4
#define STATE_TRIP 5
#define STATE_ERROR 6

//Hysteresis value used to move down the state machine.
//Measures in ADC counts.
#define STATE_HYS 5

//LED control signal bit numbers.
#define GRN 6
#define RED 7

//LED control signal port numbers.
#define prtGRN PORTB
#define prtRED PORTB

//Relay control signal bit numbers.
#define RLY1 4
#define RLY2 5
#define RLY3 6
#define RLY4 7

//Relay control signal port numbers.
#define prtRLY1 PORTD
#define prtRLY2 PORTD
#define prtRLY3 PORTD
#define prtRLY4 PORTD

//Alarm status relay signal bit numbers.
#define RLY_MAJ 2
#define RLY_TRP 3

//Alarm status relay signal port numbers.
#define prtRLY_MAJ PORTC
#define prtRLY_TRP PORTC

//Button bit numbers.
#define ASB 4
#define RSB 5

//Button port numbers.
#define prtASB PINC
#define prtRSB PINC

//Buzzer control signal bit number.
#define BZR 2

//Buzzer control signal port number.
#define prtBZR PORTB

//RX array length.
#define MAX_RX_STRING 8     //Maximum USART RX string length.

//Number of temperatures to store in history.
#define MAX_TEMPS 10

//Structure for holding system set points.
typedef struct sys_state_struct
{
    uint16_t temp_low;
    uint16_t temp_cold;
    uint16_t temp_min;
    uint16_t temp_maj;
    uint16_t temp_trip;
    uint16_t temp_high;
    uint16_t temp_err;
    uint8_t  temp_c_or_f;
} sys_struct;

//Structure for holding all data related to each temperature channel.
typedef struct temp_struct
{
    uint16_t thistory[MAX_TEMPS];
    uint16_t this_temp;
    uint16_t timer;
    uint8_t  this_state;
    uint8_t  next_state;
    uint8_t  status;
    uint8_t  tenabled;
    uint8_t  trip_mask;
    uint8_t  cal_value;
    uint8_t  cal_polarity;
} tstruct;

uint8_t error; //Current system error level (updates LEDS).

//Create new system struct.
sys_struct system_struct;

//Create new structs for each temperature channel.
tstruct tchannel1, tchannel2, tchannel3, tchannel4;

//Variables for storing button statuses.
uint8_t silenced, latched;

//TX and RX arrays.
uint8_t rx_string[MAX_RX_STRING];   //Array for storing USART RX string.
uint8_t rx_string_index;            //Index into RX array above.

void Update_LEDS_and_Buzzer();              //This function updates the LEDs and buzzer on the HMI.
unsigned int get_adc();                     //Turn on ADC and get result.
void Get_Temperatures();                    //This function gets new temperature readings and updates the temperature histories.
void Update_Temp_array(uint16_t *tarray);   //This function updates the temperature histories.
void Update_Current_Temp(tstruct *t);       //Use the average of the temperature histories to calculate the current temperature.
uint8_t Er_Check(uint16_t *arrayptr);       //This function checks the temperature values and errors if a deviation beyond delta t.
void Update_State(tstruct *t);              //This function is used to update the individual state machines.
void Do_State();                            //This function is the entry point to the state machines for the box.
void Do_Trip(tstruct *t);                   //This function activated the relays corresponding the the trip mask.
void Undo_Trip(tstruct *t);                 //This function resets activated relays from Do_Trip function.
void Display_Temp(tstruct *t);              //This function displays the temperature on the LCD.
void Update_LCD();                          //This function updates the LCD every frame.
#endif /* THERMAL_RUNAWAY_H_ */
