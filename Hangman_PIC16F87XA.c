/** EE302 Project - HANGMAN
 * 
 * Code should select a random word and the user should have to try guess it
 * using Putty as an INPUT. Correct guesses should be displayed to the LCD.
 *
 * Target System:   PIC16F877A
 * 
 * Created:         Louis Dwyer & Cian Winder
 *                  30/11/23
 */


// configuration bits___________________________________________
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)



// includes ____________________________________________________

#include	<xc.h>
#include    <stdio.h>
#include    <stdlib.h>

#include    "I2C_EE302.h"
#include    "ee302lcd.h"

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif 


// types _______________________________________________________



// constants ___________________________________________________

#define ON      1
#define OFF     0
#define OPEN    1          // Define switch action "OPEN" as 1
#define CLOSED  0		// Define switch action "Closed" as 0
#define Inc     RB0			//button used to increase counter once to a maximium of 10
#define Dec     RB1			//button used to decrease counter once to a minimium of 0
#define Reset   RB2		//button used to reset counter to zero

#define SUPERLOOP_DELAY 100


#define BUTTON1 RB0 
#define BUTTON2 RB1 
#define BUTTON3 RB2 
#define BUTTON4 RB3


#define LED1	RC0
#define LED2	RC1
#define SPK     RC2
#define POT     RA0


// globals _____________________________________________________

unsigned char gOutString[16];	

unsigned char data_read[15];

int lives = 6;

char guessedLetters[lives];//number of lives in game

char Enter_value;//value entered for putty input

const char wordBank[10] = {"Hello", "World", "Distribute", "Keyboard", "Laptop", "Ghoul", "Viable", "Business", "Creativity", "Sleepy"};

// prototypes __________________________________________________

void setup(void);
void loop(void);

//interrupts
void __interrupt();

//LCD
void data2LCD(char*);
void clearOutString(void);
void lcdTitle(void);

//eeprom
void write_ext_eeprom_(unsigned char address_hi,unsigned char address_lo, unsigned char *data);
unsigned char read_ext_eeprom_(unsigned char address_hi,unsigned char address_lo, int len);

//putty
char USART_Read(void);
void USART_Write_TEXT(char *string_print);
void putty(void);

//hangman logic functions
void send_Str(char arr[], int len);
//void sendString(char*);//idk where this function is 
char * randWord(void);
char * hideWord(char*);

//potentiometer
int whatIsThisLittleGuyDoing(void);

// top level functions _________________________________________


void main(void)  {
    
	setup();

	while(1) {
        loop();	
	}
}


void setup(void) {
    
    //Tris Registers
    TRISA = 0b00000001;
    TRISB = 0b00000001;
    TRISC = 0b11011111;
    TRISD = 0b00000000;
    
    //Ports
    PORTD = 0xFF;//in the i2c code
    
    //EEPROM
    TXSTA = 0x24;   //Set TXEN bit to enable transmit.
    RCSTA = 0x90;   //Set CREN bit for continuous read.
    SPBRG = 0x19;   //Set Baud Rate to 9600
    
    //Interrupts 
    OPTION_REG = 0x00; //enable internal pull up resistors
    INTCON = 0xC0; //enables global interrupts and peripheral interrupts
    
    //Timers
    T1CON = 0x11; //enables timer1, sets it to internal clock, sets pre-scaler to x2
    TMR1L = 0xDB; //sets timer
    TMR1IE = 1; //enables timer1 interrupt	
    TMR1IF = 0; //clear Timer 1 interrupt flag
    
    
    //I2C
    i2c_init();
    
    //Title screen for LCD
    lcdTitle();
    
}


void loop(void) {
 	__delay_ms(SUPERLOOP_DELAY);
    
    char * wordToGuess = randWord();
    data2LCD(hideWord(wordToGuess));
    
}





void __interrupt() 
    isr(void){ //Interrupt Service Routine
        
    
    }


char * randWord() { // Selects random word from file
    int wordIndex = rand() % 10 + 1;
    char* outWord = wordBank[wordIndex];
    return outWord;
}

char * hideWord(char * wordToHide) { // hides the chosen word to display to LCD
    int wordLen = sizeof wordToHide;
    char hiddenWord[16];

    for (int i = 0; i < wordLen; i++) {
        hiddenWord[i] = '_ ' ;
    }
    char * outWord = hiddenWord;
    return outWord;
}

int whatIsThisLittleGuyDoing(void){//uses potentiometer to determine what word in array used
    
    int totalPot = maxNumber;//I thonk its 100k ohm
    int arrnum = totalPot/10;//this divides up the the words in the array
    int iteration=POT/arrnum;
    return iteration;
}
void checkletter(char* word,char* hiddenword){//checks letter input from putty with wordToHide
	String str = word; // Given String
	Enter_value = USART_Read();//entered value for putty
	
	char[] arr = new char[str.length()];// Creating array of string length
    
    for (int i = 0; i < str.length(); i++) {// Copy character by character into array
      arr[i] = str.charAt(i);
    }
	for (int i = 0; i < str.length(); i++){
		if(arr[i]==Enter_value){
			 hiddenword[i] = Enter_value;
		}
	else 
		lives = lives-1;
	}
    
    // char[] arr = new char[str.length()];// Creating array of string length
    
    // for (int i = 0; i < str.length(); i++) {// Copy character by character into array
      // arr[i] = str.charAt(i);
    //}
}


void send_Str(char arr[], int len){
    
    for(int i = 0; i < len-1; i++){
        while(!TXIF);   // set when transmit register (TXREG) is empty
        TXREG = arr[i];   
    }
}

void data2LCD(char * hiddenWord) {
    static int counter = 0;
    
    Lcd8_Clear();                       //clear LCD display
    clearOutString();                   //clear outString array
      
    sprintf(gOutString, hiddenWord);               //define string as "int Value is"
    Lcd8_Write_String(gOutString);		//print string to LCD
    Lcd8_Set_Cursor(2,1);                 //select line 2 of LCD
    sprintf(gOutString, "placeholder");    //define string as "int Value is"
//    counter = 0;                          //resets counter to zero
//    sprintf(gOutString,"== %d",counter);	//define intvalue as a char in outString
//    Lcd8_Write_String(gOutString);		//print string to LCD
      
      
}

void lcdTitle(void) {
	Lcd8_Write_String("EE302 Project");		// print "EE302 Project" on line 1 of LCD
	Lcd8_Set_Cursor(2,1);                   // select line 2
	Lcd8_Write_String("---HANGMAN---");		// print "---HANGMAN---" on line 2 of LCD
}

void clearOutString(void){
    for (int i = 0; i < 16; i++) {
        gOutString[i] = 0x00;
    }
}



void write_ext_eeprom_(unsigned char address_hi,unsigned char address_lo, unsigned char *data) {
   i2c_start();					//Send Start Condition
   i2c_write(0xa0);				//Write Control Byte (A2,A1,A0 all low, R/W = 0)
   i2c_write(address_hi);		//Write high byte of address 
   i2c_write(address_lo);	    //Write low byte of address 
   
   while  (*data != '\0'){
       i2c_write(*data);        //writing data sequentially 
       *data++;                 //incrementing string 
   }
   i2c_stop();					//Send Stop condition
   
   __delay_ms(5);				//Necessary 5ms delay for write to propagate
}

unsigned char read_ext_eeprom_(unsigned char address_hi,unsigned char address_lo, int len) {
   
    i2c_start();				//Send Start Condition
    
    i2c_write(0xa0);			//Write Control Byte (A2,A1,A0 all low, R/W = 0)
    i2c_write(address_hi);		//Write high byte of address 
    i2c_write(address_lo);		//Write low byte of address 
    i2c_repStart();				//Send reStart Condition
    i2c_write(0xa1);			//Write Control Byte (A2,A1,A0 all low, R/W = 1)
    
    
    for(int j = 0; j < len-1; j++){
        data_read[j] = i2c_read(1);
    }
    
    i2c_read(0);
    i2c_stop();                 //Stop condition
    
    send_Str(data_read, len);
}

char USART_Read(void) //read function 
{

    while(!RCIF);
    return RCREG;
}

void USART_Write_TEXT(char *string_print) //function to write text to putty terminal
{
    while(*string_print != '\0')//while string isnt a null
    {
        while(!TXIF);//while not the USART transmit flag bit 
        TXREG = *string_print; // process it as a byte data
        string_print++;//increment the string to be sent 
    }
}

void putty(void) {//function for putty input
      Enter_value = USART_Read();//take input
       
        if (Enter_value == '1') //if the user inputs 1
        {
            PORTDbits.RD0=0;//set LED1 on
            USART_Write_TEXT("\n\r LED 1 lit ");
            USART_Write_TEXT("\n\r Next Value :"); 
            //^Write to putty

        }
        if (Enter_value == '2')//if the user inputs 2
        {
            PORTDbits.RD1=0;//set LED2 on
            USART_Write_TEXT("\n\r LED 2 lit ");  
            USART_Write_TEXT("\n\r Next Value :");  
            //^Write to putty
        }
        if (Enter_value == '3')//if the user inputs 3
        {
            PORTDbits.RD2=0;//set LED3 on
            USART_Write_TEXT("\n\r LED 3 lit ");  
            USART_Write_TEXT("\n\r Next Value :");  
            //^Write to putty
        }
        if (Enter_value == '4')//if the user inputs 4
        {
            PORTDbits.RD3=0;//set LED4 on
            USART_Write_TEXT("\n\rr LED 4 lit");
            USART_Write_TEXT("\n\r Next Value :");
            //^Write to putty

        }
       
        if (Enter_value == '0') //If the user enters "0"
        {
           PORTDbits.RD0=1; //Turn off LED
           PORTDbits.RD1=1; //Turn off LED
           PORTDbits.RD2=1; //Turn off LED
           PORTDbits.RD3=1; //Turn off LED
           USART_Write_TEXT("\n\r ALL LEDs are off ");
           USART_Write_TEXT("\n\r Next Value :");
           //^Write to putty
        }
       
}