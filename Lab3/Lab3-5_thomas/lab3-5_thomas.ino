#include <Key.h>
#include <Keypad.h>

void setup() {
  const byte rows = 4; //four rows
  const byte cols = 3; //three columns
  char keys[rows][cols] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
  };
  byte rowPins[rows] = {17, 16, 15, 14}; //connect to the row pinouts of the keypad
  byte colPins[cols] = {20, 19, 18}; //connect to the column pinouts of the keypad
  Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

  void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  char key = keypad.getKey();

  if (key != NO_KEY){
   Serial.println(key);
  }
}
