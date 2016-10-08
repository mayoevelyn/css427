// 9. Difficult (60min): Report photocell sensor and temperature sensor value
// together in one message. In each message, the following information should
// be included for each sensor: PIN, sampling rate, value, and threshold. The
// message should be sent to the host PC periodically or upon receiving a
// user’s request (on demand). The periodical report interval should be
// configurable by user’s input.

// global variables
int thresholdTemp;
int pollingRateInMs;
bool isPolling;

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
  thresholdTemp = 0;
  pollingRateInMs = 0;
  isPolling = false;

  // display instructions
  displayMenu();
}

// run over and over again
void loop()
{
  // get input from console
  serialInputHandler();

  
}

// display interaction menu
void displayMenu()
{
  Serial.println("Press 0 to get sensor readings");
  Serial.println("Press 1 to set a polling rate");
  Serial.println("Press 2 to set a temperature threshold");
  Serial.print("Menu: ");
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
      case 2:
        setThresholdTemp();
        break;
      default:
        displayMenu();
    }
  } 
}

void displaySensorReadings()
{
  
}

// serialInputHandler helper function
void setPollingRate()
{
  if (Serial.available() > 0) 
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
    
    if (pollingRateInMs != 0)
    {
      isPolling = true;
    }
  }
}

// serialInputHandler helper function
void setThresholdTemp()
{
  if (Serial.available() > 0) 
  {
    // stop polling while waiting for input
    bool wasPolling = isPolling;
    if (isPolling)
    {
      isPolling = false;
    }

    // display instructions
    Serial.print("Set temperature threshold in degrees F: ");
    
    // read the incoming byte:
    int input = Serial.readString().toInt();

    Serial.println(String(input));
    Serial.println();

    thresholdTemp = input;

    // resume polling
    if (wasPolling)
    {
      isPolling = true;
    }
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
