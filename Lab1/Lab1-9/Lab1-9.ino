// 9. Difficult (30+ min): Can you read user’s input to blink the LED for a certain times and keep it on after the blink? For example, if user’s input is 10, the LED should blink ten times and then keep on.

int input; // counter of the number of blinks user wants

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
  // opens serial port, sets data rate to 9600 bps
  Serial.begin(9600); 
  
  // have the LED on by default
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  
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
    
  for (int i = 0; i < input; i++)
  {
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(500); 
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);    
  } 
  input = 0;    
}
