// 2. Ninja (90min): Connect the sensor board to the Arduino Micro device over I2C interface.
// a. Sensor board information: https://www.adafruit.com/products/1120. You can
//    also find example code there.
// b. You may download the code provided by Adafruit, but you need to change it by
//    splitting the read function to two functions: one is to read accelerometer only,
//    and the other one is to read magnetometer only.
// c. Changing the code that you have wrote in the first exercise. Once key “1” is
//    pressed on the Mega device, the accelerometer information should be collected
//    on the Micro device and sent to PC. The Micro also ACKs the Mega device for
//    the received command information. Once key “2” is pressed, the magnetometer
//    information should be collected on the Micro device and sent to PC. The Micro
//    also ACKs the Mega device for the received command information. Once key “3”
//    is pressed, information about both accelerometer and magnetometer
//    information should be collected on the Micro device and sent to PC, and the
//    Micro should ACK the command. You need to show the data printed at two
//    SerialMonitor windows.

#include <Key.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

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

// Assign a unique ID to this acceleration sensor
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

// Assign a unique ID to this compass sensor
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

// Define behavior modes for Micro
enum sendTypes {
    NONE = 0,
    ACCEL = 1,
    MAG = 2,
    BOTH = 3
};

// Global variables
bool incomingData;
int sendDataType;

// Setup
void setup()
{
    #ifndef ESP8266
        while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
    #endif
    
    Serial.begin(9600);
	
    delay(1000);
    Serial.println("Booting " + boardName);
    
	sendDataType = NONE;

	// Display sensor test data to the console for Micro
    if (boardType == 1)
    {
        testAccelSensor();
        testMagSensor();
    }
    
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
        mySerial.println(String(key));
    }
	
	// Read input on Micro and set transmit mode
	char input;
    if (boardType == 1 && mySerial.available())
    {
        // Read first byte of data (expecting first character is a number)
		input = mySerial.read();
        
		// Ignore any other data by looping until there is no more data being transmitted
        while (mySerial.available())
        {
            mySerial.read();
            delay(1);
        }
        
        incomingData = true;

		// Set behavior on Micro
        switch (input)
        {
            case '0':
                sendDataType = NONE;
                Serial.println("Send data:  NONE");
                break;
            case '1':
                sendDataType = ACCEL;
                Serial.println("Send data:  ACCEL ONLY");
                break;
            case '2':
                sendDataType = MAG;
                Serial.println("Send data:  MAG ONLY");
                break;
            case '3':
                sendDataType = BOTH;
                Serial.println("Send data:  ACCEL AND MAG");
                break;
        }
    }

	// Transmit ACK from Micro after data was received
	if (boardType == 1 && incomingData)
    {
        mySerial.println("ACK!");
        incomingData = false;
    }
	
	// On Mega, receive ACK or sensor data
    if (boardType == 2 && mySerial.available())
    {
        while (mySerial.available())
        {
            Serial.write(mySerial.read());
            delay(1);
        }
    }

	// Transmit accel sensor data from Micro
    if (boardType == 1 && (sendDataType == ACCEL || sendDataType == BOTH))
    {
        getAccelSensorData();
    }

	// Transmit compass sensor data from Micro
    if (boardType == 1 && (sendDataType == MAG || sendDataType == BOTH))
    {
        getMagSensorData();
    }
}

// Test Accel Sensor
void testAccelSensor()
{
    Serial.println("Accelerometer Test"); Serial.println("");
    
    // Initialise the sensor
    if(!accel.begin())
    {
        // There was a problem detecting the ADXL345 ... check your connections
        Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
        while(1);
    }

    // Display some basic information on this sensor
    displayAccelSensorDetails();
}

// Test Compass Sensor
void testMagSensor()
{
    Serial.println("Magnetometer Test"); Serial.println("");

    // Enable auto-gain
    mag.enableAutoRange(true);

    // Initialise the sensor
    if(!mag.begin())
    {
        // There was a problem detecting the LSM303 ... check your connections
        Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
        while(1);
    }
    
    // Display some basic information on this sensor
    displayMagSensorDetails();
}

// Display Accel Sensor Details
void displayAccelSensorDetails(void)
{
    sensor_t sensor;
    accel.getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.print  ("Sensor:       "); Serial.println(sensor.name);
    Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
    Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
    Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
    Serial.println("------------------------------------");
    Serial.println("");
    delay(500);
}

// Display Mag Sensor Details
void displayMagSensorDetails(void)
{
    sensor_t sensor;
    mag.getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.print  ("Sensor:       "); Serial.println(sensor.name);
    Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
    Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
    Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");
    Serial.println("------------------------------------");
    Serial.println("");
    delay(500);
}

// Get Accel Sensor Data
void getAccelSensorData()
{
    // Get a new sensor event
    sensors_event_t event;
    accel.getEvent(&event);
    
    // Display the results (acceleration is measured in m/s^2) over the serial connection
    mySerial.print("X: "); mySerial.print(event.acceleration.x); mySerial.print("  ");
    mySerial.print("Y: "); mySerial.print(event.acceleration.y); mySerial.print("  ");
    mySerial.print("Z: "); mySerial.print(event.acceleration.z); mySerial.print("  ");mySerial.println("m/s^2 ");
    
    // Display the results (acceleration is measured in m/s^2) on the console
    Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
    Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
    Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
    
    // Delay before the next sample
    delay(500);
}

// Get Mag Sensor Data
void getMagSensorData()
{
    // Get a new sensor event
    sensors_event_t event;
    mag.getEvent(&event);
    
    // Display the results (magnetic vector values are in micro-Tesla (uT)) over the serial connection
    mySerial.print("X: "); mySerial.print(event.magnetic.x); mySerial.print("  ");
    mySerial.print("Y: "); mySerial.print(event.magnetic.y); mySerial.print("  ");
    mySerial.print("Z: "); mySerial.print(event.magnetic.z); mySerial.print("  ");mySerial.println("uT");
    
    // Display the results (magnetic vector values are in micro-Tesla (uT)) on the console
    Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
    Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
    Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");
    
    // Delay before the next sample
    delay(500);
}
