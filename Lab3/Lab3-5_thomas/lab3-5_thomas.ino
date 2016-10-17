//#include <Key.h>
//#include <Keypad.h>
//
//const byte rows = 4; //four rows
//const byte cols = 3; //three columns
//char keys[rows][cols] = {
//  {'1','2','3'},
//  {'4','5','6'},
//  {'7','8','9'},
//  {'*','0','#'}
//};
//byte rowPins[rows] = {18, 17, 16, 15}; //connect to the row pinouts of the keypad
//byte colPins[cols] = {21, 20, 19}; //connect to the column pinouts of the keypad
//Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

const byte ledPin = 13;
const byte interruptPin = 20;
volatile byte state = LOW;

void setup()
{
  //https://www.arduino.cc/en/Reference/AttachInterrupt
  //http://rasas.me/projects/arduino/keypad-activated-interrupt
  //https://www.parallax.com/sites/default/files/downloads/27899-4x4-Matrix-Membrane-Keypad-v1.2.pdf
  
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, FALLING);
  
  Serial.begin(9600);
}

void loop()
{
  digitalWrite(ledPin, state);

//  char key = keypad.getKey();
//
//  if (key != NO_KEY)
//  {
//    Serial.println(key);
//  }
}

// This is the ISR
void blink()
{
  state = !state;
  Serial.println("went to low");
}
