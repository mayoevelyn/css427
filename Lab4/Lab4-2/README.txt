2. Ninja (90min): Connect the sensor board to the Arduino Micro device over I2C interface.

a. Sensor board information: https://www.adafruit.com/products/1120. You can

   also find example code there.

b. You may download the code provided by Adafruit, but you need to change it by

   splitting the read function to two functions: one is to read accelerometer only,

   and the other one is to read magnetometer only.

c. Changing the code that you have wrote in the first exercise. Once key “1” is

   pressed on the Mega device, the accelerometer information should be collected

   on the Micro device and sent to PC. The Micro also ACKs the Mega device for

   the received command information. Once key “2” is pressed, the magnetometer

   information should be collected on the Micro device and sent to PC. The Micro

   also ACKs the Mega device for the received command information. Once key “3”

   is pressed, information about both accelerometer and magnetometer

   information should be collected on the Micro device and sent to PC, and the

   Micro should ACK the command. You need to show the data printed at two

   SerialMonitor windows.