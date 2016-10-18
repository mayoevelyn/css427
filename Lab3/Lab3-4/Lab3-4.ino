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

bool enableLed;
bool isLedOn;
bool isAsteriskPressed;
bool isOctothorpePressed;
unsigned long loopCount;
unsigned long startTime;

// Setup
void setup()
{
  // Initialize globals
  enableLed = false;
  isLedOn = false;
  isAsteriskPressed = false;
  isOctothorpePressed = false;
  loopCount = 0;
  startTime = 0;

  // Prepare keypad
  keypad.setHoldTime(2000);
  
  // Prepare led
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Prepare console output
  Serial.begin(9600);
}

// Loop
void loop()
{
  scanKeys();
  testAlarm();
  setLED();
}

// Scan Keys
void scanKeys()
{
  if (keypad.getKeys())
  {
    for (int i=0; i < LIST_MAX; i++)   // Scan the whole key list.
    {
      if (keypad.key[i].stateChanged)   // Only find keys that have changed state.
      {
        if (keypad.key[i].kstate == PRESSED) 
        {                   
          if (keypad.key[i].kchar == '*')
          {
            isAsteriskPressed = true;
            enableLed = false;
            Serial.println("LED is off");
          }
          
          if (keypad.key[i].kchar == '#')
          {
            isOctothorpePressed = true;
          }
        }

        if (keypad.key[i].kstate == HOLD) 
        {                   
          if (keypad.key[i].kchar == '#')
          {
            isOctothorpePressed = true;
            enableLed = true;
            Serial.println("LED is on");
          }
        }

        if (keypad.key[i].kstate == RELEASED) 
        {                   
          if (keypad.key[i].kchar == '*')
          {
            isAsteriskPressed = false;
          }
          
          if (keypad.key[i].kchar == '#')
          {
            isOctothorpePressed = false;
          }
        }
      }
    } 
  }
}

// Test Alarm
void testAlarm()
{
  loopCount++;
  if ((millis() - startTime) > 100) 
  {
    if (isAsteriskPressed && isOctothorpePressed)
    {
      isLedOn = !isLedOn;
      Serial.println("Alarm!");
    }
    else
    {
      isLedOn = enableLed;
    }
    
    startTime = millis();
    loopCount = 0;
  }
}

// Set LED
void setLED()
{
  if (isLedOn)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

