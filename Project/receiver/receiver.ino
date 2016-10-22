char input;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(53, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {
    input = Serial.read();

    if (input == '0')
    {
      digitalWrite(53, LOW);
    }
    if (input == '1')
    {
      digitalWrite(53, HIGH);
    }
  }
}
