/*
 * USART_Control.c
 *
 * Created: 12/29/2013 4:30:55 PM
 *  Author: Nick Mikstas
 */

#include "USART_Control.h"

//This function transmits a single byte to the USART.
void TX_Byte(uint8_t data)
{
    while(!(UCSR0A & (1 << UDRE0))); //Wait for buffer to be free.
    UDR0 = data;
}

//This function transmits a string of data out the UART.
void TX_String(char *data)
{
    uint8_t tx_string_index = 0;
    
    while(data[tx_string_index])
    {
        TX_Byte(data[tx_string_index++]);   //Transmit byte.
    }
    
}

//This function prints the hex value of a number out the USART.
void TX_Hex(uint8_t hex_byte)
{
    uint8_t temp;
    
    temp   = hex_byte;
    temp >>= 4;
    
    if(temp > 0x09)
        TX_Byte(temp + 0x37);
    else
        TX_Byte(temp + 0x30);
    
    temp  = hex_byte;
    temp &= 0x0F;
    
    if(temp > 0x09)
        TX_Byte(temp + 0x37);
    else
        TX_Byte(temp + 0x30);   
}

//Write the item number in the terminal menu.
void USART_Item_Number(const uint8_t number)
{
    char info[4];
    String_From_UInt(number, info);
    TX_String(info);
    TX_String(". ");
}

//Write a number in the terminal menu.
void USART_Number(const uint16_t number)
{
    char info[5];
    String_From_UInt(number, info);
    TX_String(info);
}

//Write "Calibration (+/-) (0 to 99)".
void USART_Cal()
{
    TX_String("Calibration (+/-) (0 to 99)");
}

//Write "Channel " in the terminal window.
void USART_Channel()
{
    TX_String("Channel ");
}

//Write " (E or D): " in the terminal window.
void USART_E_or_D()
{
    TX_String(" (E or D): ");
}

//Write "Enabled\r" or "Disabled\r" in the terminal window.
void USART_En_or_Dis(const uint8_t tenabled)
{
    if(tenabled)
    {
        TX_String("Enabled\r");
    }
    
    else
    {
        TX_String("Disabled\r");
    }
}

//Write "\r" in the terminal window.
void USART_Carriage_Return()
{
    TX_Byte('\r');
}

//Write "Trip Mask " in the terminal window.
void USART_Trip_Mask()
{
    TX_String("Trip Mask ");
}

//Write " (00 Through 0F): " in the terminal window.
void USART_00_to_0F()
{
    TX_String(" (0x00 to 0x0F): ");
}

//Write "Temperature Setpoint " in the terminal window.
void USART_Temp_Set()
{
    TX_String("Temperature Setpoint ");
}

//Write "T" in terminal window.
void USART_T()
{
    TX_Byte('T');
}

//Write ":" in terminal window.
void USART_Colon()
{
    TX_Byte(':');
}

//Write " " in terminal window.
void USART_Space()
{
    TX_Byte(' ');
}

//Write absolute min value to absolute max value in terminal window.
void USART_Min_Max()
{
    char info[5];
    float temp;
    
    TX_String(" (");
    
    if(system_struct.temp_c_or_f == CELSIUS)
    {
        temp = Celsius_From_ADC(ABSOLUTE_MIN);      
        String_From_Float(temp, info, 0);   //Display as integer.
        TX_String(info);
        TX_String(" to ");
        temp = Celsius_From_ADC(ABSOLUTE_MAX);      
        String_From_Float(temp, info, 0);   //Display as integer.
        TX_String(info);
        TX_String(")");
    }
    
    else
    {
        temp = Farenheit_From_ADC(ABSOLUTE_MIN);        
        String_From_Float(temp, info, 0);   //Display as integer.
        TX_String(info);
        TX_String(" to ");
        temp = Farenheit_From_ADC(ABSOLUTE_MAX);        
        String_From_Float(temp, info, 0);   //Display as integer.
        TX_String(info);
        TX_String(")");
    }
}

//Write a Celsius or Fahrenheit integer in the terminal window.
void USART_Temp_Show(uint16_t adc, uint8_t precision)
{
    char info[5];
    float temp;
    
    if(system_struct.temp_c_or_f == CELSIUS)
    {
        temp = Celsius_From_ADC(adc);
    }
    
    else
    {
        temp = Farenheit_From_ADC(adc);
    }
    
    //Display as integer.
    String_From_Float(temp, info, precision);   
    
    TX_String(info);
    
    if(system_struct.temp_c_or_f == CELSIUS)
    {
        TX_Byte('C');
    }
    
    else
    {
        TX_Byte('F');
    }
}

//Write "Alarm" in the terminal window.
void USART_Alarm()
{
    TX_String("Alarm");
}

//Write calibration value to terminal.
void USART_Print_Cal(const uint8_t polarity, const uint8_t value)
{
    char cal_value[4];
    
    //Print plus or minus.
    if(polarity == POSITIVE)
    {
        TX_String("+ ");
    }
    
    else
    {
        TX_String("- ");
    }
    
    //Print calibration value.
    String_From_UInt((uint16_t)value, cal_value);
    TX_String(cal_value);
}

//Write the alarm state in the terminal window.
void USART_Alarm_State(uint8_t en, uint16_t this_temp, uint8_t this_state)
{
    //Check if channel is enabled.
    if(!en)
    {
        TX_String("N/A ");
        return;
    }
    
    //Check if temperature is out of range low.
    if(this_temp <= system_struct.temp_low)
    {
        TX_String("LOW ");
        return;
    }
    
    //Check if temperature is out of range high.
    if(this_temp >= system_struct.temp_high)
    {
        TX_String("HIGH ");
        return;
    }
    
    //Check if in low temp state.
    if(this_state == STATE_COLD_MINOR)
    {
        TX_String("COLD ");
        return;
    }
    
    //Check if in warm temp state.
    if(this_state == STATE_HOT_MINOR)
    {
        TX_String("WARM ");
        return;
    }
    
    //Check if in hot temp state.
    if(this_state == STATE_HOT_MAJOR)
    {
        TX_String("HOT ");
        return;
    }
    
    //Check if in trip timer temp state.
    if(this_state == STATE_TRIP_TIMER)
    {
        TX_String("TRIP TIMER ");
        return;
    }
    
    //Check if in trip temp state.
    if(this_state == STATE_TRIP)
    {
        TX_String("TRIP ");
        return;
    }
    
    //Check if in error temp state.
    if(this_state == STATE_ERROR)
    {
        TX_String("ERROR ");
        return;
    }
    
    TX_String("NONE ");
}

//Print "ERR" in terminal window.
void USART_ERR()
{
    TX_String("ERR");
}

//Print "OK" in terminal window.
void USART_OK()
{
    TX_String("OK");
}

//Write the configuration menu in the terminal window.
void USART_Write_Menu()
{   
    //Send status of channel 1.
    USART_Item_Number(1);
    USART_Channel();
    USART_Number(1);
    USART_E_or_D();
    USART_En_or_Dis(tchannel1.tenabled);
    
    //Send status of channel 2.
    USART_Item_Number(2);
    USART_Channel();
    USART_Number(2);
    USART_E_or_D();
    USART_En_or_Dis(tchannel2.tenabled);
    
    //Send status of channel 3.
    USART_Item_Number(3);
    USART_Channel();
    USART_Number(3);
    USART_E_or_D();
    USART_En_or_Dis(tchannel3.tenabled);
    
    //Send status of channel 4.
    USART_Item_Number(4);
    USART_Channel();
    USART_Number(4);
    USART_E_or_D();
    USART_En_or_Dis(tchannel4.tenabled);
    
    //Calibration for channel 1.
    USART_Item_Number(5);
    USART_Channel();
    USART_Number(1);
    USART_Space();
    USART_Cal();
    USART_Colon();
    USART_Space();
    USART_Print_Cal(tchannel1.cal_polarity, tchannel1.cal_value);
    USART_Carriage_Return();
    
    //Calibration for channel 2.
    USART_Item_Number(6);
    USART_Channel();
    USART_Number(2);
    USART_Space();
    USART_Cal();
    USART_Colon();
    USART_Space();
    USART_Print_Cal(tchannel2.cal_polarity, tchannel2.cal_value);
    USART_Carriage_Return();
    
    //Calibration for channel 3.
    USART_Item_Number(7);
    USART_Channel();
    USART_Number(3);
    USART_Space();
    USART_Cal();
    USART_Colon();
    USART_Space();
    USART_Print_Cal(tchannel3.cal_polarity, tchannel3.cal_value);
    USART_Carriage_Return();
    
    //Calibration for channel 4.
    USART_Item_Number(8);
    USART_Channel();
    USART_Number(4);
    USART_Space();
    USART_Cal();
    USART_Colon();
    USART_Space();
    USART_Print_Cal(tchannel4.cal_polarity, tchannel4.cal_value);
    USART_Carriage_Return();
    
    //Send trip mask for channel1.
    USART_Item_Number(9);
    USART_Trip_Mask();
    USART_Channel();
    USART_Number(1);
    USART_00_to_0F();
    TX_Hex(tchannel1.trip_mask);
    USART_Carriage_Return();
    
    //Send trip mask for channel2.
    USART_Item_Number(10);
    USART_Trip_Mask();
    USART_Channel();
    USART_Number(2);
    USART_00_to_0F();
    TX_Hex(tchannel2.trip_mask);
    USART_Carriage_Return();
    
    //Send trip mask for channel3.
    USART_Item_Number(11);
    USART_Trip_Mask();
    USART_Channel();
    USART_Number(3);
    USART_00_to_0F();
    TX_Hex(tchannel3.trip_mask);
    USART_Carriage_Return();
    
    //Send trip mask for channel4.
    USART_Item_Number(12);
    USART_Trip_Mask();
    USART_Channel();
    USART_Number(4);
    USART_00_to_0F();
    TX_Hex(tchannel4.trip_mask);
    USART_Carriage_Return();
    
    //Send units of measure.
    USART_Item_Number(13);
    TX_String("Units of Measure (C or F): ");
    if(system_struct.temp_c_or_f == CELSIUS)
    {
        TX_String("Celsius\r");
    }
    else
    {
        TX_String("Fahrenheit\r");
    }
    
    //Send low temperature value.
    USART_Item_Number(14);
    USART_Temp_Set();
    TX_String("LOW");
    USART_Min_Max();
    USART_Colon();
    USART_Space();
    USART_Temp_Show(system_struct.temp_low, 0);
    USART_Carriage_Return();
    
    //Send cold temperature value.
    USART_Item_Number(15);
    USART_Temp_Set();
    TX_String("COLD");
    USART_Min_Max();
    USART_Colon();
    USART_Space();
    USART_Temp_Show(system_struct.temp_cold, 0);
    USART_Carriage_Return();
    
    //Send warm temperature value.
    USART_Item_Number(16);
    USART_Temp_Set();
    TX_String("WARM");
    USART_Min_Max();
    USART_Colon();
    USART_Space();
    USART_Temp_Show(system_struct.temp_min, 0);
    USART_Carriage_Return();
    
    //Send hot temperature value.
    USART_Item_Number(17);
    USART_Temp_Set();
    TX_String("HOT");
    USART_Min_Max();
    USART_Colon();
    USART_Space();
    USART_Temp_Show(system_struct.temp_maj, 0);
    USART_Carriage_Return();
    
    //Send trip temperature value.
    USART_Item_Number(18);
    USART_Temp_Set();
    TX_String("TRIP");
    USART_Min_Max();
    USART_Colon();
    USART_Space();
    USART_Temp_Show(system_struct.temp_trip, 0);
    USART_Carriage_Return();
    
    //Send high temperature value.
    USART_Item_Number(19);
    USART_Temp_Set();
    TX_String("HIGH");
    USART_Min_Max();
    USART_Colon();
    USART_Space();
    USART_Temp_Show(system_struct.temp_high, 0);
    USART_Carriage_Return();
    
    //Send error temperature value.
    USART_Item_Number(20);
    USART_Temp_Set();
    TX_String("ERROR (1 to 999)");
    USART_Colon();
    USART_Space();
    USART_Number(system_struct.temp_err);
    USART_Carriage_Return();
    
    //Send "reset to factory default" message.
    USART_Item_Number(21);
    TX_String("Reset to Factory Defaults");
    USART_Carriage_Return();
    USART_Carriage_Return();
    
    //Send current temperature readings.
    USART_T();
    USART_Number(1);
    USART_Colon();
    USART_Temp_Show(tchannel1.this_temp, 1);
    USART_Space();
    USART_T();
    USART_Number(2);
    USART_Colon();
    USART_Temp_Show(tchannel2.this_temp, 1);
    USART_Space();
    USART_T();
    USART_Number(3);
    USART_Colon();
    USART_Temp_Show(tchannel3.this_temp, 1);
    USART_Space();
    USART_T();
    USART_Number(4);
    USART_Colon();
    USART_Temp_Show(tchannel4.this_temp, 1);
    USART_Space();
    USART_Carriage_Return();
    
    //Send channel current statuses.
    USART_Alarm();
    USART_Number(1);
    USART_Colon();
    USART_Space();
    USART_Alarm_State(tchannel1.tenabled, tchannel1.this_temp, tchannel1.this_state);
    USART_Alarm();
    USART_Number(2);
    USART_Colon();
    USART_Space();
    USART_Alarm_State(tchannel2.tenabled, tchannel2.this_temp, tchannel2.this_state);
    USART_Carriage_Return();
    USART_Alarm();
    USART_Number(3);
    USART_Colon();
    USART_Space();
    USART_Alarm_State(tchannel3.tenabled, tchannel3.this_temp, tchannel3.this_state);
    USART_Alarm();
    USART_Number(4);
    USART_Colon();
    USART_Space();
    USART_Alarm_State(tchannel4.tenabled, tchannel4.this_temp, tchannel4.this_state);
    USART_Carriage_Return();    
    
    //Send Firmware version.
    TX_String("Firmware Revision: ");
    USART_Number(FIRMWARE_MAJOR);
    TX_Byte('.');
    USART_Number(FIRMWARE_MINOR);
    USART_Carriage_Return();    
}

//This function tokenizes the user input.
void USART_Tokenizer(uint8_t rx_byte)
{
    static uint8_t rx_array[RX_ARRAY_LENGTH];
    static uint8_t array_index;
    uint8_t errno;
    
    //Get byte from USART.
    rx_array[array_index++] = rx_byte;
    
    //Check if array is going to overflow.
    if((array_index >= RX_ARRAY_LENGTH) && (rx_array[RX_ARRAY_LENGTH - 1] != 0x0D))
    {
        array_index = 0;
        USART_Carriage_Return();
        USART_ERR();
        USART_Carriage_Return();
        return;
    }
    
    //Special case. empty array prints the menu.
    if(rx_array[0] == 0x0D)
    {
        array_index = 0;
        USART_Write_Menu();
        return;
    }
    
    //If enter not pressed, keep building array.
    if(rx_array[array_index - 1] != 0x0D)
    {
        return;
    }
    
    //At this point, enter has been pressed and array is ready for processing.  
    
    //Convert array hex values to uppercase.
    for(int i = 0; i < array_index; i++)
    {
        if(rx_array[i] >= 0x61 && rx_array[i] < 0x67)
        {
            rx_array[i] -= 0x20;
        }
    }
    
    //Reset array index for next input string.
    array_index = 0;
    
    //Do a quick error check on first character in array.
    if(rx_array[0] < 0x30) //< 0.
    {
        USART_ERR();
        USART_Carriage_Return();
        return;
    }
    
    if(rx_array[0] > 0x46) //< F.
    {
        USART_ERR();
        USART_Carriage_Return();
        return;
    }
    
    if(rx_array[0] > 0x39 && rx_array[0] < 0x41) //> 9, < A.
    {
        USART_ERR();
        USART_Carriage_Return();
        return;
    }
    
    //Get number selection from user input and call appropriate function.
    if(rx_array[0] == '1' && rx_array[1] == ' ')
    {
        errno = Selection1(&rx_array[2]);
    }
    
    else if(rx_array[0] == '2' && rx_array[1] == ' ')
    {
        errno = Selection2(&rx_array[2]);
    }
    
    else if(rx_array[0] == '3' && rx_array[1] == ' ')
    {
        errno = Selection3(&rx_array[2]);
    }
    
    else if(rx_array[0] == '4' && rx_array[1] == ' ')
    {
        errno = Selection4(&rx_array[2]);
    }
    
    else if(rx_array[0] == '5' && rx_array[1] == ' ')
    {
        errno = Selection5(&rx_array[2]);
    }
    
    else if(rx_array[0] == '6' && rx_array[1] == ' ')
    {
        errno = Selection6(&rx_array[2]);
    }
    
    else if(rx_array[0] == '7' && rx_array[1] == ' ')
    {
        errno = Selection7(&rx_array[2]);
    }
    
    else if(rx_array[0] == '8' && rx_array[1] == ' ')
    {
        errno = Selection8(&rx_array[2]);
    }
    
    else if(rx_array[0] == '9' && rx_array[1] == ' ')
    {
        errno = Selection9(&rx_array[2]);
    }
    
    else if(rx_array[0] == '1' && rx_array[1] == '0' && rx_array[2] == ' ')
    {
        errno = Selection10(&rx_array[3]);
    }
    
    else if(rx_array[0] == '1' && rx_array[1] == '1' && rx_array[2] == ' ')
    {
        errno = Selection11(&rx_array[3]);
    }
    
    else if(rx_array[0] == '1' && rx_array[1] == '2' && rx_array[2] == ' ')
    {
        errno = Selection12(&rx_array[3]);
    }
    
    else if(rx_array[0] == '1' && rx_array[1] == '3' && rx_array[2] == ' ')
    {
        errno = Selection13(&rx_array[3]);
    }
    
    else if(rx_array[0] == '1' && rx_array[1] == '4' && rx_array[2] == ' ')
    {
        errno = Selection14(&rx_array[3]);
    }
    
    else if(rx_array[0] == '1' && rx_array[1] == '5' && rx_array[2] == ' ')
    {
        errno = Selection15(&rx_array[3]);
    }
    
    else if(rx_array[0] == '1' && rx_array[1] == '6' && rx_array[2] == ' ')
    {
        errno = Selection16(&rx_array[3]);
    }
    
    else if(rx_array[0] == '1' && rx_array[1] == '7' && rx_array[2] == ' ')
    {
        errno = Selection17(&rx_array[3]);
    }
    
    else if(rx_array[0] == '1' && rx_array[1] == '8' && rx_array[2] == ' ')
    {
        errno = Selection18(&rx_array[3]);
    }
    
    else if(rx_array[0] == '1' && rx_array[1] == '9' && rx_array[2] == ' ')
    {
        errno = Selection19(&rx_array[3]);
    }
    
    else if(rx_array[0] == '2' && rx_array[1] == '0' && rx_array[2] == ' ')
    {
        errno = Selection20(&rx_array[3]);
    }
    
    else if(rx_array[0] == '2' && rx_array[1] == '1' && rx_array[2] == 0x0D)
    {
        errno = Selection21();
    }
    
    else //Invalid selection.       
    {
        errno = ERROR;
    }
    
    if(errno)
    {
        USART_ERR();
        USART_Carriage_Return();
        return;
    }
    
    USART_OK();
    USART_Carriage_Return();
    return;
}

//Menu selection 1.
uint8_t Selection1(uint8_t *rx_array)
{
    if(rx_array[0] == 'D' && rx_array[1] == 0x0D) //Disable channel.
    {
        EEPROM_Write_Word((uint16_t *)CH_1_ENABLED, DISABLED);
        tchannel1.tenabled = EEPROM_Read_Word((uint16_t *)CH_1_ENABLED);
        return NO_ERROR;
    }
    
    if(rx_array[0] == 'E' && rx_array[1] == 0x0D) //Enable channel.
    {
        EEPROM_Write_Word((uint16_t *)CH_1_ENABLED, ENABLED);
        tchannel1.tenabled = EEPROM_Read_Word((uint16_t *)CH_1_ENABLED);
        return NO_ERROR;
    }
    
    return ERROR; //Unrecognized parameter.
}

//Menu selection 2.
uint8_t Selection2(uint8_t *rx_array)
{
    if(rx_array[0] == 'D' && rx_array[1] == 0x0D) //Disable channel.
    {
        EEPROM_Write_Word((uint16_t *)CH_2_ENABLED, DISABLED);
        tchannel2.tenabled = EEPROM_Read_Word((uint16_t *)CH_2_ENABLED);
        return NO_ERROR;
    }
    
    if(rx_array[0] == 'E' && rx_array[1] == 0x0D) //Enable channel.
    {
        EEPROM_Write_Word((uint16_t *)CH_2_ENABLED, ENABLED);
        tchannel2.tenabled = EEPROM_Read_Word((uint16_t *)CH_2_ENABLED);
        return NO_ERROR;
    }
    
    return ERROR; //Unrecognized parameter.
}

//Menu selection 3.
uint8_t Selection3(uint8_t *rx_array)
{
    if(rx_array[0] == 'D' && rx_array[1] == 0x0D) //Disable channel.
    {
        EEPROM_Write_Word((uint16_t *)CH_3_ENABLED, DISABLED);
        tchannel3.tenabled = EEPROM_Read_Word((uint16_t *)CH_3_ENABLED);
        return NO_ERROR;
    }
    
    if(rx_array[0] == 'E' && rx_array[1] == 0x0D) //Enable channel.
    {
        EEPROM_Write_Word((uint16_t *)CH_3_ENABLED, ENABLED);
        tchannel3.tenabled = EEPROM_Read_Word((uint16_t *)CH_3_ENABLED);
        return NO_ERROR;
    }
    
    return ERROR; //Unrecognized parameter.
}

//Menu selection 4.
uint8_t Selection4(uint8_t *rx_array)
{
    if(rx_array[0] == 'D' && rx_array[1] == 0x0D) //Disable channel.
    {
        EEPROM_Write_Word((uint16_t *)CH_4_ENABLED, DISABLED);
        tchannel4.tenabled = EEPROM_Read_Word((uint16_t *)CH_4_ENABLED);
        return NO_ERROR;
    }
    
    if(rx_array[0] == 'E' && rx_array[1] == 0x0D) //Enable channel.
    {
        EEPROM_Write_Word((uint16_t *)CH_4_ENABLED, ENABLED);
        tchannel4.tenabled = EEPROM_Read_Word((uint16_t *)CH_4_ENABLED);
        return NO_ERROR;
    }
    
    return ERROR; //Unrecognized parameter.
}


//Menu selection 5.
uint8_t Selection5(uint8_t *rx_array)
{
    uint16_t polarity;
    uint16_t value;
    
    //Check polarity
    if(rx_array[0] == '+' && rx_array[1] == ' ')
    {
        polarity = POSITIVE;
    }
    
    else if(rx_array[0] == '-' && rx_array[1] == ' ')
    {
        polarity = NEGATIVE;
    }
    
    else //Invalid input.
    {
        return ERROR;
    }
    
    //Get calibration value.    
    if(rx_array[3] == 0x0D) //1 digit.
    {
        //Do conversion and check for success.
        if(Convert_1_Num(&rx_array[2], &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Range check value.
        if(value > 99)
        {
            return ERROR;
        }
        
        //Save value.
        EEPROM_Write_Word((uint16_t *)CH_1_CAL, value);
        EEPROM_Write_Word((uint16_t *)CH_1_POLARITY, polarity);
        tchannel1.cal_value = EEPROM_Read_Word((uint16_t *)CH_1_CAL);
        tchannel1.cal_polarity = EEPROM_Read_Word((uint16_t *)CH_1_POLARITY);
        
        return NO_ERROR;    
    }
    
    //Get calibration value.    
    if(rx_array[4] == 0x0D) //1 digit.
    {
        //Do conversion and check for success.
        if(Convert_2_Num(&rx_array[2], &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Range check value.
        if(value > 99)
        {
            return ERROR;
        }
        
        //Save value.
        EEPROM_Write_Word((uint16_t *)CH_1_CAL, value);
        EEPROM_Write_Word((uint16_t *)CH_1_POLARITY, polarity);
        tchannel1.cal_value = EEPROM_Read_Word((uint16_t *)CH_1_CAL);
        tchannel1.cal_polarity = EEPROM_Read_Word((uint16_t *)CH_1_POLARITY);
        
        return NO_ERROR;    
    }
            
    return ERROR;
}

//Menu selection 6.
uint8_t Selection6(uint8_t *rx_array)
{
    uint16_t polarity;
    uint16_t value;
    
    //Check polarity
    if(rx_array[0] == '+' && rx_array[1] == ' ')
    {
        polarity = POSITIVE;
    }
    
    else if(rx_array[0] == '-' && rx_array[1] == ' ')
    {
        polarity = NEGATIVE;
    }
    
    else //Invalid input.
    {
        return ERROR;
    }
    
    //Get calibration value.    
    if(rx_array[3] == 0x0D) //1 digit.
    {
        //Do conversion and check for success.
        if(Convert_1_Num(&rx_array[2], &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Range check value.
        if(value > 99)
        {
            return ERROR;
        }
        
        //Save value.
        EEPROM_Write_Word((uint16_t *)CH_2_CAL, value);
        EEPROM_Write_Word((uint16_t *)CH_2_POLARITY, polarity);
        tchannel2.cal_value = EEPROM_Read_Word((uint16_t *)CH_2_CAL);
        tchannel2.cal_polarity = EEPROM_Read_Word((uint16_t *)CH_2_POLARITY);
        
        return NO_ERROR;    
    }
    
    //Get calibration value.    
    if(rx_array[4] == 0x0D) //1 digit.
    {
        //Do conversion and check for success.
        if(Convert_2_Num(&rx_array[2], &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Range check value.
        if(value > 99)
        {
            return ERROR;
        }
        
        //Save value.
        EEPROM_Write_Word((uint16_t *)CH_2_CAL, value);
        EEPROM_Write_Word((uint16_t *)CH_2_POLARITY, polarity);
        tchannel2.cal_value = EEPROM_Read_Word((uint16_t *)CH_2_CAL);
        tchannel2.cal_polarity = EEPROM_Read_Word((uint16_t *)CH_2_POLARITY);
        
        return NO_ERROR;    
    }
            
    return ERROR;
}

//Menu selection 7.
uint8_t Selection7(uint8_t *rx_array)
{
    uint16_t polarity;
    uint16_t value;
    
    //Check polarity
    if(rx_array[0] == '+' && rx_array[1] == ' ')
    {
        polarity = POSITIVE;
    }
    
    else if(rx_array[0] == '-' && rx_array[1] == ' ')
    {
        polarity = NEGATIVE;
    }
    
    else //Invalid input.
    {
        return ERROR;
    }
    
    //Get calibration value.    
    if(rx_array[3] == 0x0D) //1 digit.
    {
        //Do conversion and check for success.
        if(Convert_1_Num(&rx_array[2], &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Range check value.
        if(value > 99)
        {
            return ERROR;
        }
        
        //Save value.
        EEPROM_Write_Word((uint16_t *)CH_3_CAL, value);
        EEPROM_Write_Word((uint16_t *)CH_3_POLARITY, polarity);
        tchannel3.cal_value = EEPROM_Read_Word((uint16_t *)CH_3_CAL);
        tchannel3.cal_polarity = EEPROM_Read_Word((uint16_t *)CH_3_POLARITY);
        
        return NO_ERROR;    
    }
    
    //Get calibration value.    
    if(rx_array[4] == 0x0D) //1 digit.
    {
        //Do conversion and check for success.
        if(Convert_2_Num(&rx_array[2], &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Range check value.
        if(value > 99)
        {
            return ERROR;
        }
        
        //Save value.
        EEPROM_Write_Word((uint16_t *)CH_3_CAL, value);
        EEPROM_Write_Word((uint16_t *)CH_3_POLARITY, polarity);
        tchannel3.cal_value = EEPROM_Read_Word((uint16_t *)CH_3_CAL);
        tchannel3.cal_polarity = EEPROM_Read_Word((uint16_t *)CH_3_POLARITY);
        
        return NO_ERROR;    
    }
            
    return ERROR;
}

//Menu selection 8.
uint8_t Selection8(uint8_t *rx_array)
{
    uint16_t polarity;
    uint16_t value;
    
    //Check polarity
    if(rx_array[0] == '+' && rx_array[1] == ' ')
    {
        polarity = POSITIVE;
    }
    
    else if(rx_array[0] == '-' && rx_array[1] == ' ')
    {
        polarity = NEGATIVE;
    }
    
    else //Invalid input.
    {
        return ERROR;
    }
    
    //Get calibration value.    
    if(rx_array[3] == 0x0D) //1 digit.
    {
        //Do conversion and check for success.
        if(Convert_1_Num(&rx_array[2], &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Range check value.
        if(value > 99)
        {
            return ERROR;
        }
        
        //Save value.
        EEPROM_Write_Word((uint16_t *)CH_4_CAL, value);
        EEPROM_Write_Word((uint16_t *)CH_4_POLARITY, polarity);
        tchannel4.cal_value = EEPROM_Read_Word((uint16_t *)CH_4_CAL);
        tchannel4.cal_polarity = EEPROM_Read_Word((uint16_t *)CH_4_POLARITY);
        
        return NO_ERROR;    
    }
    
    //Get calibration value.    
    if(rx_array[4] == 0x0D) //1 digit.
    {
        //Do conversion and check for success.
        if(Convert_2_Num(&rx_array[2], &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Range check value.
        if(value > 99)
        {
            return ERROR;
        }
        
        //Save value.
        EEPROM_Write_Word((uint16_t *)CH_4_CAL, value);
        EEPROM_Write_Word((uint16_t *)CH_4_POLARITY, polarity);
        tchannel4.cal_value = EEPROM_Read_Word((uint16_t *)CH_4_CAL);
        tchannel4.cal_polarity = EEPROM_Read_Word((uint16_t *)CH_4_POLARITY);
        
        return NO_ERROR;    
    }
            
    return ERROR;
}

//Menu selection 9.
uint8_t Selection9(uint8_t *rx_array)
{
    uint16_t value;
    
    if(rx_array[1] == 0x0D) //1 hex number.
    {
        //Do conversion and check for success.
        if(Convert_1_Hex(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        if(value > 0x0F)    //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)CH_1_TRP_MSK, value);
        tchannel1.trip_mask = EEPROM_Read_Word((uint16_t *)CH_1_TRP_MSK);
        
        return NO_ERROR;
    }
    
    if(rx_array[2] == 0x0D) //2 hex numbers.
    {
        //Do conversion and check for success.
        if(Convert_2_Hex(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        if(value > 0x0F)    //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)CH_1_TRP_MSK, value);
        tchannel1.trip_mask = EEPROM_Read_Word((uint16_t *)CH_1_TRP_MSK);
        
        return NO_ERROR;
    }
    
    return ERROR;
}

//Menu selection 10.
uint8_t Selection10(uint8_t *rx_array)
{
    uint16_t value;
    
    if(rx_array[1] == 0x0D) //1 hex number.
    {
        //Do conversion and check for success.
        if(Convert_1_Hex(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        if(value > 0x0F)    //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)CH_2_TRP_MSK, value);
        tchannel2.trip_mask = EEPROM_Read_Word((uint16_t *)CH_2_TRP_MSK);
        
        return NO_ERROR;
    }
    
    if(rx_array[2] == 0x0D) //2 hex numbers.
    {
        //Do conversion and check for success.
        if(Convert_2_Hex(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        if(value > 0x0F)    //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)CH_2_TRP_MSK, value);
        tchannel2.trip_mask = EEPROM_Read_Word((uint16_t *)CH_2_TRP_MSK);
        
        return NO_ERROR;
    }
    
    return ERROR;
}

//Menu selection 11.
uint8_t Selection11(uint8_t *rx_array)
{
    uint16_t value;
    
    if(rx_array[1] == 0x0D) //1 hex number.
    {
        //Do conversion and check for success.
        if(Convert_1_Hex(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        if(value > 0x0F)    //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)CH_3_TRP_MSK, value);
        tchannel3.trip_mask = EEPROM_Read_Word((uint16_t *)CH_3_TRP_MSK);
        
        return NO_ERROR;
    }
    
    if(rx_array[2] == 0x0D) //2 hex numbers.
    {
        //Do conversion and check for success.
        if(Convert_2_Hex(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        if(value > 0x0F)    //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)CH_3_TRP_MSK, value);
        tchannel3.trip_mask = EEPROM_Read_Word((uint16_t *)CH_3_TRP_MSK);
        
        return NO_ERROR;
    }
    
    return ERROR;
}

//Menu selection 12.
uint8_t Selection12(uint8_t *rx_array)
{
    uint16_t value;
    
    if(rx_array[1] == 0x0D) //1 hex number.
    {
        //Do conversion and check for success.
        if(Convert_1_Hex(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        if(value > 0x0F)    //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)CH_4_TRP_MSK, value);
        tchannel4.trip_mask = EEPROM_Read_Word((uint16_t *)CH_4_TRP_MSK);
        
        return NO_ERROR;
    }
    
    if(rx_array[2] == 0x0D) //2 hex numbers.
    {
        //Do conversion and check for success.
        if(Convert_2_Hex(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        if(value > 0x0F)    //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)CH_4_TRP_MSK, value);
        tchannel4.trip_mask = EEPROM_Read_Word((uint16_t *)CH_4_TRP_MSK);
        
        return NO_ERROR;
    }
    
    return ERROR;
}

//Menu selection 13.
uint8_t Selection13(uint8_t *rx_array)
{
    if(rx_array[0] == 'C' && rx_array[1] == 0x0D)   //Set units of measure to Celsius.
    {
        EEPROM_Write_Word((uint16_t *)C_OR_F, CELSIUS);
        system_struct.temp_c_or_f = EEPROM_Read_Word((uint16_t *)C_OR_F);
        return NO_ERROR;    
    }
    
    if(rx_array[0] == 'F' && rx_array[1] == 0x0D)
    {
        EEPROM_Write_Word((uint16_t *)C_OR_F, FAHRENHEIT);  //Set units of measure to Fahrenheit.
        system_struct.temp_c_or_f = EEPROM_Read_Word((uint16_t *)C_OR_F);
        return NO_ERROR;    
    }
    
    return ERROR;
}

//Menu selection 14.
uint8_t Selection14(uint8_t *rx_array)
{
    uint16_t value;
    
    if(rx_array[1] == 0x0D) //1 digit.
    {
        //Do conversion and check for success.
        if(Convert_1_Num(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Convert to ADC value.
        if(system_struct.temp_c_or_f == CELSIUS)
        {
            value = ADC_From_Celsius((float)value);
        }
        
        else
        {
            value = ADC_From_Farenheit((float)value);
        }
        
        if(value < ABSOLUTE_MIN || value > ABSOLUTE_MAX) //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)T_SP_LOW, value);
        system_struct.temp_low = EEPROM_Read_Word((uint16_t *)T_SP_LOW);
        
        return NO_ERROR;
    }
    
    if(rx_array[2] == 0x0D) //2 digits.
    {
        //Do conversion and check for success.
        if(Convert_2_Num(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Convert to ADC value.
        if(system_struct.temp_c_or_f == CELSIUS)
        {
            value = ADC_From_Celsius((float)value);
        }
        
        else
        {
            value = ADC_From_Farenheit((float)value);
        }
        
        if(value < ABSOLUTE_MIN || value > ABSOLUTE_MAX) //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)T_SP_LOW, value);
        system_struct.temp_low = EEPROM_Read_Word((uint16_t *)T_SP_LOW);
        
        return NO_ERROR;
    }
    
    if(rx_array[3] == 0x0D) //3 digits.
    {
        //Do conversion and check for success.
        if(Convert_3_Num(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Convert to ADC value.
        if(system_struct.temp_c_or_f == CELSIUS)
        {
            value = ADC_From_Celsius((float)value);
        }
        
        else
        {
            value = ADC_From_Farenheit((float)value);
        }
        
        if(value < ABSOLUTE_MIN || value > ABSOLUTE_MAX) //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)T_SP_LOW, value);
        system_struct.temp_low = EEPROM_Read_Word((uint16_t *)T_SP_LOW);
        
        return NO_ERROR;
    }
    
    return ERROR;   
}

//Menu selection 15.
uint8_t Selection15(uint8_t *rx_array)
{
    uint16_t value;
    
    if(rx_array[1] == 0x0D) //1 digit.
    {
        //Do conversion and check for success.
        if(Convert_1_Num(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Convert to ADC value.
        if(system_struct.temp_c_or_f == CELSIUS)
        {
            value = ADC_From_Celsius((float)value);
        }
        
        else
        {
            value = ADC_From_Farenheit((float)value);
        }
        
        if(value < ABSOLUTE_MIN || value > ABSOLUTE_MAX) //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)T_SP_COLD, value);
        system_struct.temp_cold = EEPROM_Read_Word((uint16_t *)T_SP_COLD);
        
        return NO_ERROR;
    }
    
    if(rx_array[2] == 0x0D) //2 digits.
    {
        //Do conversion and check for success.
        if(Convert_2_Num(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Convert to ADC value.
        if(system_struct.temp_c_or_f == CELSIUS)
        {
            value = ADC_From_Celsius((float)value);
        }
        
        else
        {
            value = ADC_From_Farenheit((float)value);
        }
        
        if(value < ABSOLUTE_MIN || value > ABSOLUTE_MAX) //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)T_SP_COLD, value);
        system_struct.temp_cold = EEPROM_Read_Word((uint16_t *)T_SP_COLD);
        
        return NO_ERROR;
    }
    
    if(rx_array[3] == 0x0D) //3 digits.
    {
        //Do conversion and check for success.
        if(Convert_3_Num(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Convert to ADC value.
        if(system_struct.temp_c_or_f == CELSIUS)
        {
            value = ADC_From_Celsius((float)value);
        }
        
        else
        {
            value = ADC_From_Farenheit((float)value);
        }
        
        if(value < ABSOLUTE_MIN || value > ABSOLUTE_MAX) //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)T_SP_COLD, value);
        system_struct.temp_cold = EEPROM_Read_Word((uint16_t *)T_SP_COLD);
        
        return NO_ERROR;
    }
    
    return ERROR;
}

//Menu selection 16.
uint8_t Selection16(uint8_t *rx_array)
{
    uint16_t value;
    
    if(rx_array[1] == 0x0D) //1 digit.
    {
        //Do conversion and check for success.
        if(Convert_1_Num(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Convert to ADC value.
        if(system_struct.temp_c_or_f == CELSIUS)
        {
            value = ADC_From_Celsius((float)value);
        }
        
        else
        {
            value = ADC_From_Farenheit((float)value);
        }
        
        if(value < ABSOLUTE_MIN || value > ABSOLUTE_MAX) //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)T_SP_WARM, value);
        system_struct.temp_min = EEPROM_Read_Word((uint16_t *)T_SP_WARM);
        
        return NO_ERROR;
    }
    
    if(rx_array[2] == 0x0D) //2 digits.
    {
        //Do conversion and check for success.
        if(Convert_2_Num(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Convert to ADC value.
        if(system_struct.temp_c_or_f == CELSIUS)
        {
            value = ADC_From_Celsius((float)value);
        }
        
        else
        {
            value = ADC_From_Farenheit((float)value);
        }
        
        if(value < ABSOLUTE_MIN || value > ABSOLUTE_MAX) //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)T_SP_WARM, value);
        system_struct.temp_min = EEPROM_Read_Word((uint16_t *)T_SP_WARM);
        
        return NO_ERROR;
    }
    
    if(rx_array[3] == 0x0D) //3 digits.
    {
        //Do conversion and check for success.
        if(Convert_3_Num(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Convert to ADC value.
        if(system_struct.temp_c_or_f == CELSIUS)
        {
            value = ADC_From_Celsius((float)value);
        }
        
        else
        {
            value = ADC_From_Farenheit((float)value);
        }
        
        if(value < ABSOLUTE_MIN || value > ABSOLUTE_MAX) //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)T_SP_WARM, value);
        system_struct.temp_min = EEPROM_Read_Word((uint16_t *)T_SP_WARM);
        
        return NO_ERROR;
    }
    
    return ERROR;
}

//Menu selection 17.
uint8_t Selection17(uint8_t *rx_array)
{
    uint16_t value;
    
    if(rx_array[1] == 0x0D) //1 digit.
    {
        //Do conversion and check for success.
        if(Convert_1_Num(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Convert to ADC value.
        if(system_struct.temp_c_or_f == CELSIUS)
        {
            value = ADC_From_Celsius((float)value);
        }
        
        else
        {
            value = ADC_From_Farenheit((float)value);
        }
        
        if(value < ABSOLUTE_MIN || value > ABSOLUTE_MAX) //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)T_SP_HOT, value);
        system_struct.temp_maj = EEPROM_Read_Word((uint16_t *)T_SP_HOT);
        
        return NO_ERROR;
    }
    
    if(rx_array[2] == 0x0D) //2 digits.
    {
        //Do conversion and check for success.
        if(Convert_2_Num(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Convert to ADC value.
        if(system_struct.temp_c_or_f == CELSIUS)
        {
            value = ADC_From_Celsius((float)value);
        }
        
        else
        {
            value = ADC_From_Farenheit((float)value);
        }
        
        if(value < ABSOLUTE_MIN || value > ABSOLUTE_MAX) //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)T_SP_HOT, value);
        system_struct.temp_maj = EEPROM_Read_Word((uint16_t *)T_SP_HOT);
        
        return NO_ERROR;
    }
    
    if(rx_array[3] == 0x0D) //3 digits.
    {
        //Do conversion and check for success.
        if(Convert_3_Num(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Convert to ADC value.
        if(system_struct.temp_c_or_f == CELSIUS)
        {
            value = ADC_From_Celsius((float)value);
        }
        
        else
        {
            value = ADC_From_Farenheit((float)value);
        }
        
        if(value < ABSOLUTE_MIN || value > ABSOLUTE_MAX) //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)T_SP_HOT, value);
        system_struct.temp_maj = EEPROM_Read_Word((uint16_t *)T_SP_HOT);
        
        return NO_ERROR;
    }
    
    return ERROR;
}

//Menu selection 18.
uint8_t Selection18(uint8_t *rx_array)
{
    uint16_t value;
    
    if(rx_array[1] == 0x0D) //1 digit.
    {
        //Do conversion and check for success.
        if(Convert_1_Num(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Convert to ADC value.
        if(system_struct.temp_c_or_f == CELSIUS)
        {
            value = ADC_From_Celsius((float)value);
        }
        
        else
        {
            value = ADC_From_Farenheit((float)value);
        }
        
        if(value < ABSOLUTE_MIN || value > ABSOLUTE_MAX) //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)T_SP_TRIP, value);
        system_struct.temp_trip = EEPROM_Read_Word((uint16_t *)T_SP_TRIP);
        
        return NO_ERROR;
    }
    
    if(rx_array[2] == 0x0D) //2 digits.
    {
        //Do conversion and check for success.
        if(Convert_2_Num(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Convert to ADC value.
        if(system_struct.temp_c_or_f == CELSIUS)
        {
            value = ADC_From_Celsius((float)value);
        }
        
        else
        {
            value = ADC_From_Farenheit((float)value);
        }
        
        if(value < ABSOLUTE_MIN || value > ABSOLUTE_MAX) //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)T_SP_TRIP, value);
        system_struct.temp_trip = EEPROM_Read_Word((uint16_t *)T_SP_TRIP);
        
        return NO_ERROR;
    }
    
    if(rx_array[3] == 0x0D) //3 digits.
    {
        //Do conversion and check for success.
        if(Convert_3_Num(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Convert to ADC value.
        if(system_struct.temp_c_or_f == CELSIUS)
        {
            value = ADC_From_Celsius((float)value);
        }
        
        else
        {
            value = ADC_From_Farenheit((float)value);
        }
        
        if(value < ABSOLUTE_MIN || value > ABSOLUTE_MAX) //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)T_SP_TRIP, value);
        system_struct.temp_trip = EEPROM_Read_Word((uint16_t *)T_SP_TRIP);
        
        return NO_ERROR;
    }
    
    return ERROR;
}

//Menu selection 19.
uint8_t Selection19(uint8_t *rx_array)
{
    uint16_t value;
    
    if(rx_array[1] == 0x0D) //1 digit.
    {
        //Do conversion and check for success.
        if(Convert_1_Num(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Convert to ADC value.
        if(system_struct.temp_c_or_f == CELSIUS)
        {
            value = ADC_From_Celsius((float)value);
        }
        
        else
        {
            value = ADC_From_Farenheit((float)value);
        }
        
        if(value < ABSOLUTE_MIN || value > ABSOLUTE_MAX) //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)T_SP_HIGH, value);
        system_struct.temp_high = EEPROM_Read_Word((uint16_t *)T_SP_HIGH);
        
        return NO_ERROR;
    }
    
    if(rx_array[2] == 0x0D) //2 digits.
    {
        //Do conversion and check for success.
        if(Convert_2_Num(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Convert to ADC value.
        if(system_struct.temp_c_or_f == CELSIUS)
        {
            value = ADC_From_Celsius((float)value);
        }
        
        else
        {
            value = ADC_From_Farenheit((float)value);
        }
        
        if(value < ABSOLUTE_MIN || value > ABSOLUTE_MAX) //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)T_SP_HIGH, value);
        system_struct.temp_high = EEPROM_Read_Word((uint16_t *)T_SP_HIGH);
        
        return NO_ERROR;
    }
    
    if(rx_array[3] == 0x0D) //3 digits.
    {
        //Do conversion and check for success.
        if(Convert_3_Num(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Convert to ADC value.
        if(system_struct.temp_c_or_f == CELSIUS)
        {
            value = ADC_From_Celsius((float)value);
        }
        
        else
        {
            value = ADC_From_Farenheit((float)value);
        }
        
        if(value < ABSOLUTE_MIN || value > ABSOLUTE_MAX) //Conversion out of range.
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)T_SP_HIGH, value);
        system_struct.temp_high = EEPROM_Read_Word((uint16_t *)T_SP_HIGH);
        
        return NO_ERROR;
    }
    
    return ERROR;
}

//Menu selection 20.
uint8_t Selection20(uint8_t *rx_array)
{
    uint16_t value;
    
    if(rx_array[1] == 0x0D) //1 digit.
    {
        //Do conversion and check for success.
        if(Convert_1_Num(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Check to make sure value is greater than 0.
        if(!value)
        {
            return ERROR;
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)T_SP_ERROR, value);
        system_struct.temp_err = EEPROM_Read_Word((uint16_t *)T_SP_ERROR);
        
        return NO_ERROR;
    }
    
    if(rx_array[2] == 0x0D) //2 digits.
    {
        //Do conversion and check for success.
        if(Convert_2_Num(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)T_SP_ERROR, value);
        system_struct.temp_err = EEPROM_Read_Word((uint16_t *)T_SP_ERROR);
        
        return NO_ERROR;
    }
    
    if(rx_array[3] == 0x0D) //3 digits.
    {
        //Do conversion and check for success.
        if(Convert_3_Num(rx_array, &value))
        {
            return ERROR;   //Conversion failed.
        }
        
        //Conversion successful.
        EEPROM_Write_Word((uint16_t *)T_SP_ERROR, value);
        system_struct.temp_err = EEPROM_Read_Word((uint16_t *)T_SP_ERROR);
        
        return NO_ERROR;
    }
    
    return ERROR;
}

//Menu selection 21.
uint8_t Selection21()
{
    EEPROM_Reset_to_Default(); //Set all set points to their default values.
    EEPROM_Read_All(); //Load all set points from the EEPROM.
    return NO_ERROR;
}

//Takes one hex value from an array and converts to a numeric value.
uint8_t Convert_1_Hex(uint8_t *p, uint16_t *value)
{
    if(Check_Hex_Range(p[0])) //Check to make sure character is a valid hex character.
    {
        return ERROR;
    }
    
    if((*p) < 'A')
    {
        (*value) = (p[0] - 0x30);
    }
    
    else
    {
        (*value) = (p[0] - 0x37);
    }
        
    return NO_ERROR;
}

//Takes two hex values from an array and converts to a numeric value.
uint8_t Convert_2_Hex(uint8_t *p, uint16_t *value)
{
    uint16_t working_value;
    
    if(Convert_1_Hex(p, &working_value))
    {
        return ERROR;
    }
    
    working_value *= 16; //Upper hex. Multiply by 16.
    
    if(Convert_1_Hex(&p[1], value))
    {
        return ERROR;
    }
    
    *value = (*value) + working_value;
    
    return NO_ERROR;
}

//Takes one decimal value from an array and converts to a numeric value.
uint8_t Convert_1_Num(uint8_t *p, uint16_t *value)
{
    if(Check_Int_Range(p[0])) //Check to make sure character is a valid integer character.
    {
        return ERROR;
    }
    
    (*value) = (p[0] - 0x30);
        
    return NO_ERROR;
}

//Takes two decimal values from an array and converts to a numeric value.
uint8_t Convert_2_Num(uint8_t *p, uint16_t *value)
{
    uint16_t working_value;
    
    if(Convert_1_Num(p, &working_value))
    {
        return ERROR;
    }
    
    working_value *= 10; //Upper digit. Multiply by 10.
    
    if(Convert_1_Num(&p[1], value))
    {
        return ERROR;
    }
    
    *value = (*value) + working_value;
    
    return NO_ERROR;
}

//Takes three decimal values from an array and converts to a numeric value.
uint8_t Convert_3_Num(uint8_t *p, uint16_t *value)
{
    uint16_t working_value1, working_value2;
    
    if(Convert_1_Num(p, &working_value1))
    {
        return ERROR;
    }
    
    working_value1 *= 100; //Upper digit. Multiply by 100.
    
    if(Convert_1_Num(&p[1], &working_value2))
    {
        return ERROR;
    }
    
    working_value2 *= 10; //Middle digit. Multiply by 10.
    
    if(Convert_1_Num(&p[2], value))
    {
        return ERROR;
    }
    
    *value = (*value) + working_value1 + working_value2;
    
    return NO_ERROR;
}

//Check if character is a valid hex value.
uint8_t Check_Hex_Range(const uint8_t value)
{
    if(value < '0')
    {
        return ERROR;
    }
    
    if(value > '9' && value < 'A')
    {
        return ERROR;
    }
    
    if(value > 'F')
    {
        return ERROR;
    }
    
    return NO_ERROR;
}

//Check if character is a valid decimal value.
uint8_t Check_Int_Range(const uint8_t value)
{
    if(value < '0')
    {
        return ERROR;
    }
    
    if(value > '9')
    {
        return ERROR;
    }
    
    return NO_ERROR;
}
