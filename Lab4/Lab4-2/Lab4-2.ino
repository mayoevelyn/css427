#include <Key.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include "Adafruit_LSM303_U.h"

// detect Arduino version
#if ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

// determine board type
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
    String boardName = "Arduino Uno or older";
    int boardType = 0;
#elif defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
    String boardName = "Arduino Micro or Leonardo";
    int boardType = 1;
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    String boardName = "Arduino Mega";
    int boardType = 2;
#else
    String boardName = "Unknown board.  Check wikipedia";
    int boardType -1;
#endif

// Setup keyboard
const byte rows = 4; //four rows
const byte cols = 3; //three columns
char keys[rows][cols] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};
byte rowPins[rows] = {28, 26, 24, 22}; //connect to the row pinouts of the keypad
byte colPins[cols] = {34, 32, 30}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

SoftwareSerial mySerial(10, 11); // RX, TX
bool incomingData;

// Setup
void setup()
{
    Serial.begin(9600);
    delay(1000);
    Serial.println("Booting " + boardName);

    // set the data rate for the SoftwareSerial port
    mySerial.begin(4800);
    incomingData = false;
}

// Loop
void loop()
{
    char key = keypad.getKey();
    
    if (boardType == 2 && key != NO_KEY)
    {
        mySerial.println("From mega: " + String(key));
    }

    if (mySerial.available())
    {
        while (mySerial.available())
        {
            Serial.write(mySerial.read());
            delay(1);
        }
        incomingData = true;
    }
    
    if (boardType == 1 && incomingData)
    {
        mySerial.println("ACK!");
        incomingData = false;
    }
}
