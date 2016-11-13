#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// light sensor
// connect I2C wires to SDA and SCL only.
const int lightSensorAddr = 0x23; // i2c address
byte lightSensorBuff[2];

// moisture sensor
const int moisturePin = 64; // A10 on Mega2560

// temp and humidity sensor
#define DHTPIN 50 // digital pin for data out
#define DHTTYPE DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);

void setup()
{
    Serial.begin(9600);

    // initialize light sensor
    Wire.begin();
    lightSensorInit(lightSensorAddr);
    
    // initialize temp and humidity sensor
    dht.begin();
}

void loop()
{
    getLightSensorReading();
    getMoistureReading();
    getTempHumidityReading();

    Serial.println();
    delay(2000);
}

// Light Sensor Initialization
void lightSensorInit(int address)
{  
    Wire.beginTransmission(address);
    Wire.write(0x10); // 1 [lux]
    Wire.endTransmission();
}

// Get Light Sensor Reading (Wrapper)
void getLightSensorReading()
{
    float value = 0;
    
    if (lightSensorRead(lightSensorAddr) == 2)
    {
        value = ((lightSensorBuff[0] << 8) | lightSensorBuff[1]) / 1.2;

        Serial.print("Light: ");
        
        if (value < 0)
        {
            Serial.println("> 65535");
        }
        else
        {
            Serial.println((int)value, DEC); 
        }
    }
}

// Light Sensor Read
byte lightSensorRead(int address)
{  
    byte i = 0;
    Wire.beginTransmission(address);
    Wire.requestFrom(address, 2);
    
    while(Wire.available())
    {
        lightSensorBuff[i] = Wire.read(); 
        i++;
    }

    Wire.endTransmission();  
    return i;
}

void getMoistureReading()
{
    int moisture = analogRead(moisturePin);
    Serial.print("Moisture: ");
    Serial.println(moisture);
}

void getTempHumidityReading()
{
    // Get temperature event and print its value.
    sensors_event_t event;  
    dht.temperature().getEvent(&event);
    
    if (isnan(event.temperature))
    {
        Serial.println("Error reading temperature!");
    }
    else
    {
        Serial.print("Temperature: ");
        Serial.println(event.temperature);
    }
    
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity))
    {
        Serial.println("Error reading humidity!");
    }
    else
    {
        Serial.print("Humidity: ");
        Serial.println(event.relative_humidity);
    }
}

