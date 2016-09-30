// 9. Difficult (30+ min): Can you read user’s input to blink the LED for a certain times and keep it on after the blink? For example, if user’s input is 10, the LED should blink ten times and then keep on.

String input;   // serial input from console
int counter;    // counter of the number of blinks user wants

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
  // opens serial port, sets data rate to 9600 bps
  Serial.begin(9600); 
  
  // have the LED on by default
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
}

// the loop function runs over and over again forever
void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
          // read the incoming byte:
          input = Serial.readString();

          // convert input to int counter
          counter = input.toInt();

          // say what you got:
          Serial.print("I received: ");
          Serial.println(String(counter));
  }
    
  for (int i = 0; i < counter; i++)
  {
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(500); 
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);    
  }
  counter = 0;   
}
