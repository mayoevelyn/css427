1. Difficult (60 min): Implement the wired communication between two Arduino devices.

   NOTICE: You ONLY NEED ONE PROGRAM for Arduino Mega and Arduino Micro.

   You just need to select the correct board, CPU, and port when compiling and downloading the program.

a. You need to first connect the Arduino Mega and Micro devices.

b. Following the SoftwareSerial example to pick two ports on each Arduino device as the UART port.

c. Connect the keypad you used in Lab 3 into the Mega device

d. Connect both Mega and Micro to the PC via USB

e. Press the key on the keypad that is connected the Mega device, and let the Mega sends the captured

   key value to the Micro (you may add extra information besides the key value), then the Micro should

   send (forward) the information received form Mega to PC so as to be printed in SerialMonitor.

f. Once you have implemented the previous step, as an acknowledgement method

   to the Micro device: after the Micro sends data to PC, it should also send back

   an ACK to the Mega, so that Mega actually knows that the message is received

   by the Micro. The Mega device should also print out the received ACK to the

   host PC. After this step, you should see information about the pressed key, and

   the ACK information in two separated SerialMonitor windows.

   NOTICE: how to open two SerialMonitor?

i. Open two Arduino IDEs from the start menu. It’s not opening two sketches, but two IDEs.

ii. Open one Arduino IDE, and open one serial terminal (e.g., Putty)