const int VAL_PROBE = 0; // Analog pin 0

void setup()
{
    Serial.begin(9600);
}

void loop() {
    int moisture = analogRead(VAL_PROBE);
    Serial.println(moisture);

    delay(1000);
}

