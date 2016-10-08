// 4. Medium (15 min): Enable or disable temperature sensor based on user’s
// input from serial communication, report “Disabled” to the host when the
// sensor is disabled, report sensor readings to the host when the sensor is
// enabled.

//TMP36 Pin Variables
int sensorPin = 0; //the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade with a
                        //500 mV offset to allow for negative temperatures
bool isReadTempOn; 

void setup()
{
  Serial.begin(9600);  //Start the serial connection with the computer
                       //to view the result open the serial monitor 
  isReadTempOn = false;
}
 
void loop()                     // run over and over again
{
  if (Serial.available() > 0) 
  {
        // read the incoming byte:
        int input = Serial.readString().toInt();

        if (input == 1)
        {
          Serial.println("Enabled");
          isReadTempOn = true;
        }
        else if (input == 0)
        {
          Serial.println("Disabled");
          isReadTempOn = false;
        }
  }

  if (isReadTempOn)
  {
    readTemperature();
  }

  delay(1000);                                     //waiting a second
}

void readTemperature()
{
  //getting the voltage reading from the temperature sensor
   int reading = analogRead(sensorPin);  
   
   // converting that reading to voltage, for 3.3v arduino use 3.3
   float voltage = reading * 5.0;
   voltage /= 1024.0; 
   
   // print out the voltage
   Serial.print(voltage); Serial.println(" volts");
   
   // now print out the temperature
   float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                                 //to degrees ((voltage - 500mV) times 100)
   Serial.print(temperatureC); Serial.println(" degrees C");
   
   // now convert to Fahrenheit
   float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
   Serial.print(temperatureF); Serial.println(" degrees F");
}

