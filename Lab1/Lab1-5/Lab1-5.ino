// 5. Medium (15 min): Write a loop to update the blink interval at from 500ms to 4000ms, with a step-up value of 500ms. At each interval, performs the blink activity for 10 times.

const int blinkStart = 500;
const int blinkStop = 4000;
const int blinkStep = 500;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {

  for(int blinkInterval = blinkStart; blinkStart <= blinkStop; blinkInterval += blinkStep)
  {
    for (int count = 0; count < 10; count++)
    {
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(blinkInterval);              // wait a variable time depending on sequence
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      delay(blinkInterval);              // wait a variable time depending on sequence
    }                 
  }
}
