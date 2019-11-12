/*
 * Helper.c
 *
 * Created: 12/24/2013 8:41:18 PM
 *  Author: Nick Mikstas
 */

#include "Helper.h"

//This function waits for delay milliseconds before
//exiting.  number of counts of i can be changed
//to change the timing on the delay. Configured for
//8MHz processor.
void Wait_ms(uint16_t delay)
{
    uint16_t i;

    while(delay)
    {
        for( i = 0; i < 418; i++)
        {
            ;
        }
        delay -= 1;     
    }
}

//Initialize the micro-controller.
void init_micro()
{
    uint16_t ubrr = UBRR;
    
    //Turn off unused hardware.
    PRR =  (1 << PRTIM2) | (1 << PRTIM0) | (1 << PRSPI);
    ACSR = (1  << ACD); //Turn off analog comparator.
    
    //Disable digital input pins.
    DIDR0 = 0x0F;
    
    ADCSRA |= (1 << ADPS2); //Div by 64. 125KHz ADC clock.
    ADCSRA |= (1 << ADPS1);
    
    //Set pins as outputs.
    DDRD |= (1 << RS) | (1 << SI) | (1 << RLY1) | (1 << RLY2) | (1 << RLY3) | (1 << RLY4);
    DDRB |= (1 << RST)| (1 << SCL)| (1 << GRN)  | (1 << RED)  | (1 << BZR);
            
    //Clear bits on LCD pins.
    prtRS  &= ~(1 << RS);
    prtRST &= ~(1 << RST);
    prtSCL &= ~(1 << SCL);
    prtSI  &= ~(1 << SI);
    
    UBRR0H = (uint8_t)(ubrr >> 8);                          //Load upper ubrr byte.
    UBRR0L = (uint8_t)ubrr;                                 //Load lower ubrr byte
    UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);   //Enable RX interrupt, receive and transmit circuits.
    UCSR0C = (3 << UCSZ00);                                 //Use 8 bit bytes.  
    
    sei(); //Enable global interrupts.  
}

//Turn on audible alarm.
void Buzzer_On()
{
    TCCR1A = 0;                         //Disable PWM.
    
    TCCR1A |= (1 << WGM11);             //Setup fast PWM mode.
    TCCR1B |= (1<<WGM13) | (1<<WGM12);  //Setup fast PWM mode.
    TCCR1B |= (1<<CS11);                //divide by 8 prescalar for 8MHz clock.
    

    TCCR1A |= (2 << 4);
    
    ICR1 = 249;                         //Frequency is 4KHz.
    OCR1B = ICR1 / 2;                   //50% duty cycle.
}

//Turn off audible alarm.
void Buzzer_Off()
{
    TCCR1A = 0;                         //Disable PWM.
}

//ADC = C / 100 * 1024 / 1.1
//Returns an equivalent ADC value from a celsius value.
uint16_t ADC_From_Celsius(const float celsius)
{
    return (uint16_t)(celsius / 100 * 1024 / 1.1f);
}

//ADC = (F - 32) / 1.8 / 100 * 1024 / 1.1
//Returns an equivalent ADC value from a Fahrenheit value.
uint16_t ADC_From_Farenheit(const float farenheit)
{
    return (uint16_t)((farenheit - 32) / 1.8f / 100 * 1024 / 1.1f);
}

//C = ADC * 100 / 1024 * 1.1
//Returns an equivalent celsius value from an ADC value.
float Celsius_From_ADC(const uint16_t adc)
{
    return adc * 100.0f / 1024.0f * 1.1f;
}

//F = ADC * 100 / 1024 * 1.1 * 1.8 + 32
//Returns a farenheit value from an ADC value.
float Farenheit_From_ADC(const uint16_t adc)
{
    return adc * 100.0f / 1024.0f * 1.1f * 1.8f + 32.0f;
}

//F = C * 1.8 + 32
//Returns a farenheit value from a celsius value.
float Farenheit_From_Celsius(const float celsius)
{
    return celsius * 1.8f + 32.0f;
}

//C = (F - 32) / 1.8
//Returns a celsius value from a farenheit value.
float Celsius_From_Farenheit(const float farenheit)
{
    return (farenheit - 32) / 1.8f;
}

//Converts a positive float value to an unsigned 16 bit integer with rounding.
//return values:
//0 = no errors.
//1 = negative float value provided.
//2 = float value above 65535.
uint8_t UInt16_From_Float(const float f, uint16_t *i)
{
    if(f < 0)
    {
        return 1;
    }
    
    if(f > 65535)
    {
        return 2;
    }
    
    //Round up.
    if(f - (uint16_t)f >= .5)
    {
        *i = (uint16_t)(f + 1);
        return 0;
    }
    
    //Round down
    *i = (uint16_t)f;
    return 0;
}

//Converts an unsigned integer into a null terminated string.
//return values:
//0 = no errors.
uint8_t String_From_UInt(uint16_t i, char *s)
{
    uint8_t number_started = FALSE; //Indicate conversion has started.
    
    if(i / 10000) //Get upper digit if present.
    {
        number_started = TRUE;
        *s = i / 10000 + 0x30;  
        i %= 10000;
        s++;
    }       
    
    if(i / 1000 || number_started) //Get next digit if present.
    {
        number_started = TRUE;
        *s = i / 1000 + 0x30;
        i %= 1000;
        s++;
    }
    
    if(i / 100 || number_started) //Get next digit if present.
    {
        number_started = TRUE;
        *s = i / 100 + 0x30;
        i %= 100;
        s++;
    }
    
    if(i / 10 || number_started) //Get next digit if present.
    {
        number_started = TRUE;
        *s = i / 10 + 0x30;
        i %= 10;
        s++;
    }
    
    //Always do last steps.
    *s = i + 0x30; //Get last digit.
    s++;
    
    *s = 0; //Null terminate string.
    
    return 0;
}

//Converts a floating point number to a string. The valid range of the
//floating point number is 0 to 998.999. positive values only with
//a maximum of 4 digits past the decimal point. The precision value
//determines how many decimal places to take the conversion.
//Return values:
//0 = no errors.
//1 = float value < 0.
//2 = float value >= 999.000.
//3 = precision value is > 3.
uint8_t String_From_Float(const float f, char *s, const uint8_t precision)
{
    uint32_t temp_var; //Temporary variable used for calculations.
    uint8_t number_started = FALSE; //Indicate conversion has started.
    
    if(f < 0) //Negative numbers not supported.
    {
        return 1;
    }
    
    if(f >= 999.0f) //Float number too large.
    {
        return 2;
    }
    
    if(precision > 3) //High precision not supported.
    {
        return 3;
    }
    
    //convert float into integer.
    temp_var = (uint32_t)(f * 1000);
    
    //Do rounding first thing.
    if(precision == 0 && (temp_var % 1000 >= 500))
    {
        temp_var += 1000;
    }
    
    if(precision == 1 && (temp_var % 100 >= 50))
    {
        temp_var += 100;
    }
    
    if(precision == 2 && (temp_var % 10 >= 5))
    {
        temp_var += 10;
    }
    
    //Check for first digit.
    if(temp_var / 100000L)
    {
        number_started = TRUE;
        *s = temp_var / 100000L + 0x30;
        temp_var %= 100000L;
        s++;
    }
    
    //Get next digit if present.
    if(temp_var / 10000 || number_started)
    {
        number_started = TRUE;
        *s = temp_var / 10000 + 0x30;
        temp_var %= 10000;
        s++;
    }
    
    //Get next digit if present.
    if(temp_var / 1000 || number_started)
    {
        number_started = TRUE;
        *s = temp_var / 1000 + 0x30;
        temp_var %= 1000;
        s++;
    }
    
    //If no fractional component, finish up here.
    if(!precision)
    {
        *s = 0;     //Null terminate string.
        return 0;
    }
    
    //Add decimal point.
    *s = '.';
    s++;
    
    //Get tenths digit.
    *s = temp_var / 100 + 0x30;
    temp_var %= 100;
    s++;
    
    //If only tenths required, finish up here.
    if(precision == 1)
    {
        *s = 0;     //Null terminate string.
        return 0;
    }
    
    //Get hundredths digit.
    *s = temp_var / 10 + 0x30;
    temp_var %= 10;
    s++;
    
    //If only hundredths required, finish up here.
    if(precision == 2)
    {
        *s = 0;     //Null terminate string.
        return 0;
    }
    
    //Go all the way and get thousandths digit.
    *s = temp_var + 0x30;
    s++;
    
    *s = 0;     //Null terminate string.
    
    return 0;   
}
