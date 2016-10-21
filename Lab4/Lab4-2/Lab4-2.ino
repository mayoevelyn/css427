#include <Key.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

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

// Assign a unique ID to this sensor at the same time
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

// Assign a unique ID to this sensor at the same time
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

// Setup
void setup()
{
#ifndef ESP8266
    while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
    Serial.begin(9600);
    delay(1000);
    Serial.println("Booting " + boardName);

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

    // set the data rate for the SoftwareSerial port
    mySerial.begin(4800);
    incomingData = false;
}

// Loop
void loop()
{
    //getAccelSensorData();
    getMagSensorData();
    
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

void getAccelSensorData()
{
    /* Get a new sensor event */
    sensors_event_t event;
    accel.getEvent(&event);
    
    /* Display the results (acceleration is measured in m/s^2) */
    Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
    Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
    Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
    
    /* Note: You can also get the raw (non unified values) for */
    /* the last data sample as follows. The .getEvent call populates */
    /* the raw values used below. */
    //Serial.print("X Raw: "); Serial.print(accel.raw.x); Serial.print("  ");
    //Serial.print("Y Raw: "); Serial.print(accel.raw.y); Serial.print("  ");
    //Serial.print("Z Raw: "); Serial.print(accel.raw.z); Serial.println("");
    
    /* Delay before the next sample */
    delay(500);
}

void getMagSensorData()
{
    /* Get a new sensor event */
    sensors_event_t event;
    mag.getEvent(&event);
    
    /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
    Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
    Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
    Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");
    
    /* Note: You can also get the raw (non unified values) for */
    /* the last data sample as follows. The .getEvent call populates */
    /* the raw values used below. */
    // Serial.print("X Raw: "); Serial.print(mag.raw.x); Serial.print("  ");
    // Serial.print("Y Raw: "); Serial.print(mag.raw.y); Serial.print("  ");
    // Serial.print("Z Raw: "); Serial.print(mag.raw.z); Serial.println("");
    
    /* Delay before the next sample */
    delay(500);
}

