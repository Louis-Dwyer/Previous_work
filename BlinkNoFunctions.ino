/**
 * Blinking various BarLEDs without using functions.
 * This results in quite a lot of repetitive code which differs only in the
 * specific LED to blink or the number of times to blink it and the length
 * of time to blink on and off.
 */
#include <pinouts.h>

// ___________________________________________________________________
// constants

const int SHORT_ON_MS = 100;
const int SHORT_OFF_MS = 500;
const int MED_ON_MS = 500;
const int MED_OFF_MS = 700;
const int LONG_ON_MS = 1200;
const int LONG_OFF_MS = 600;

// ===================================================================
// Top level functions
// ===================================================================

/**
 * set up the sketch to use LEDs and serial
 */
void setup() {
  int ledIndex;
  
  // set up the LEDs
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  // set up the Bar LEDs
//  for (ledIndex = 0; ledIndex < NUM_BAR_LEDS; ledIndex++)
//    pinMode(BAR_LED_1_PIN + ledIndex, OUTPUT);
  
  // serial output
  Serial.begin(9600);

  Serial.println("====== Lab4_BlinkNoFunctions starting ========");

  setupBarLeds();//calling the setup function
  
  blinkBarLED(6, 3, LONG_ON_MS, LONG_OFF_MS);//function to control the bar LED 
}

/**
 * Blink the LEDs in accordance with the lab requirements each
 * time the loop function is called
 */
 
void loop() {
//  static int loopNum = 0;
//  int i;
//  
//  Serial.print("Loop ");
//  Serial.println(loopNum);
}

int setupBarLeds()//function to set up the BAR LEDs
{
  for (int ledIndex = 0; ledIndex < NUM_BAR_LEDS; ledIndex++)
    pinMode(BAR_LED_1_PIN + ledIndex, OUTPUT);
}


//x=LED bar number
//y=number of blinks
//z=blink on duration
//s=blink off duration
int blinkBarLED(int x, int y, int z, int s)
{
  static int loopNum = 0;//define the loop number outside of loop as this has to change with each loop
  for (int i=0; i < y; i++) {//for loop to make the light blink however many times desired
  Serial.print("Loop ");
  Serial.println(loopNum);
  digitalWrite(BAR_LED_1_PIN + x, HIGH);//pin + x to select the LED that you want to light
  delay(z);// variable for the ms on to control it via a function
  digitalWrite(BAR_LED_1_PIN + x, LOW);
  delay(s);//variable for the ms off to control it via a function
  loopNum++;//add to the loop to count the number of loops
  }
}

//  Serial.print("Loop ");
//  Serial.println(loopNum);
//
//  // BarLED1
//  Serial.println("  Blink BarLED1 on/off long pattern x 1");
//  digitalWrite(BAR_LED_1_PIN, HIGH);
//  delay(LONG_ON_MS);
//  digitalWrite(BAR_LED_1_PIN, LOW);
//  delay(LONG_OFF_MS);

//  // BarLED8
//  Serial.println("  Blink BarLED8 on/off short pattern x 3 times");
//  for (int i=0; i < 3; i++) {
//    digitalWrite(BAR_LED_8_PIN, HIGH);
//    delay(SHORT_ON_MS);
//    digitalWrite(BAR_LED_8_PIN, LOW);
//    delay(SHORT_OFF_MS);
//  }
//
//  // BarLED4
//  Serial.println("  Blink BarLED4 on/off short pattern x 2 times");
//  for (int i=0; i < 2; i++) {
//    digitalWrite(BAR_LED_4_PIN, HIGH);
//    delay(MED_ON_MS);
//    digitalWrite(BAR_LED_4_PIN, LOW);
//    delay(MED_OFF_MS);
//  }

  // there are plenty of delays in the blinking code so we don't
  // need to add any extra delay at the end of the loop function
//}
