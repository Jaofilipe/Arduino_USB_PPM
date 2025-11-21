/*
  Example sketch for the SteelSeries SRW-S1 Steering Wheel - developed by Kristian Lauszus
  For more information visit my blog: http://blog.tkjelectronics.dk/ or
  send me an e-mail:  kristianl@tkjelectronics.com
*/

#include <SPI.h>
#include "THRUSTMASTER_FCS.h"

USB Usb;
THRUSTMASTER_FCS thrust_joy(&Usb);

bool connected;

void setup() {
  
  Serial.begin(115200);

#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif

if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  
  Serial.println(F("\r\nThrustMaster T.16000M FCS Joystick Example Started"));

}

void loop() {

  Usb.Task();

  if (thrust_joy.connected()) {
      if (thrust_joy.TmJoyData.Xaxis) {
      Serial.print("X Axis: ");
      Serial.println(thrust_joy.TmJoyData.Xaxis, HEX);
    }
          if (thrust_joy.TmJoyData.Yaxis) {
      Serial.print("Y Axis: ");
      Serial.println(thrust_joy.TmJoyData.Yaxis, HEX);
    }
          if (thrust_joy.TmJoyData.RZaxis) {
      Serial.print("RZ Axis: ");
      Serial.println(thrust_joy.TmJoyData.RZaxis, HEX);
    }
              if (thrust_joy.TmJoyData.Slider) {
      Serial.print("Slider: ");
      Serial.println(thrust_joy.TmJoyData.Slider, HEX);
    }
    /*
    if (thrust_joy.buttonClickState.speedLimiter) {
      thrust_joy.buttonClickState.speedLimiter = 0; // Clear event
      Serial.println(F("Speed Limiter"));
    }
    */

    switch (thrust_joy.TmJoyData.Buttons.dpad) {
      case DPAD_UP:
        Serial.println(F("Up"));
        break;
      case DPAD_UP_RIGHT:
        Serial.println(F("UP & right"));
        break;
      case DPAD_RIGHT:
        Serial.println(F("Right"));
        break;
      case DPAD_RIGHT_DOWN:
        Serial.println(F("Right & down"));
        break;
      case DPAD_DOWN:
        Serial.println(F("Down"));
        break;
      case DPAD_DOWN_LEFT:
        Serial.println(F("Down & left"));
        break;
      case DPAD_LEFT:
        Serial.println(F("Left"));
        break;
      case DPAD_LEFT_UP:
        Serial.println(F("Left & up"));
        break;
      case DPAD_OFF:
        break;
      default:
        Serial.print(F("Unknown state: "));
        Serial.println(thrust_joy.TmJoyData.Buttons.dpad, HEX);
        break;
    }
  } else {
    Serial.println("No Connection to Joystick");  //Joystick not connected, wait for connection
    delay(100);
  }
  
}
