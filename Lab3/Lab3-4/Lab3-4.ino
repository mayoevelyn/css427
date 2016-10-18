// 4.a. Difficult (60+ min): Implement a simple alarm system. You may use the keypad.addEventListener() method or other methods provided 
// in the keypad library. You are welcome to develop your own methods too.
// A simple “Alarm” button.
// i. When “#” has been hold for over 2 seconds, the onboard LED will be turned on.
// ii. When “*” is pressed, the onboard LED with be turned off.
// iii. The LED should not be turned off before “*” is pressed.

#include <Key.h>
#include <Keypad.h>
  
const byte rows = 4; //four rows
const byte cols = 3; //three columns
char keys[rows][cols] = 
{
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}             
};

byte rowPins[rows] = {18, 17, 16, 15}; //connect to the row pinouts of the keypad
byte colPins[cols] = {21, 20, 19}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

bool isLedOn;

// Setup
void setup()
{
  // Prepare keypad
  keypad.addEventListener(keypadEvent);  // Add an event listener.
  keypad.setHoldTime(2000);

  // Prepare led
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Prepare console output
  Serial.begin(9600);
}

void loop()
{
  char key = keypad.getKey();
  
  if (isLedOn)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void keypadEvent(KeypadEvent key)
{
  switch (keypad.getState())
  {
    case HOLD:
      if (key == '#')
      {
        isLedOn = true;
        Serial.println("led is on");
      }
      break;
    
    case PRESSED:
      if (key == '*')
      {
        isLedOn = false;
        Serial.println("led is off");
      }
      break;
  }
}

