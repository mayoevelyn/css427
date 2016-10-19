// 2. Easy (5 min): Whenever a key is pressed, print the key value to the host PC through serial communication.

#include <Key.h>
#include <Keypad.h>

  const byte rows = 4; //four rows
  const byte cols = 3; //three columns
  char keys[rows][cols] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
  };
  byte rowPins[rows] = {18, 17, 16, 15}; //connect to the row pinouts of the keypad
  byte colPins[cols] = {21, 20, 19}; //connect to the column pinouts of the keypad
  Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

  void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
   char key = keypad.getKey();

   if (key != NO_KEY){
     Serial.println(key);
   }
}

