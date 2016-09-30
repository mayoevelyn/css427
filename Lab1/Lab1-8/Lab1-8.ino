// 8. Difficult (30+ min): Can you read user’s input to 1) turn on/off and 2) toggle the LED? Try your best to develop such a sketch (program). “Toggle” means if the LED is on, you turn it off, if the LED is off, you turn it on.

int state, input; // 0 is off, 1 is on, 2 is toggle

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
  // opens serial port, sets data rate to 9600 bps
  Serial.begin(9600);

  // initialize state to 0 (off)
  state = 0;
  input = -1;
}

// the loop function runs over and over again forever
void loop() {

  // send data only when you receive data:
  if (Serial.available() > 0) {
          // read the incoming byte:
          input = Serial.read();
          input -= 48;  // ascii conversion

          // say what you got:
          Serial.print("I received: ");
          Serial.println(input, DEC);
  }

  // process user input
  // turn off
  if (input == 0)
  {
    state = 0;
  }
  // turn on
  else if (input == 1)
  {
    state = 1;
  }
  // toggle
  else if (input == 2)
  {
    if (state == 0)
    {
      state = 1;
    }
    else
    {
      state = 0;
    }
  }

  // reset user input
  input = -1;

  // turn LED on or off depending on state
  if (state == 0)
  {
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  }
  else
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
}
