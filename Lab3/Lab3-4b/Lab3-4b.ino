/* 
4.b Difficult (60+ min): Implement a simple alarm system. You may use the keypad.addEventListener() method or other methods provided 
in the keypad library. You are welcome to develop your own methods too.
An advanced alarm system
i. If “#” and “*” are pressed at the same time, keep sending alarm messages to the host PC, and blinking the onboard LED, until 
both “#” and “*” are released. */

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
    isLedOn = false;
    isLedBlinking = false;
    isAsteriskPressed = false;
    isOctothorpePressed = false;
}

void loop()
{    
    loopCount++;
    if ( (millis()-startTime)>1000 ) 
    {
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
                if (keypad.key[i].kstate == PRESSED) 
                {                   
                    if (keypad.key[i].kchar == '*')
                    {
                      isAsteriskPressed = true;
                    }
                    
                    if (keypad.key[i].kchar == '#')
                    {
                      isOctothorpePressed = true;
                    }
                }
            }
        }      
    }

    if (isAsteriskPressed && isOctothorpePressed)
    {
        alarm();    
    }  
}

void alarm()
{
    Serial.println("Alarm!");
    isLedBlinking = true; 
    if (keypad.getKeys())
    {
        for (int i=0; i < LIST_MAX; i++)   // Scan the whole key list.
        {
            if (keypad.key[i].stateChanged)   // Only find keys that have changed state.
            {
                if (keypad.key[i].kstate == RELEASED) 
                {                  
                    if (keypad.key[i].kchar == '*')
                    {
                      isAsteriskPressed = false;
                      isLedBlinking = false;
                      isLedOn = false;
                    }
                    
                    if (keypad.key[i].kchar == '#')
                    {
                      isOctothorpePressed = false;
                      isLedBlinking = false;
                      isLedOn = false;
                    }                  
                }
            }
        }      
    } 
}


