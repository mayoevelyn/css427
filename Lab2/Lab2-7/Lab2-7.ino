//TMP36 Pin Variables
int sensorPin = 0; //the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade with a
                        //500 mV offset to allow for negative temperatures

int mode;

void setup()
{
  Serial.begin(9600);  //Start the serial connection with the computer
                       //to view the result open the serial monitor 

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  mode = 0;
}
 
void loop()                     // run over and over again
{
  if (Serial.available() > 0) 
  {
        // read the incoming byte:
        int input = Serial.readString().toInt();        
               
        Serial.println("Reference voltage mode: " + String(input));

        if (!(input == 0 || input == 2 || input == 3))
        {
          Serial.println("bad mode");
        }
        else
        {
          mode = input;
        }
  } 

   readTemperature();
   delay(1000);                                     
}

void readTemperature()
{
   switch(mode)
   {
     case 0:
       analogReference(DEFAULT);
       break;
     case 2:
      analogReference(INTERNAL1V1);
      break;
     case 3:
      analogReference(INTERNAL2V56);
      break;
   }
   
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
}


