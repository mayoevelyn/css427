/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO 
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN takes care 
  of use the correct LED pin whatever is the board used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products
  
  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  modified 2 Sep 2016
  by Arturo Guadalupi
*/

const int blinkStart = 500;
const int blinkStop = 4000;
const int blinkStep = 500;

String message;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {

  for(int blinkInterval = blinkStart; blinkStart <= blinkStop; blinkInterval += blinkStep)
  {
    for (int count = 0; count < 10; count++)
    {      
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)      
      message = "Turned on light for " + String(blinkInterval) + "ms for count: " + String(count + 1);
      Serial.println(message);      
      delay(blinkInterval);              // wait a variable time depending on sequence
      
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      message = "Turned off light for " + String(blinkInterval) + "ms for count: " + String(count + 1);
      Serial.println(message);
      delay(blinkInterval);              // wait a variable time depending on sequence      
    }                 
  }
}
