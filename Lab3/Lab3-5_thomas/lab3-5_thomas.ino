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

int pin = 21;
volatile int state = LOW;

void setup()
{
  pinMode(pin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(pin), ISR, mode);
  
  //Serial.begin(9600);
}

void loop()
{
  digitalWrite(pin, state);

//  char key = keypad.getKey();
//
//  if (key != NO_KEY)
//  {
//    Serial.println(key);
//  }
}

void blink()
{
  state = !state;
}
