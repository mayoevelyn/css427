// 7. Medium (15 min): Find a way to light the LED at varying brightnesses, you can use existing functions or develop your own function to do it.

const int minBright = 0;
const int maxBright = 255;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  for (int brightness = minBright; brightness <= maxBright; brightness++)
  {
    analogWrite(LED_BUILTIN, brightness);   // enable the LED at varying brightness
    delay(10);                               // delay to fade into next brightness
  }
}
