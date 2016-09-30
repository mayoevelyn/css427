// 10. Difficult (30+ min): This is an optional exercise. Please finish this exercise when you have time. Can you read user’s input to change the speed of fading the LED’s brightness? Please use numbers to indicate the speed.

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
      delay(timeInMs / maxBright);            // delay to fade into next brightness
    }
  }
}
