# Arduino_USB_PPM
Convert a USB joystick input into a PPM signal for a RC transmitter using an Arduino + USB Host Shield.

This repository contains:
- [USB_PPM_Arduino.ino](USB_PPM_Arduino.ino) — main Arduino sketch
- [LICENSE](LICENSE) — GNU GPL v3
- [.gitignore](.gitignore)

**TODO: Finish USB to PPM code**

## Features
- Generates multi-channel PPM signal for FLYSKY I6X and other common transmitters.
- Configurable PPM channel count, frame length and pulse settings in the sketch.
- Simple demo mode that animates a channel if no USB joystick is present.
- Example to test PPM Generation 
- Example to test Joystick HID data retrieval

## Quick start

Requirements:
- Arduino (Uno was used)
- USB Host Shield (see USB Host Shield 2.0) 
- USB joystick/gamepad
- USB Host Shield library: https://github.com/felis/USB_Host_Shield_2.0

Basic Wiring:
- PPM output: Connect the PPM signal to the pin defined as `PPM_PIN` in the header file [`PPM_TxConfig.h`](TEST_PPM_Arduino/PPM_TxConfig.h).
- USB HOST: Configure the USB HOST 2.0 jumper traces according to the desired voltages and Pin availability referring to the [USB Host 2.0 Hardware Manual](https://chome.nerpa.tech/usb-host-shield-hardware-manual/). 

Configuration (in [PPM_TxConfig.h](TEST_PPM_Arduino/PPM_TxConfig.h))
- TX Axis mapping modes: define one of the modes at top of the sketch (`MODE_1`, `MODE_2`, `MODE_3`, `MODE_4`) — this controls how transmitter axes map to joystick axes. The current default is `MODE_3`.
![Operation modes of a RC Transmitter](https://www.rcgroups.com/forums/showatt.php?attachmentid=714749)
- PPM parameters are set according to the selected PPM Configuration. The current default is [`FLYSKY_I6X`](TEST_PPM_Arduino/PPM_TxConfig.h):
  - `CHANNEL_NUMBER` — number of channels
  - `CHANNEL_DEFAULT_VALUE` — default servo mid value (µs)
  - `THROTTLE_DEFAULT_VALUE` — default throttle (µs)
  - `PPM_FRAME_LEN` — frame length (µs)
  - `PPM_PULSE_LEN` — separation pulse length (µs)
  - `PPM_PULSE_POLARITY` — pulse polarity (0 = low pulse, 1 = high pulse)

Notes about code
- The sketch uses Timer1 and an interrupt routine (`ISR(TIMER1_COMPA_vect)` in [USB_PPM_Arduino.ino](TEST_PPM_Arduino/PPM_TxConfig.cpp)) to generate precise PPM timing.
- USB handling is prepared via the USB Host Shield library; (see https://github.com/felis/USB_Host_Shield_2.0)

Build & upload
1. Install Arduino IDE (or PlatformIO).
2. Install the USB Host Shield 2.0 library.
3. Open "USB_PPM_Arduino.ino" in the IDE.
4. Select board and port, then Upload.

Usage
- Select the desired `*.ino` file:
    - [PPM Generation test example](TEST_PPM_Arduino/TEST_PPM_Arduino.ino)
    - [USB Host read Joystick test example](TEST_USB_Arduino/TEST_USB_Arduino.ino)
    - [Code to generate PPM signals based on Joystick](Arduino_USB_PPM.ino)
- Compile and upload the code.
- Check on serial port everython is working correctly.
- Use transmitter/receiver to verify PPM channel outputs.

Troubleshooting
- If PPM output is not present, confirm:
  - The PPM pin matches [`PPM_PIN`](USB_PPM_Arduino.ino).
  - Timer1 is not used by other libraries.
  - The USB Host Shield library is initialized successfully (check Serial output).
- If timings are off, check the constants: [`PPM_FRAME_LEN`](USB_PPM_Arduino.ino) and [`PPM_PULSE_LEN`](USB_PPM_Arduino.ino).

References
- PPM Generation:
  - https://code.google.com/p/generate-ppm-signal/ 
- USB Host to PPM examples used as base:
  - https://github.com/DroneMesh/USB_PPM
  - https://github.com/drorgl/USBJoystickPPM
  - https://github.com/lexfp/le3dpToPPM
- PPM input reference:
  - https://github.com/joshbuker/mcu-ppm-input
- Flysky i6x timings used:
  - https://github.com/aemarkov/PPMControl/blob/master/AudioPPM/StandartProfiles.cs
- TX axis operation modes:
  - https://www.rcgroups.com/forums/showthread.php?1122256-whats-difference-between-mode-1-and-mode-2
- ATmega328 Microcontroller Datasheet:
  - https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
- USB Host Shield 2.0 Library filled with references and examples:
  - https://github.com/felis/USB_Host_Shield_2.0
- Oleg Mazurov's website filled with lots of helpful information:
  - https://chome.nerpa.tech/usb-host-shield-hardware-manual/
  - https://chome.nerpa.tech/mcu/using-logitech-extreme-3d-pro-joystick-with-arduino-hid-library/
  
License
- This project is distributed under the terms of the GNU General Public License v3. See [LICENSE](LICENSE).

