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

const int minBright = 0;
const int maxBright = 255;
int timeInMs;
int input;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // opens serial port, sets data rate to 9600 bps
  Serial.begin(9600);
  timeInMs = 0;   
  input = 0;
}

// the loop function runs over and over again forever
void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
          // read the incoming byte:
          input = Serial.read();
          input -= 48;

          // say what you got:
          Serial.print("I received: ");
          Serial.println(input, DEC);
  }
  
  timeInMs = input * 1000;  
  
  // step through brighntess
  if (input == 0)
  {
    digitalWrite( LED_BUILTIN, LOW);
  }
  else
  {
    for (int brightness = minBright; brightness <= maxBright; brightness++)
    {
      analogWrite(LED_BUILTIN, brightness);   // enable the LED at varying brightness
      delay(timeInMs / maxBright);                               // delay to fade into next brightness
    }
  }
}
