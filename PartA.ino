// Import libraries
#include <Arduino.h>
#include <U8x8lib.h>

// Make synonyms using #define.  No semi-colons needed here.
#define MOSFET 2   // The MOSFET driver for the water pump on digital I/O 2
#define REDLED 4   // Big red LED on digital I/O 4
#define BUTTON 6   // Push button on digital I/O 6
#define MOISTURE A1
#define WET_THRESH 600

// Creates an instance of the `U8X8_SSD1306_128X64_NONAME_HW_I2C` class
auto display = U8X8_SSD1306_128X64_NONAME_HW_I2C(U8X8_PIN_NONE);

// Set up variable for measuring time.
// https://www.arduino.cc/reference/en/language/functions/time/millis/
unsigned long elapsedTime = 0;       // "global" variable as long integer, positive values only.
unsigned long startTime = 0;         // "global" variable as long integer, positive values only.
int moistureValue = 0;


/* -------------- Initialize the Grove board ------------- */
void setup() {
    Serial.begin(9600);
    pinMode(MOSFET, OUTPUT); // Sets the D2 pin (MOSFET + Pump) to output
    pinMode(REDLED, OUTPUT); // Sets the D4 pin (LED) to output
    pinMode(BUTTON, INPUT); // Sets the D6 pin (Button) to input
    digitalWrite(MOSFET, LOW);    // pump off
    
    display.begin(); // start up the OLED display
    display.setFlipMode(0); // set to 1 or 0, depending on orientation of board
    display.clearDisplay();                     // blank display
    display.setFont(u8x8_font_profont29_2x3_r); // set font

    // Record initial time.
    startTime = millis();
}

/* --------------- Run this over and over ------------------- */
void loop() {

    // what time is it now?
    elapsedTime = millis()-startTime;

    // Read soil moisture sensor & assign to a variable
    moistureValue = analogRead(MOISTURE);
    
    // place the cursor @ position x=0, y=0
    display.setCursor(0,0);

  if(digitalRead(BUTTON) == HIGH) // button pressed
  {
      digitalWrite(MOSFET, HIGH);    // pump on
      digitalWrite(REDLED, HIGH);    // LED on

      display.print("water!  ");     // msg on OLED

      // place the cursor @ position x=0, y=3
      display.setCursor(0,5);
      // Display elapsed time on OLED
      display.print("t: " + String(elapsedTime));
   } 
   else if(moistureValue > WET_THRESH)
   {
      digitalWrite(MOSFET, HIGH);    // pump on
      digitalWrite(REDLED, HIGH);    // LED on

      display.print("water! ");

      // place the cursor @ position x=0, y=5
      display.setCursor(0,5);
      // Display Moisture value on OLED
      // ~750 when dry, ~500 when wet
      display.print("A1: " + String(moistureValue));

   }
    else if(moistureValue <= WET_THRESH)
   {
      digitalWrite(MOSFET, LOW);    // pump off
      digitalWrite(REDLED, LOW);    // LED off

      display.print("no water");

      // place the cursor @ position x=0, y=5
      display.setCursor(0,5);
      // Display Moisture value on OLED
      // ~750 when dry, ~500 when wet
      display.print("A1: " + String(moistureValue));

   }
   if(moistureValue <= WET_THRESH){
    Serial.write("wet!");
   }
   
}