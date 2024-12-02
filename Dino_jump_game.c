#define _XTAL_FREQ 4000000

#include<xc.h>
#include <stdio.h>
#include <math.h>
#include "ee302lc.h"
#include "i2c_ee302.h"

// Configs
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#ifndef _XTAL_FREQ
 // Unless already defined assume 4MHz system frequency
 // This definition is required to calibrate the delay functions, __delay_us() and __delay_ms()
 #define _XTAL_FREQ 4000000
#endif 


//definitions___________________________
#define SWITCH1 RB0
#define SWITCH2 RB1
#define PRESSED = 0
#define OPEN 1



// Global variables
unsigned int obstacleCounter = 0;
int obsArray[2][16];
int charArray[2][16];
uint8_t gameState = 1;
uint8_t highscore;
int *scorecounter;
uint8_t gCount;
unsigned char String_array[10];
volatile unsigned int seconds = 0;

// Function prototypes
void interrupt_setup();
void timer_setup();
void spawnObstacle();
void lcd_title(void);
void descend(void);
void USART_WRITE_TEXT(char *ptr);



void setup() //setup function
{
    Lcd8_Init(); // initialization of LCD to 8-bit mode
    i2c_init();
    PORTA = 0b00000000;
    PORTB = 0b00000100;
    PORTC = 0b11000011;
    TRISA = 0b00000010;
    TRISB = 0b00000001;
    TRISC = 0xD8; // RC6 and RC7 must be configured as inputs to enable the UART
    // RC4 and RC3 high from I2C_init
    TXSTA = 0x24; // Set TXEN and BRGH
    RCSTA = 0x90; // Enable serial port by setting SPEN bit
    SPBRG = 0x19; // Select 9600 baud rate.
    TRISD = 0x00; //PORTD all outputs
    PORTD = 0xff; //turns off all LED's on start
    OPTION_REG = 0x00; //enable internal pull up resistors
    INTCON = 0xC0; //enables global interrupts and peripherial interrupts
    T1CON = 0x11; //enables timer1, sets it to internal clock, sets prescaler to x2
    //(this was done to half frequency of LED's to correct value)
    //Note Timer1 will increment every Fosc/4 = 1us. With the prescaler
    //set to 2 Timer1 will now increment every 2us.
    TMR1H = 0x0B; //Pre-load Timer1 so it is not counting from 0.
    TMR1L = 0xDB; //sets timer to 3035 -> 65535 - 3035 = 62500
    TMR1IE = 1; //enables timer1 interrupt    
    TMR1IF = 0; //clear Timer 1 interrupt flag
//    ADC_Initialize();
}
void interrupt_setup() {
    INTCON = 0b10100000;  // Enable Timer0 overflow interrupt
}

void lcd_title(void)//should start up display
{
    Lcd8_Write_String("Lab Project"); // display string on LCD
    Lcd8_Set_Cursor(2, 0); // select line 2 of LCD 
    Lcd8_Write_String("Dino Run Game"); // display string on LCD      
}
void __interrupt() //interrupts function
isr()//interrupt service routine
 {
    if (TMR0IF) {
        TMR0IF = 0;    // Clear Timer0 overflow interrupt flag
        TMR0 = 100;    // Load Timer0 for next interrupt

        obstacleCounter++;
        if (obstacleCounter >= 1000)
        {
            spawnObstacle();
            obstacleCounter = 0;
        }
    }
}

void __airtime()
isr()
{
    if (TMR0IE && TMR0IF)
        {
        TMR0IF = 0;   // Clear Timer0 overflow flag
        TMR0 = 5;     // Reload Timer0 for a 5-second delay
        seconds++;

        if (seconds >= 1) {
            descend();
        }
}

void timer_setup() {
    T0CS = 0;     // Select internal instruction cycle clock
    PSA = 0;      // Assign prescaler to Timer0 module
    PS2 = 1;      // Set prescaler to 1:256
    PS1 = 1;
    PS0 = 1;

    TMR0 = 100;   // Load Timer0 for the first time

    TMR0IE = 1;   // Enable Timer0 overflow interrupt
    GIE = 1;      // Enable global interrupts
}


int spawnObstacle()
{
    for(int i = 0; i < 16; i++) 
    {
            int j = 15 - i;
            int tempArray[16] = obsArray[2][i];
            obsArray[i] = tempArray[j];
            return obsArray;
    }
}
void jump() 
{
    uint8_t jumpDuration = 10;  // Unsigned integer of 8 bytes
    for (uint8_t i = 0; i < jumpDuration; i++)
    {
        if (SWITCH1 == PRESSED)
            {
                charArray[2][3] = 1;
                charArray[1][3] = 0;
            }
        __airtime();
    }
}
void descend(void)
{
    for (int i = 0; i < 2; i++)
    {
        charArray[2][3] = 0;
        charArray[1][3] = 1;
    }
}
void collision(obsArray,charArray)
        {
            if(obsArray[1][3] = 1 && charArray[1][3] == 1)
            {
                gameState = 0;
            }
            else if(obsArray[1][3] = 1 && charArray[2][3] == 0 || obsArray[2][3] = 1 && charArray[1][3])
            {
                scorecounter++;
            } 
        }
void endgame(void)
        {
            if(gameState == 0)
            {
                Lcd8_Set_Cursor(1,8);
                Lcd8_Write_String("You Died");
                USART_Write_TEXT("Your highscore was ");
                read_EEPROM(0x00,0x10);//read low address
                USART_Write_TEXT(String_array);//write what is in the array
            }
        }

    void floor(void)
    {
        Lcd8_Set_Cursor(2,1);
        for(int i = 0; i < 15; i++)
        Lcd8_Write_String("_");
    }
    void roof(void)
    {
        Lcd8_Set_Cursor(1,1);
        for(int i = 0; i < 15; i++)
        Lcd8_Write_Char('-');
    }
    
void main() 
{
    interrupt_setup();
    timer_setup();

    while (1) {
        //do stuff here :)
    }

    return;
}
void USART_WRITE_TEXT(char *ptr)
{
    while(*ptr != '\0')
    {
        while(!TXIF)
        {
            TXREG = *ptr;
            ptr++;
        }
    }
}

void recordHighScore(char AddressHigh,char AddressLow,char *highscore) {
    i2c_start();//initialize i2c
   i2c_write(0xA0);//CB
   i2c_write(AddressHigh);//write to addresshigh
   i2c_write(AddressLow);//write to addresslow

   
    while(*scorecounter != '\0')
   if(*scorecounter > highscore)
   {
   {
       i2c_write(*scorecounter);//write 
       *scorecounter++;//increment
       
   }
    i2c_stop();//stop i2c
    __delay_ms(5);
   }
    continue; 
}
