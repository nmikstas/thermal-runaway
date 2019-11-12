/*
 * LCD_Control.c
 *
 * Created: 12/18/2013 10:06:19 PM
 *  Author: Nick Mikstas
 */

#include "LCD_Control.h"

//The following function writes command byte d to the LCD.
void write_command(uint8_t d)
{
    uint8_t serialcounter;  
    
    prtRS &= ~(1 << RS); //A0 = Command.
    
    for(serialcounter = 1; serialcounter <= 8; serialcounter++) //Send 8 bits.
    {   
        if((d & 0x80) == 0x80) //get only the MSB.
        {
            prtSI |=  (1 << SI); //if 1, then SI=1.
        }        
        
        else
        {
            prtSI &= ~(1 << SI); //if 0, then SI=0.     
        }        
        
        d = (d << 1); //shift data byte left.
        prtSCL &= ~(1 << SCL);      
        prtSCL |=  (1 << SCL);      
        prtSCL &= ~(1 << SCL); //SCL
    }
    
    prtRS &= ~(1 << RS); //Turn off LED (Cerebot Nano).
    prtSI &= ~(1 << SI); //Turn off LED (Cerebot Nano).
}

//The following function writes byte d to LCD to be displayed.
void write_byte(uint8_t d)
{
    uint8_t serialcounter;
    
    prtRS |=  (1 << RS); //A0 = Data.
    
    for(serialcounter = 1; serialcounter <= 8; serialcounter++) //Send 8 bits.
    {       
        if((d & 0x80) == 0x80) //get only the MSB.
        {
            prtSI |=  (1 << SI); //if 1, then SI=1.
        }

        else
        {
            prtSI &= ~(1 << SI); //if 0, then SI=0.     
        }

        d = (d << 1); //shift data byte left.
        prtSCL &= ~(1 << SCL);      
        prtSCL |=  (1 << SCL);      
        prtSCL &= ~(1 << SCL); //SCL.
    }
    
    prtRS &= ~(1 << RS); //Turn off LED (Cerebot Nano).
    prtSI &= ~(1 << SI); //Turn off LED (Cerebot Nano).
}

//The following function writes a string of bytes to the LCD.
void write_string(char *p)
{
    while(*p)
    {
        if(*p == 0x12) //Clear display. ctrl-r
        {
            init_lcd();
            p++;
        }
                
        else if(*p == 0x06) //Return to beginning of first line. ctrl-f
        {
            write_command(0x02);
            write_string("                           "); //Erase line
            write_command(0x02); //Return to beginning of first line    
            p++;            
        }
                
        else if(*p == 0x13) //Return to beginning of second line. ctrl-s
        {
            write_command(0xC0);
            write_string("                           "); //Erase line
            write_command(0x02);
            write_command(0xC0); //Return to beginning of second line.
            p++;
        }
        
        else if(*p < 0x10) //Ignore non-printable characters.
        {
            p++;
        }
                
        else //Display printable characters.
        {
            write_byte(*p);
            p++;
        }               
    }
}

//This function prints the hex value of a number
//on the LCD display. It is used for debugging.
void print_hex(unsigned char b)
{
    unsigned char temp;
    
    temp   = b;
    temp >>= 4;
    
    if(temp > 0x09)
        write_byte(temp + 0x37);
    else
        write_byte(temp + 0x30);
    
    temp  = b;
    temp &= 0x0F;
    
    if(temp > 0x09)
        write_byte(temp + 0x37);
    else
        write_byte(temp + 0x30);    
}

//initialize the LCD.
void init_lcd()
{   
    prtRST &= ~(1 << RST); //RESET.
    Wait_ms(2);
    prtRST |= (1 << RST); //end reset.
    Wait_ms(20);
    write_command(0x30); //wake up.
    Wait_ms(2);
    write_command(0x30); //wake up.
    write_command(0x30); //wake up.
    write_command(0x39); //function set.
    write_command(0x14); //internal osc frequency.
    write_command(0x56); //power control.
    write_command(0x6D); //follower control.
    write_command(0x70); //contrast.
    write_command(0x0C); //display on.
    write_command(0x06); //entry mode.
    write_command(0x01); //clear.
    Wait_ms(10);
}
