// 9. Difficult (60min): Report photocell sensor and temperature sensor value
// together in one message. In each message, the following information should
// be included for each sensor: PIN, sampling rate, value, and threshold. The
// message should be sent to the host PC periodically or upon receiving a
// user’s request (on demand). The periodical report interval should be
// configurable by user’s input.

#include "SimpleTimer.h"  // http://playground.arduino.cc/Code/SimpleTimer

// global variables
const int thresholdBrightness = 150;
const int thresholdTemp = 70;

int pollingRateInMs;
bool isPolling;
SimpleTimer timer;
int timerId;

// assign analog pins
enum
{
  sensorPhotoCell = 0,
  sensorTemperature = 1
};

// initialize parameters
void setup()
{
  Serial.begin(9600);  //Start the serial connection with the computer
                       //to view the result open the serial monitor 

  // initialize globals
  pollingRateInMs = 0;
  isPolling = false;
  timerId = -1;

  // display instructions
  displayMenu();
}

// run over and over again
void loop()
{
  // get input from console
  serialInputHandler();

  if (isPolling)
  {
    timer.run();
  }
}

// display interaction menu
void displayMenu()
{
  Serial.println("---Menu---");
  Serial.println("Press 0 to get sensor readings");
  Serial.println("Press 1 to set a polling rate");
  Serial.print("Command: ");
}

// take input from the console
void serialInputHandler()
{
  if (Serial.available() > 0) 
  {
    // read the incoming byte:
    int input = Serial.readString().toInt();        

    Serial.println(String(input));
    Serial.println();
    
    switch(input)
    {
      case 0:
        displaySensorReadings();
        break;
      case 1:
        setPollingRate();
        break;
    }

    displayMenu();
  } 
}

// serialInputHandler helper function
void displaySensorReadings()
{
  int photocell = readPhotocell();
  float temperature = readTemperature();

  // display information for photocell
  Serial.print(
    "Value of photocell on pin " + String(sensorPhotoCell) +
    " using a " + String(pollingRateInMs) + "ms sampling rate: " + String(photocell)
  );
  if (photocell > thresholdBrightness)
  {
    Serial.print(" (above ");
  }
  else
  {
    Serial.print(" (below ");
  }
  Serial.println(String(thresholdBrightness) + " threshold)");

  //display information for temperature
  Serial.print(
    "Value of temperature on pin " + String(sensorTemperature) +
    " using a " + String(pollingRateInMs) + "ms sampling rate: " + String(temperature)
  );
  if (temperature > thresholdTemp)
  {
    Serial.print(" (above ");
  }
  else
  {
    Serial.print(" (below ");
  }
  Serial.println(String(thresholdTemp) + " threshold)");

  Serial.println();
}

// serialInputHandler helper function
void setPollingRate()
{
  // stop polling while waiting for input
  isPolling = false;

  // display instructions
  Serial.print("Set polling rate in milliseconds (0 to stop): ");
  
  // read the incoming byte:
  int input = Serial.readString().toInt();

  Serial.println(String(input));
  Serial.println();

  pollingRateInMs = input;
  
  if (pollingRateInMs == 0 && timerId != -1)
  {
    isPolling = false;
    timer.deleteTimer(timerId);
  }
  else
  {
    isPolling = true;
    timerId = timer.setInterval(pollingRateInMs, displaySensorReadings);
  }
}

// displaySensorReadings helper function
int readPhotocell()
{
  return analogRead(sensorPhotoCell);
}

// displaySensorReadings helper function
float readTemperature()
{
  //getting the voltage reading from the temperature sensor
  int reading = analogRead(sensorTemperature);
  
  // converting that reading to voltage, for 3.3v arduino use 3.3
  float voltage = reading * 5.0;
  voltage /= 1024.0; 

  // calculate temperature
  float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree with 500 mV offset
                                                //to degrees ((voltage - 500mV) times 100)
  
  // now convert to Fahrenheit
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
  return temperatureF;
}
