/* 
Difficult (60+ min): Implement a simple alarm system. You may use the keypad.addEventListener() method or other methods provided 
in the keypad library. You are welcome to develop your own methods too.
a. A simple “Alarm” button.
i. When “#” has been hold for over 2 seconds, the onboard LED will be turned on.
ii. When “*” is pressed, the onboard LED with be turned off.
iii. The LED should not be turned off before “*” is pressed.
b. An advanced alarm system
i. If “#” and “*” are pressed at the same time, keep sending alarm messages to the host PC, and blinking the onboard LED, until 
both “#” and “*” are released. */

#include <Key.h>
#include <Keypad.h>
  
const byte rows = 4; //four rows
const byte cols = 3; //three columns
char keys[rows][cols] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[rows] = {17, 16, 15, 14}; //connect to the row pinouts of the keypad
byte colPins[cols] = {20, 19, 18}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

bool isLedOn;
bool isLedBlinking;
bool isAsteriskPressed;
bool isOctothorpePressed;

unsigned long loopCount;
unsigned long startTime;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  loopCount = 0;
  startTime = millis();
  pinMode(LED_BUILTIN, OUTPUT);
  keypad.addEventListener(keypadEvent);  // Add an event listener.
  keypad.setHoldTime(2000);
  isLedOn = false;
  isLedBlinking = false;
  isAsteriskPressed = false;
  isOctothorpePressed = false;
}

void loop()
{
  loopCount++;
  if ( (millis()-startTime)>1000 ) {
      Serial.print("Average loops per second = ");
      Serial.println(loopCount);

      if (isLedBlinking)
      {
        isLedOn = !isLedOn;
      }
      
      startTime = millis();
      loopCount = 0;
  }

  char key = keypad.getKey();
  
  if (isLedOn)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }

  if (keypad.getKeys())
  {
      for (int i=0; i < LIST_MAX; i++)   // Scan the whole key list.
      {
          if (keypad.key[i].stateChanged)   // Only find keys that have changed state.
          {
              switch (keypad.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                  case PRESSED:
                    if (keypad.key[i].kchar == '*')
                    {
                      isAsteriskPressed = true;
                    }
                    if (keypad.key[i].kchar == '#')
                    {
                      isOctothorpePressed = true;
                    }
              break;
                  case RELEASED:
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

  if (isAsteriskPressed && isOctothorpePressed)
  {
    freakOut();
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

void freakOut()
{
  Serial.println("Really freaking out here, man.");
}

