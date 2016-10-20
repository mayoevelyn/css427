/* Example code
  const byte rows = 4; //four rows
  const byte cols = 3; //three columns
  char keys[rows][cols] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
  };
  byte rowPins[rows] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
  byte colPins[cols] = {8, 7, 6}; //connect to the column pinouts of the keypad
  Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );
  */

#include <Key.h>
#include <Keypad.h>

void setup()
{

}

void loop()
{

}
