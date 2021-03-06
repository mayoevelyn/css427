// 3. Ninja Sensei (unlimited time until due)
// a. Change the UART connection between the Mega and Micro devices, using I2C instead.
// b. All other functions should be the same as in exercise 2.
// c. This basically means: 1) The Mega device will communicate with the Micro
//    device via I2C back and forth; 2) The Micro device will communicate with the
//    Mega device via I2C back and forth, plus, the Micro device will also need to
//    collect sensor data from another I2C interface.

#include <Key.h>
#include <Keypad.h>
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
int microAddress = 0x10;
int megaAddress = 0x20;

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
    incomingData = false;

	// Display sensor test data to the console for Micro
    if (boardType == 1)
    {
        testAccelSensor();
        testMagSensor();
    }

	// Setup L2C connections
    if (boardType == 1)
    {
        Wire.begin(microAddress);
    }
    if (boardType == 2)
    {
        Wire.begin(megaAddress);
    }
    Wire.onReceive(receiveEvent); // register event
}

// Loop
void loop()
{
    // Save any single input from the keypad
	char key = keypad.getKey();
    
	// Transmit key from Mega
    if (boardType == 2 && key != NO_KEY)
    {
        Wire.beginTransmission(microAddress);
        Wire.write(String(key).toInt());
        Wire.endTransmission();
    }

	// Transmit ACK from Micro after data was received
    if (boardType == 1 && incomingData)
    {
        Wire.beginTransmission(megaAddress);
        Wire.write("ACK!\n");
        Wire.endTransmission();
        incomingData = false;
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

    delay(100);
}

// Receive Event: function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
    // Set behavior on Micro
	if (boardType == 1)
    {
        int input = Wire.read();    // receive byte as an integer
        Serial.println(input);      // print the integer
        switch (input)
        {
            case 0:
                sendDataType = NONE;
                Serial.println("Send data:  NONE");
                break;
            case 1:
                sendDataType = ACCEL;
                Serial.println("Send data:  ACCEL ONLY");
                break;
            case 2:
                sendDataType = MAG;
                Serial.println("Send data:  MAG ONLY");
                break;
            case 3:
                sendDataType = BOTH;
                Serial.println("Send data:  ACCEL AND MAG");
                break;
        }

        incomingData = true;
    }

	// On Mega, receive ACK or sensor data
    if (boardType == 2)
    {
        while (Wire.available())
        {
            char c = Wire.read(); // receive byte as a character
            Serial.print(c);      // print the character
        }
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

// Test Mag Sensor
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
    
	// Format float into character array for transmittion
	char accelX[15];
    char accelY[15];
    char accelZ[15];
    dtostrf(event.acceleration.x, 7, 3, accelX);
    dtostrf(event.acceleration.y, 7, 3, accelY);
    dtostrf(event.acceleration.z, 7, 3, accelZ);
    
	// Transmit the results (acceleration is measured in m/s^2) over L2C
    Wire.beginTransmission(megaAddress);
    Wire.write("X: "); Wire.write(accelX); Wire.write("  ");
    Wire.endTransmission();
    Wire.beginTransmission(megaAddress);
    Wire.write("Y: "); Wire.write(accelY); Wire.write("  ");
    Wire.endTransmission();
    Wire.beginTransmission(megaAddress);
    Wire.write("Z: "); Wire.write(accelZ); Wire.write("  "); Wire.write("m/s^2\n");
    Wire.endTransmission();
    
    // Display the results (acceleration is measured in m/s^2) on the console
    Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
    Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
    Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  "); Serial.println("m/s^2");
    
    // Delay before the next sample
    delay(500);
}

// Get Mag Sensor Data
void getMagSensorData()
{
    // Get a new sensor event
    sensors_event_t event;
    mag.getEvent(&event);
	
	// Format float into character array for transmittion
    char magX[15];
    char magY[15];
    char magZ[15];
    dtostrf(event.magnetic.x, 7, 3, magX);
    dtostrf(event.magnetic.y, 7, 3, magY);
    dtostrf(event.magnetic.z, 7, 3, magZ);
    
	// Display the results (magnetic vector values are in micro-Tesla (uT)) over L2C
    Wire.beginTransmission(megaAddress);
    Wire.write("X: "); Wire.write(magX); Wire.write("  ");
    Wire.endTransmission();
    Wire.beginTransmission(megaAddress);
    Wire.write("Y: "); Wire.write(magY); Wire.write("  ");
    Wire.endTransmission();
    Wire.beginTransmission(megaAddress);
    Wire.write("Z: "); Wire.write(magZ); Wire.write("  ");Wire.write("uT\n");
    Wire.endTransmission();
    
    // Display the results (magnetic vector values are in micro-Tesla (uT)) on the console
    Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
    Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
    Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");
    
    // Delay before the next sample
    delay(500);
}
