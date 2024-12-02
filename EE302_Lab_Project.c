#define _XTAL_FREQ 40000000

#include<xc.h>
#include <stdio.h>
#include "ee308302lcd.h"
#include "IC2_EE302.h"

#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

//prototype--------------------------------------------------------------------------
//etc etc


//globals
int x;
unsigned char eeprom_read_string[30];
static int count = 0;
unsigned char last_value, adc_channel;	
static int voltage;

//getting average temporary temperature
double temptemp = 0;



void setup() //setup function
{
    Lcd8_Init();  // initialization of LCD to 8-bit mode
    PORTA = 0b00000000;
    PORTB = 0b00000100;
    PORTC = 0b00000011;
    TRISA = 0b00000000;
    TRISB = 0b00000001;
    ADCON0 = 0b01010001;
    ADCON1 = 0b01000010;


    TRISC 	= 0xD8;		// RC6 and RC7 must be configured as inputs to enable the UART
						// RC4 and RC3 high from I2C_init 
	TXSTA 	= 0x24;		// Set TXEN and BRGH
	RCSTA 	= 0x90;		// Enable serial port by setting SPEN bit
	SPBRG	= 0x19;		// Select 9600 baud rate.



    TRISD = 0x00;				//PORTD all outputs
	PORTD = 0xff;				//turns off all LED's on bootup

	OPTION_REG = 0x00;			//enable internal pull up resistors
	INTCON = 0xC0;				//enables global interrupts and peripherial interrupts

	T1CON = 0x11;				//enables timer1, sets it to internal clock, sets prescaler to x2 
								//(this was done to half frequency of LED's to correct value)

								//Note Timer1 will increment every Fosc/4 = 1us. With the prescaler
								//set to 2 Timer1 will now increment every 2us.
	TMR1H = 0x0B;				//Pre-load Timer1 so it is not counting from 0.
	TMR1L = 0xDB;				//sets timer to 3035 -> 65535 - 3035 = 62500
								//62500*2us = 0.125s) -> on/off = 4Hz

	TMR1IE = 1;					//enables timer1 interrupt	
	TMR1IF = 0;					//clear Timer 1 interrupt flag
}

void lcd_title(void)//should start up display
	{
	Lcd8_Write_String("Lab Project");	// display string on LCD
	Lcd8_Set_Cursor(2,0);				// select line 2 of LCD	
	Lcd8_Write_String("EE302 - Temperature Sensor");	// display string on LCD	
	}

void ADC_Initialize() //initializing adc
{
  ADCON0 = 0b01000001; //ADC ON and Fosc/16 is selected
  ADCON1 = 0b11000000; // Internal reference voltage is selected
}

// unsigned int ADC_Read(unsigned char channel) 
// {
//     int x;
//     __delay_us(100); // acquisition time
//     GO_nDONE = 1;    // A/d Conversion
//     while (GO_nDONE); // wait till complete conversion;
//     x = 1;
//     float val = ADRESH * 50 /255;  // this is the same as ADRESH *5/255*10
//     x = val / 10;   //get number
//     show(x);//display 
       
// }

void Lcd_Start()//stRTING LCD SCREEN
{
  Lcd8_Init();
}

// void Lcd_Print_Char(char data)  //Send 8-bits through 4-bit mode
// {
   
// }

void calcADC(void)//calculate adc funciton
{
    float adc;
    float volt, temp;
    __delay_us(30);					//Acqusition time delay
    ADC_Initialize();
    while(1)
    {
        adc = (ADC_Read(4)); // Reading ADC values
        voltage = adc*4.88281; // Convert it into the voltage
        temperature = volt/10.0;  // Getting the temperature values
        temp1 = temperature*100;
     
        char c1 = (temp1/1000)%10; //getting first number of temperature

        char c2 = (temp1/100)%10; //getting second number of temperature

        char c3 = (temp1/10)%10; //getting third number of temperature

        char c4 = (temp1/1)%10; //getting fourth number of temperature
    }
    return temperature;
    return c1;
    return c2;
    return c3;
    return c4;
}


void Lcd_Print_Temperature(char *c1, char *c2,char *c3, char *c4)
{
    Lcd8_Clear();
        Lcd8_SetCursor(1,2);
        Lcd8_Print_String("Temperature");
        Lcd8_SetCursor(2,5);
        LC_IDcd8_Print_String(" = ");
        Lcd8_Write_Char(c1);
        Lcd_Write_Char(c2);
        Lcd_Write_String(".");
        Lcd_Write_Char(c3);
        Lcd_Write_Char(c4);

        //printing out the first no. of temperature then second the . then the 2 decimals
        lcd.Write_Char((char)223)//symbol for degrees in ascii
        Lcd_Write_String("C");//celcius
}



void __interrupt(static int voltage) //interrupts functiion
isr() {                         //interrupt service routine
if (TMR1IF == 1)				//Check if Timer1 Interrupt
	{
		TMR1IF=0;				//resets timer1 interrupt flag
	
		TMR1H = 0x0B;			//Pre-load Timer1 so it is not counting from 0.
		TMR1L = 0xDB;			//sets timer to 3035 -> 65535 - 3035 = 62500

        count++;
        if(count == 8) //if the no of interrupts reaches 8 (125ms per interrupt *8 = 1 second)
        { 
            for(int j = 0; j < 6; j++) //for loop to coount to 5 
            {
                temptemp = voltage + temptemp; //add the 5 voltages to each other
            }
            count = 0;//reset counter
        }
        	return temptemp;
	}
}



void transmit_string (const char* p)
{
	while (*p != '\0')			//While string does not equal Null character.
	{
		while (!TXIF);			//Wait until TXREG empty.
		TXREG = *p;				//Load TXREG with character from string pointed to
		p++;					// by p, then increment p.
	}
}

void read_string(unsigned char address_hi,unsigned char address_lo, unsigned char data[], int length, double temptemp)
{
    int i;						//Declare i as an integer variable

    i2c_start();				//Send Start Condition
    i2c_write(0xa0);			//Write Control Byte (A2,A1,A0 all low, R/W = 0)
    i2c_write(address_hi);		//Write high byte of address 
    i2c_write(address_lo);		//Write low byte of address 
    i2c_repStart();				//Send reStart Condition
    i2c_write(0xa1);			//Write Control Byte (A2,A1,A0 all low, R/W = 1)
    
    for(i=0; i<length-1;i++)
    {
        data[i]=i2c_read(1);	//Sequential read with ACK until length-1
    }

	i++;
	data[i]=i2c_read(0);		//Read final byte followed by a NACK
		
    i2c_stop();					//Send Stop condition

}

void write_string(unsigned char address_hi,unsigned char address_lo, const char* ptr)
{
    i2c_start();				//Send Start Condition
    i2c_write(0xa0);			//Write Control Byte (A2,A1,A0 all low, R/W = 0)
    i2c_write(address_hi);		//Write high byte of address 
    i2c_write(address_lo);		//Write low byte of address 
   while(*ptr !='\0')
   {
		i2c_write(*ptr);		//Sequential write of data until end of string
		ptr++;
   }
   i2c_stop();					//Send Stop condition
   __delay_ms(5);				//Necessary 5ms delay
}





int main(double temptemp)
{
    Lcd_Print_Temperature();
    i2c_init();										
	setup();												
	write_string(0x00, 0x01, "Average Voltage = ");		//write out a string to eprom using i2c	ack the memory now stored on the eprom using i2c#
    write_string(0x00, 0x02,temptemp)
	read_string(0x00,0x01,eeprom_read_string, 30);	//read back the memory now stored on the eprom using i2c
	transmit_string(eeprom_read_string); 			//write string to PC using USART	
    
    for(;;)
    {

    }
    return 0;
}
