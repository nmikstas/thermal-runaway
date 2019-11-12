/*
 * Thermal_Runaway.c
 *
 * Created: 12/12/2013
 * Author: Nick Mikstas
 *
 * This file firmware was written for an ATMega168 running at 8MHz.
 * Fuse bits should be set to 0xFF xD5 0xE2.
 */

#include "Thermal_Runaway.h"

//This function updates the LEDs and buzzer on the HMI.
//This function also updates the status relays.
void Update_LEDS_and_Buzzer()
{
    static uint8_t counter;
    uint8_t port_bits;
    
    counter++; //Increment counter every cycle.
    
    //Reset counter every 20 cycles.
    if(counter == 20)
    {
        counter = 0;
    }
    
    //Get button statuses.
    port_bits = prtASB;
    
    //Update statuses as necessary.
    if(port_bits & (1 << ASB))
    {
        silenced = TRUE;
    }
    
    //Only reset if not currently in trip state.
    if(port_bits & (1 << RSB) && error != ERR_TRIP)
    {
        latched = FALSE;
        silenced = FALSE;
    }
    
    //Else just unsilence alarm.
    else if(port_bits & (1 << RSB) && error == ERR_TRIP)
    {
        silenced = FALSE;
    }
    
    //Update trip status relay.
    if(latched || error == ERR_TRIP)
    {
        prtRLY_TRP |= (1 << RLY_TRP);
    }
    
    else
    {
        prtRLY_TRP &= ~(1 << RLY_TRP);
    }
    
    //Update major alarm relay.
    if(error == ERR_MAJ)
    {
        prtRLY_MAJ |= (1 << RLY_MAJ);
    }
    
    else
    {
        prtRLY_MAJ &= ~(1 << RLY_MAJ);
    }   
        
    switch(error)
    {
        case ERR_NONE:
            prtRED &= ~(1 << RED); //Red LED always off.
            
            //Turn off buzzer if trip is not latched.
            if(latched && !silenced)
            {
                Buzzer_On();
            }
            else
            {
                Buzzer_Off();
            }
            
            if(counter > 9)
            {
                prtGRN |= (1 << GRN); //Blink green LED with 50% duty cycle.
            }
            
            else
            {
                prtGRN &= ~(1 << GRN);
            }
            
            break;
            
        case ERR_MAJ:
            prtGRN &= ~(1 << GRN); //Green LED always off.
            
            if(latched && !silenced)    //Latch takes priority.
            {
                Buzzer_On();
            }
            else if(!silenced && counter > 9) //50% audible buzzer.
            {
                Buzzer_On();                
            }
            else //No buzzer otherwise.
            {
                Buzzer_Off();
            }
            
            if(counter > 9)
            {
                prtRED |= (1 << RED); //Blink red LED with 50% duty cycle.
            }
            
            else
            {
                prtRED &= ~(1 << RED);
            }
            
            break;
            
        case ERR_TRIP:
            if(!silenced)   //Turn on buzzer if alarm silence has not been pressed.
            {
                Buzzer_On();
            }
            else
            {
                Buzzer_Off();
            }
            
            if(counter % 2)
            {
                prtGRN &= ~(1 << GRN); //Green LED always off.
                prtRED |= (1 << RED); //Blink red LED fast with 50% duty cycle.
            }
            
            else
            {
                prtGRN &= ~(1 << GRN);
                prtRED &= ~(1 << RED);
            }
            break;
        
        case ERR_MIN:   
        default: //Something is wrong if we end up here...minor error.
            if(latched && !silenced)    //Latch takes priority.
            {
                Buzzer_On();
            }
            else if(!silenced && counter == 19) //1/20th buzzer duty cycle.
            {
                Buzzer_On();
            }
            else //No buzzer otherwise.
            {
                Buzzer_Off();
            }
        
            if(counter >= 0 && counter <= 9)
            {
                prtGRN &= ~(1 << GRN); //Blink green LED with 50% duty cycle.
                prtRED &= ~(1 << RED); //Blink Red LED with 1/20th duty cycle.
            }
        
            else if(counter > 9 && counter < 19)
            {
                prtGRN |= (1 << GRN);
                prtRED &= ~(1 << RED);
            }
            
            else
            {
                prtGRN |= (1 << GRN);
                prtRED |= (1 << RED);
            }
        
            break;
    }
}

//Turn on ADC and get result.
unsigned int get_adc()
{   
    int ADCval;
    
    ADCSRA |= (1 << ADEN);                  //Begin conversion.
    ADCSRA |= (1 << ADSC);                  //
    
    while(!(ADCSRA & (1 << ADIF)));         //Wait for conversion to finish.
    
    ADCval = ADCL;                          //Get high and low conversion bytes.
    ADCval = (ADCH << 8) + ADCval;          //
    
    return ADCval;
}

//This function gets new temperature readings and updates the temperature histories.
void Get_Temperatures()
{
    if(tchannel1.tenabled) //Only get temperature if channel is active.
    {
        ADMUX  = 0x00; //ADC0 is input.
        ADMUX  |= (1 << REFS0) | (1 << REFS1); //ADC VREF is internal 1.1v source.
        ADMUX  &= ~(1 << ADLAR); //Make sure results are not left shifted.
        tchannel1.thistory[MAX_TEMPS - 1] = get_adc(); //Get temperature.
        Update_Temp_array(tchannel1.thistory);
        Update_Current_Temp(&tchannel1);
    }
    
    if(tchannel2.tenabled)
    {
        ADMUX  = 0x01; //ADC1 is input.
        ADMUX  |= (1 << REFS0) | (1 << REFS1); //ADC VREF is internal 1.1v source.
        ADMUX  &= ~(1 << ADLAR); //Make sure results are not left shifted.
        tchannel2.thistory[MAX_TEMPS - 1] = get_adc(); //Get temperature.
        Update_Temp_array(tchannel2.thistory);
        Update_Current_Temp(&tchannel2);
    }
    
    if(tchannel3.tenabled)
    {
        ADMUX  = 0x06; //ADC6 is input.
        ADMUX  |= (1 << REFS0) | (1 << REFS1); //ADC VREF is internal 1.1v source.
        ADMUX  &= ~(1 << ADLAR); //Make sure results are not left shifted.
        tchannel3.thistory[MAX_TEMPS - 1] = get_adc(); //Get temperature.
        Update_Temp_array(tchannel3.thistory);
        Update_Current_Temp(&tchannel3);
    }
    
    if(tchannel4.tenabled)
    {
        ADMUX  = 0x07; //ADC7 is input.
        ADMUX  |= (1 << REFS0) | (1 << REFS1); //ADC VREF is internal 1.1v source.
        ADMUX  &= ~(1 << ADLAR); //Make sure results are not left shifted.
        tchannel4.thistory[MAX_TEMPS - 1] = get_adc(); //Get temperature.
        Update_Temp_array(tchannel4.thistory);
        Update_Current_Temp(&tchannel4);
    }
}

//This function moves the current temperatures into the arrays and gets rid of the oldest one.
void Update_Temp_array(uint16_t *tarray)
{
    uint16_t array_index = 0;
    while(array_index < MAX_TEMPS - 1)
    {
        tarray[array_index] = tarray[array_index + 1];
        array_index++;
    }
}

//Use the average of the temperature histories to calculate the current temperature.
void Update_Current_Temp(tstruct *t)
{
    uint32_t temp = 0;
    uint16_t i;
    
    for(i = 0; i < MAX_TEMPS; i++)
    {
        temp += t->thistory[i];
    }
    
    temp /= MAX_TEMPS;
    
    //Do nothing if temperature is out of range.
    if(temp < ABSOLUTE_MIN || temp > ABSOLUTE_MAX)
    {
        ;
    }
    
    //Add calibration offset to current temperature.
    else if(t->cal_polarity == POSITIVE)
    {
        temp += t->cal_value;
    }
    
    else
    {
        temp -= t->cal_value;
    }
    
    t->this_temp = temp;
}

//This function checks the temperature values and sees if there is a deviation
//beyond the maximum delta t.  If so, it generates an error.
uint8_t Er_Check(uint16_t *arrayptr)
{
    uint16_t array_index = 0;
    
    while(array_index < MAX_TEMPS - 1)
    {
        if((arrayptr[array_index] > arrayptr[array_index + 1]) &&
        ((arrayptr[array_index] - arrayptr[array_index + 1]) >= system_struct.temp_err))
        {
            return ER_YES;
        }
        
        else if((arrayptr[array_index + 1] > arrayptr[array_index]) &&
        (arrayptr[array_index + 1] - arrayptr[array_index]) >= system_struct.temp_err)
        {
            return ER_YES;
        }
        
        array_index++;
    }
    
    return ER_NONE;
}

//This function is used to update the individual state machines. The guts start here.
void Update_State(tstruct *t)
{
    switch (t->this_state)
    {
        case STATE_NORMAL:
            t->timer = 0; //Reset trip timer.
            
            t->status = Er_Check(t->thistory); //Check for errors in the temperature history.
            if(t->status) //If error, goto error state.
            {
                t->next_state = STATE_ERROR;
                silenced = FALSE;
                break;
            }
            
            //If current temp is cold, move to cold minor state.
            if(t->this_temp <= system_struct.temp_cold)
            {
                t->next_state = STATE_COLD_MINOR;
                silenced = FALSE;
                break;
            }
            
            //If current temp is slightly warm, move to hot minor state.
            if(t->this_temp >= system_struct.temp_min)
            {
                t->next_state = STATE_HOT_MINOR;
                silenced = FALSE;
                break;
            }
            
            t->next_state = STATE_NORMAL; //Else stay in normal state.          
            break;
        
        case STATE_COLD_MINOR:
            t->timer = 0; //Reset trip timer.
            
            if(error < ERR_MIN) //If no system errors exist, set to minor error.
            {
                error = ERR_MIN;
            }
            
            t->status = Er_Check(t->thistory); //Check for errors in the temperature history.
            if(t->status) //If error, goto error state.
            {
                silenced = FALSE;
                t->next_state = STATE_ERROR;
                break;
            }
            
            //If current temp is normal, move to normal state.
            if(t->this_temp > (system_struct.temp_cold + STATE_HYS))
            {
                t->next_state = STATE_NORMAL;
                break;
            }
            
            t->next_state = STATE_COLD_MINOR; //Else stay in cold minor state.          
            break;
        
        case STATE_HOT_MINOR:
            t->timer = 0; //Reset trip timer.
            
            if(error < ERR_MIN) //If no system errors exist, set to minor error.
            {
                error = ERR_MIN;
            }
            
            t->status = Er_Check(t->thistory); //Check for errors in the temperature history.
            if(t->status) //If error, goto error state.
            {
                silenced = FALSE;
                t->next_state = STATE_ERROR;
                break;
            }
            
            //If current temp is normal, move to normal state.
            if(t->this_temp < (system_struct.temp_min - STATE_HYS))
            {
                t->next_state = STATE_NORMAL;
                break;
            }
            
            //If current temperature is hot, move to state hot major.
            if(t->this_temp >= system_struct.temp_maj)
            {
                silenced = FALSE;
                t->next_state = STATE_HOT_MAJOR;
                break;
            }
            
            t->next_state = STATE_HOT_MINOR; //Else stay in hot minor state.
            break;
        
        case STATE_HOT_MAJOR:
            t->timer = 0; //Reset trip timer.
            
            if(error < ERR_MAJ) //If no system errors above minor error, set to major error.
            {
                error = ERR_MAJ;
            }
            
            t->status = Er_Check(t->thistory); //Check for errors in the temperature history.
            if(t->status) //If error, goto error state.
            {
                t->next_state = STATE_ERROR;
                break;
            }
            
            //If current temp is warm, move to state hot minor.
            if(t->this_temp < (system_struct.temp_maj - STATE_HYS))
            {
                t->next_state = STATE_HOT_MINOR;
                break;
            }
            
            //If current temperature is critical, move to state trip timer.
            if(t->this_temp >= system_struct.temp_trip)
            {
                silenced = FALSE;
                latched = TRUE;
                t->next_state = STATE_TRIP_TIMER;
                break;
            }
            
            t->next_state = STATE_HOT_MAJOR; //Else stay in hot major state.
            break;
        
        case STATE_TRIP_TIMER:
            (t->timer)++; //Increment trip timer.
            
            error = ERR_TRIP; //Set highest system error level.
            
            t->status = Er_Check(t->thistory); //Check for errors in the temperature history.
            if(t->status) //If error, goto error state.
            {
                t->next_state = STATE_ERROR;
                break;
            }
            
            //Goto error state if temperature is too high.
            if(t->this_temp >= system_struct.temp_high)
            {
                t->next_state = STATE_ERROR;
                break;
            }
            
            //If current temp is hot, move to state hot major.
            if(t->this_temp < (system_struct.temp_trip - STATE_HYS))
            {
                t->next_state = STATE_HOT_MAJOR;
                break;
            }
            
            //If timer has reached its limit, move to trip state.
            if(t->timer >= TRIP_COUNT)
            {
                silenced = FALSE;
                t->next_state = STATE_TRIP;
                break;
            }

            t->next_state = STATE_TRIP_TIMER; //Else stay in trip timer state.
            break;
        
        case STATE_TRIP:
            t->timer = 0; //Reset trip timer.
            
            error = ERR_TRIP; //Set highest system error level.
            
            t->status = Er_Check(t->thistory); //Check for errors in the temperature history.
            if(t->status) //If error, goto error state.
            {
                t->next_state = STATE_ERROR;
                break;
            }
            
            //Goto error state if temperature is too high.
            if(t->this_temp >= system_struct.temp_high)
            {
                t->next_state = STATE_ERROR;
                break;
            }
            
            //If current temp is hot, move to state hot major.
            if(t->this_temp < (system_struct.temp_trip - STATE_HYS))
            {
                t->next_state = STATE_HOT_MAJOR;
                break;
            }       
            
            t->next_state = STATE_TRIP; //Else stay in trip state.
            break;      
        
        case STATE_ERROR:
        default:
            t->timer = 0; //Reset trip timer.
            
            if(error < ERR_MIN) //If no system errors exist, set to minor error.
            {
                error = ERR_MIN;
            }
            
            t->status = Er_Check(t->thistory); //Check again for errors in the temperature history.
            if(t->status) //If errors, stay in error state.
            {
                t->next_state = STATE_ERROR;
                break;
            }
            
            //Goto error state if temperature is too high.
            if(t->this_temp >= system_struct.temp_high)
            {
                t->next_state = STATE_ERROR;
                break;
            }
            
            t->next_state = STATE_NORMAL; //Else return to normal state.            
            break;
    }
}

//This function is the entry point to the state machines for the box.
void Do_State()
{
    //***Temperature channel 1***
    if(tchannel1.tenabled)
    {
        Update_State(&tchannel1);
    
        //energize/de-energize relays based on temperature states.
        if(tchannel1.this_state == STATE_TRIP)
        {
            Do_Trip(&tchannel1);    //Use trip mask to trip corresponding relays.
        }
        
        else
        {
            Undo_Trip(&tchannel1);  //Use trip mask to reset tripped relays.
        }
    
        //Move to next state.
        tchannel1.this_state = tchannel1.next_state;
    }   
    
    //***Temperature channel 2***
    if(tchannel2.tenabled)
    {
        Update_State(&tchannel2);
    
        //energize/de-energize relays based on temperature states.
        if(tchannel2.this_state == STATE_TRIP)
        {
            Do_Trip(&tchannel2);    //Use trip mask to trip corresponding relays.
        }
        
        else
        {
            Undo_Trip(&tchannel2);  //Use trip mask to reset tripped relays.
        }
    
        //Move to next state.
        tchannel2.this_state = tchannel2.next_state;
    }   
    
    //***Temperature channel 3***
    if(tchannel3.tenabled)
    {
        Update_State(&tchannel3);
    
        //energize/de-energize relays based on temperature states.
        if(tchannel3.this_state == STATE_TRIP)
        {
            Do_Trip(&tchannel3);    //Use trip mask to trip corresponding relays.
        }
        
        else
        {
            Undo_Trip(&tchannel3);  //Use trip mask to reset tripped relays.
        }
    
        //Move to next state.
        tchannel3.this_state = tchannel3.next_state;
    }   
    
    //***Temperature channel 4***
    if(tchannel4.tenabled)
    {
        Update_State(&tchannel4);
    
        //energize/de-energize relays based on temperature states.
        if(tchannel4.this_state == STATE_TRIP)
        {
            Do_Trip(&tchannel4);    //Use trip mask to trip corresponding relays.
        }
        
        else
        {
            Undo_Trip(&tchannel4);  //Use trip mask to reset tripped relays.
        }
    
        //Move to next state.
        tchannel4.this_state = tchannel4.next_state;
    }           
}

//This function uses the temperature channel trip_mask byte to determine which
//relays to trip when a thermal runaway is detected.
void Do_Trip(tstruct *t)
{
    if(t->trip_mask & RELAY1_BIT_MASK)
    {
        prtRLY1 |= (1 << RLY1);
    }
    
    if(t->trip_mask & RELAY2_BIT_MASK)
    {
        prtRLY2 |= (1 << RLY2);
    }
    
    if(t->trip_mask & RELAY3_BIT_MASK)
    {
        prtRLY3 |= (1 << RLY3);
    }
    
    if(t->trip_mask & RELAY4_BIT_MASK)
    {
        prtRLY4 |= (1 << RLY4);
    }
}

//This function uses the temperature channel trip_mask byte to reset
//relays that tripped when a thermal runaway was detected but has cleared.
void Undo_Trip(tstruct *t)
{
    if(t->trip_mask & RELAY1_BIT_MASK)
    {
        prtRLY1 &= ~(1 << RLY1);
    }
    
    if(t->trip_mask & RELAY2_BIT_MASK)
    {
        prtRLY2 &= ~(1 << RLY2);
    }
    
    if(t->trip_mask & RELAY3_BIT_MASK)
    {
        prtRLY3 &= ~(1 << RLY3);
    }
    
    if(t->trip_mask & RELAY4_BIT_MASK)
    {
        prtRLY4 &= ~(1 << RLY4);
    }
}

//This function displays the temperature on the LCD.
void Display_Temp(tstruct *t)
{
    char tarray[4];
    float temp;
    
    //Check if an error exists.
    if(t->status)
    {
        write_string("ERR ");
        return;
    }
    
    //Check if temperature is out of range low.
    if(t->this_temp <= system_struct.temp_low)
    {
        write_string("LOW ");
        return;
    }   
    
    //Check if temperature is out of range high.
    if(t->this_temp >= system_struct.temp_high)
    {
        write_string("HI  ");
        return;
    }
    
    //Display Celsius temperature.
    if(system_struct.temp_c_or_f == CELSIUS)
    {
        temp = Celsius_From_ADC(t->this_temp);
        String_From_Float(temp, tarray, 0);
        write_string(tarray);
        write_string("C ");
    }
    
    //Display Fahrenheit temperature.
    if(system_struct.temp_c_or_f == FAHRENHEIT)
    {
        temp = Farenheit_From_ADC(t->this_temp);
        String_From_Float(temp, tarray, 0);
        write_string(tarray);
        
        //Check if temperature is above 100F.
        if(tarray[2])
        {
            write_string("F");  //If so, remove pad character.
        }
        
        else
        {
            write_string("F "); //Add pad character.
        }
    }           
}

//This function updates the LCD every frame.
void Update_LCD()
{
    //Display number of temp sensors.
    write_command(0x02); //Goto beginning of first line.
    write_string("                "); //Erase line
    write_command(0x02); //Goto beginning of first line.
        
    if(tchannel1.tenabled)
    {
        write_string("T1  " );
    }   
        
    if(tchannel2.tenabled)
    {
        write_string("T2  " );
    }
        
    if(tchannel3.tenabled)
    {
        write_string("T3  " );
    }   
        
    if(tchannel4.tenabled)
    {
        write_string("T4  " );
    }
    
    write_command(0xC0); //Goto beginning of second line.
    write_string("                           "); //Erase line
    write_command(0xC0); //Return to beginning of second line.
        
    //Display temperatures on the LCD.
    if(tchannel1.tenabled)
    {
        Display_Temp(&tchannel1);
    }
            
    if(tchannel2.tenabled)
    {
        Display_Temp(&tchannel2);
    }
        
    if(tchannel3.tenabled)
    {
        Display_Temp(&tchannel3);
    }
        
    if(tchannel4.tenabled)
    {
        Display_Temp(&tchannel4);
    }   
}

int main(void)
{
    //Wait for everything to stabilize before startup.
    Wait_ms(250);
    
    //EEPROM_Reset_to_Default(); //Set all set points to their default values.
    EEPROM_Read_All(); //Load all set points from the EEPROM.
    
    init_micro();
    init_lcd();
    
    while(1)
    {
        //Reset error status every cycle.
        error = ERR_NONE;
        
        //Get temperatures and update temperature arrays.
        Get_Temperatures();
        
        //Update the temperature sensor states.
        Do_State();
        
        //Update LCD display.
        Update_LCD();
        
        //Update LED and buzzer status.
        Update_LEDS_and_Buzzer();
        
        Wait_ms(100);   
    }
}

//Interrupt based USART RX function.
ISR(USART_RX_vect)
{
    uint8_t rx_byte;
    
    rx_byte = UDR0; //Get RX byte.
    TX_Byte(rx_byte); //Echo byte.
    USART_Tokenizer(rx_byte); //Send byte to tokenizer.
}

