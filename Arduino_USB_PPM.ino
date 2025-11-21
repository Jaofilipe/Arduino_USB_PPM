#include "ARDUINO_USB_PPM.h"

USB Usb;

ThrustMasterPPM thrust_joy( ppm, &Usb);

void setup() {

  Serial.begin(115200);                             //initialize serial communication

#if !defined(__MIPSEL__)
  while (!Serial);                                  // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif

Serial.println(F("\r\nThrustMaster T.16000M FCS Joystick Example Started"));

if (Usb.Init() == -1) {
  Serial.print(F("\r\n Check USB Board, Init Failed"));
  while (1); // Halt
}

  setup_PPM();
  Serial.print(F("\r\n PPM Started \n"));
    
}

void loop() {

  Usb.Task();

  if (!thrust_joy.connected()) {
    Serial.println("No Connection to Joystick");  //Joystick not connected, wait for connection
    delay(100);
  }

}
