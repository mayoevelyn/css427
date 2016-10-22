// 1. Difficult (60 min): Implement the wired communication between two Arduino devices.
//    NOTICE: You ONLY NEED ONE PROGRAM for Arduino Mega and Arduino Micro.
//    You just need to select the correct board, CPU, and port when compiling and downloading the program.
// a. You need to first connect the Arduino Mega and Micro devices.
// b. Following the SoftwareSerial example to pick two ports on each Arduino device as the UART port.
// c. Connect the keypad you used in Lab 3 into the Mega device
// d. Connect both Mega and Micro to the PC via USB
// e. Press the key on the keypad that is connected the Mega device, and let the Mega sends the captured
//    key value to the Micro (you may add extra information besides the key value), then the Micro should
//    send (forward) the information received form Mega to PC so as to be printed in SerialMonitor.
// f. Once you have implemented the previous step, as an acknowledgement method
//    to the Micro device: after the Micro sends data to PC, it should also send back
//    an ACK to the Mega, so that Mega actually knows that the message is received
//    by the Micro. The Mega device should also print out the received ACK to the
//    host PC. After this step, you should see information about the pressed key, and
//    the ACK information in two separated SerialMonitor windows.
//    NOTICE: how to open two SerialMonitor?
// i. Open two Arduino IDEs from the start menu. It’s not opening two sketches, but two IDEs.
// ii. Open one Arduino IDE, and open one serial terminal (e.g., Putty)

#include <Key.h>
#include <Keypad.h>
#include <SoftwareSerial.h>

// Detect Arduino version
#if ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

// Determine board type
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

// Setup device interconnect over serial
SoftwareSerial mySerial(10, 11); // RX, TX

// Global variables
bool incomingData;

// Setup
void setup()
{
    Serial.begin(9600);
	
    delay(1000);
    Serial.println("Booting " + boardName);

    // Set the data rate for the SoftwareSerial port
    mySerial.begin(4800);
    incomingData = false;
}

// Loop
void loop()
{
    // Save any single input from the keypad
	char key = keypad.getKey();
    
	// Transmit key from Mega
    if (boardType == 2 && key != NO_KEY)
    {
        mySerial.println("From mega: " + String(key));
    }

	// Read transmitted key or ACK on either device
    if (mySerial.available())
    {
        while (mySerial.available())
        {
            Serial.write(mySerial.read());
            delay(1);
        }
        incomingData = true;
    }
    
	// Transmit ACK from Micro after data was received
    if (boardType == 1 && incomingData)
    {
        mySerial.println("ACK!");
        incomingData = false;
    }
}
