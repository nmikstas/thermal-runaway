/*
 * EEPROM_Control.c
 *
 * Created: 12/31/2013 3:10:50 PM
 *  Author: Nick Mikstas
 */

#include "EEPROM_Control.h"

//This function writes a 16-bit value to the EEPROM.
void EEPROM_Write_Word(uint16_t *address, uint16_t data)
{   
    cli(); //Disable interrupts.
    
    eeprom_write_word(address, data);
    
    sei(); //Enable interrupts.
    
    eeprom_write_word((uint16_t *)EEPROM_PARK, 0);  //Park the pointer.
}

//This function reads a 16-bit value from the EEPROM.
uint16_t EEPROM_Read_Word(uint16_t *address)
{   
    uint16_t read_data;
    
    cli(); //Disable interrupts.
    
    read_data = eeprom_read_word(address);
    
    sei(); //Enable interrupts.
    
    eeprom_read_word((uint16_t *)EEPROM_PARK);  //Park the pointer.
    
    return read_data;
}

//This function sets all the set points back to factory default.
void EEPROM_Reset_to_Default()
{
    EEPROM_Write_Word((uint16_t *)CH_1_ENABLED, DEFAULT_CH_1_EN);
    EEPROM_Write_Word((uint16_t *)CH_2_ENABLED, DEFAULT_CH_2_EN);
    EEPROM_Write_Word((uint16_t *)CH_3_ENABLED, DEFAULT_CH_3_EN);
    EEPROM_Write_Word((uint16_t *)CH_4_ENABLED, DEFAULT_CH_4_EN);
    EEPROM_Write_Word((uint16_t *)CH_1_TRP_MSK, DEFAULT_CH_1_TM);
    EEPROM_Write_Word((uint16_t *)CH_2_TRP_MSK, DEFAULT_CH_2_TM);
    EEPROM_Write_Word((uint16_t *)CH_3_TRP_MSK, DEFAULT_CH_3_TM);
    EEPROM_Write_Word((uint16_t *)CH_4_TRP_MSK, DEFAULT_CH_4_TM);
    EEPROM_Write_Word((uint16_t *)C_OR_F, DEFAULT_C_OR_F);
    EEPROM_Write_Word((uint16_t *)T_SP_LOW, DEFAULT_T_SP_LOW);
    EEPROM_Write_Word((uint16_t *)T_SP_COLD, DEFAULT_T_SP_COLD);
    EEPROM_Write_Word((uint16_t *)T_SP_WARM, DEFAULT_T_SP_WARM);
    EEPROM_Write_Word((uint16_t *)T_SP_HOT, DEFAULT_T_SP_HOT);
    EEPROM_Write_Word((uint16_t *)T_SP_TRIP, DEFAULT_T_SP_TRIP);
    EEPROM_Write_Word((uint16_t *)T_SP_HIGH, DEFAULT_T_SP_HIGH);
    EEPROM_Write_Word((uint16_t *)T_SP_ERROR, DEFAULT_T_SP_ERROR);
    EEPROM_Write_Word((uint16_t *)CH_1_CAL, DEFAULT_CAL);
    EEPROM_Write_Word((uint16_t *)CH_1_POLARITY, DEFAULT_POLARITY);
    EEPROM_Write_Word((uint16_t *)CH_2_CAL, DEFAULT_CAL);
    EEPROM_Write_Word((uint16_t *)CH_2_POLARITY, DEFAULT_POLARITY);
    EEPROM_Write_Word((uint16_t *)CH_3_CAL, DEFAULT_CAL);
    EEPROM_Write_Word((uint16_t *)CH_3_POLARITY, DEFAULT_POLARITY);
    EEPROM_Write_Word((uint16_t *)CH_4_CAL, DEFAULT_CAL);
    EEPROM_Write_Word((uint16_t *)CH_4_POLARITY, DEFAULT_POLARITY);
}

//This function reads all set points from the EEPROM.
void EEPROM_Read_All()
{
    //Set initial values for each temperature channel. This will be
    //replaced with an EEPROM read function. Also set system set points.
    tchannel1.tenabled = EEPROM_Read_Word((uint16_t *)CH_1_ENABLED);
    tchannel2.tenabled = EEPROM_Read_Word((uint16_t *)CH_2_ENABLED);
    tchannel3.tenabled = EEPROM_Read_Word((uint16_t *)CH_3_ENABLED);
    tchannel4.tenabled = EEPROM_Read_Word((uint16_t *)CH_4_ENABLED);
    tchannel1.trip_mask = EEPROM_Read_Word((uint16_t *)CH_1_TRP_MSK);
    tchannel2.trip_mask = EEPROM_Read_Word((uint16_t *)CH_2_TRP_MSK);
    tchannel3.trip_mask = EEPROM_Read_Word((uint16_t *)CH_3_TRP_MSK);
    tchannel4.trip_mask = EEPROM_Read_Word((uint16_t *)CH_4_TRP_MSK);
    system_struct.temp_c_or_f = EEPROM_Read_Word((uint16_t *)C_OR_F);
    system_struct.temp_low = EEPROM_Read_Word((uint16_t *)T_SP_LOW);
    system_struct.temp_cold = EEPROM_Read_Word((uint16_t *)T_SP_COLD);
    system_struct.temp_min = EEPROM_Read_Word((uint16_t *)T_SP_WARM);
    system_struct.temp_maj = EEPROM_Read_Word((uint16_t *)T_SP_HOT);
    system_struct.temp_trip = EEPROM_Read_Word((uint16_t *)T_SP_TRIP);
    system_struct.temp_high = EEPROM_Read_Word((uint16_t *)T_SP_HIGH);
    system_struct.temp_err = EEPROM_Read_Word((uint16_t *)T_SP_ERROR);
    tchannel1.cal_value = EEPROM_Read_Word((uint16_t *)CH_1_CAL);
    tchannel2.cal_value = EEPROM_Read_Word((uint16_t *)CH_2_CAL);
    tchannel3.cal_value = EEPROM_Read_Word((uint16_t *)CH_3_CAL);
    tchannel4.cal_value = EEPROM_Read_Word((uint16_t *)CH_4_CAL);
    tchannel1.cal_polarity = EEPROM_Read_Word((uint16_t *)CH_1_POLARITY);
    tchannel2.cal_polarity = EEPROM_Read_Word((uint16_t *)CH_2_POLARITY);
    tchannel3.cal_polarity = EEPROM_Read_Word((uint16_t *)CH_3_POLARITY);
    tchannel4.cal_polarity = EEPROM_Read_Word((uint16_t *)CH_4_POLARITY);
}
