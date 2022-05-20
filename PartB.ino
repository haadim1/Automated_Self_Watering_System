#include <Arduino.h>
#include <U8x8lib.h>

#define MOSFET 2 // The MOSFET driver for the water pump on digital I/O 2
#define REDLED 4 // Big red LED on digital I/O 4
#define BUTTON 6 // Push button on digital I/O 6
#define MOISTURE A0 //Moisture Sensor

int moistureValue = 0;
auto display = U8X8_SSD1306_128X64_NONAME_HW_I2C(U8X8_PIN_NONE);
void setup() {
 pinMode(MOSFET, OUTPUT);
 pinMode(REDLED, OUTPUT);
 pinMode(BUTTON, INPUT); 
 display.begin();
 display.setFlipMode(0);
 display.clearDisplay();
 display.setFont(u8x8_font_profont29_2x3_r); 
 Serial.begin(9600);
}
 
void sendPotentiometerData() {
 const auto value = analogRead(MOISTURE);
 const byte data[] = {0, 0, highByte(value), lowByte(value)};
 Serial.write(data, 4);
 Serial.println();
}
 
void loop() {
moistureValue = analogRead(MOISTURE);
display.print("A0: " + String(moistureValue));
delay(500);
sendPotentiometerData();
 
while(moistureValue > 600 ) {
  digitalWrite(MOSFET,HIGH);
  moistureValue = analogRead(MOISTURE);}
digitalWrite(MOSFET,LOW);
 
 const auto receivedData = Serial.read();
 if (receivedData == 255){ 

digitalWrite(MOSFET,HIGH); }
 
  else if (receivedData == 1){
    digitalWrite(REDLED,HIGH);}
 
  else if (receivedData == 2){
    digitalWrite(REDLED,LOW);}
 
 else {digitalWrite(MOSFET,LOW); }
 display.clear();
 
}