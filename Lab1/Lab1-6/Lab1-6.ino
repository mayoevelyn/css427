// 6. Medium (15 min): Report the iterations and blink interval to serial port and capture the received data in your serial monitor.

const int blinkStart = 500;
const int blinkStop = 4000;
const int blinkStep = 500;

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
      Serial.println("Turned on light for " + String(blinkInterval) + "ms for count: " + String(count + 1));      
      delay(blinkInterval);              // wait a variable time depending on sequence
      
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      Serial.println("Turned off light for " + String(blinkInterval) + "ms for count: " + String(count + 1));
      delay(blinkInterval);              // wait a variable time depending on sequence      
    }                 
  }
}
