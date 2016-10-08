// 6. Medium (15 min): Change the onboard LED’s brightness based on
// temperature: the higher temperature is, the brighter the LED is.

//TMP36 Pin Variables
int sensorPin = 0; //the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade with a
                        //500 mV offset to allow for negative temperatures
 
const int minBright = 0;
const int maxBright = 255;
const int minTemp = 75;
const int maxTemp = 80;

void setup()
{
  Serial.begin(9600);  //Start the serial connection with the computer
                       //to view the result open the serial monitor 

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}
 
void loop()                     // run over and over again
{
  int range = maxTemp - minTemp;
  int temperature = (int)readTemperature();
  int brightness = (maxBright / range) * (temperature - minTemp);
  if (brightness < 0)
  {
    brightness = 0;
  }
  else if (brightness > 255)
  {
    brightness = 255;
  }
  
  analogWrite(LED_BUILTIN, brightness);   // enable the LED at varying brightness
  Serial.println("brightness: " + String(brightness));
 
  delay(1000);                                     //waiting a second
}

float readTemperature()
{
  //getting the voltage reading from the temperature sensor
   int reading = analogRead(sensorPin);  
   
   // converting that reading to voltage, for 3.3v arduino use 3.3
   float voltage = reading * 5.0;
   voltage /= 1024.0; 
   
   float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                                 //to degrees ((voltage - 500mV) times 100)
   
   // now convert to Fahrenheit
   float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
   Serial.print(temperatureF); Serial.println(" degrees F");
   return temperatureF;
}

// 

