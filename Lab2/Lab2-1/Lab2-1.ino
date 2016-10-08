// 1. Easy (15 min): Connect the photocell sensor to an Analog Pin, read the
// sensor data and report to host PC through serial communication. Be aware
// that you need to choose a proper resistor when connecting the photocell
// sensor.

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // print out the value you read:
  Serial.println(sensorValue);
  delay(100);        // delay in between reads for stability
}
