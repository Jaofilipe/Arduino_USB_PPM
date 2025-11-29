# Arduino USB to PPM with Display
Convert a USB joystick input into a PPM signal for a RC transmitter using an Arduino + USB Host Shield.
Display the current PPM values and Joystick Axis&Button status in a LCD 20×4 display via I2C.

This repository contains:
- [USB_PPM_Arduino.ino](USB_PPM_Arduino.ino) — main Arduino sketch that bridges USB HID to PPM and shows data on Display
- [TEST_PPM_Arduino](TEST_PPM_Arduino/TEST_PPM_Arduino.ino) — PPM generation test example and PPM configuration headers
- [TEST_USB_Arduino](TEST_USB_Arduino/TEST_USB_Arduino.ino) — USB host joystick test example
- [Media_Folder](/Media/) — Folder containing some useful resources 
- [LICENSE](LICENSE) — GNU GPL v3
- [.gitignore](.gitignore)

# Features
- Generates multi-channel PPM signal for FLYSKY I6X and common transmitters (basic default PPM provided).
- Configurable PPM channel count, frame length and pulse settings via header file (see [TEST_PPM_Arduino/PPM_TxConfig.h](TEST_PPM_Arduino/PPM_TxConfig.h))
- Example to test PPM Generation (with demo mode that moves a channel)
- Example to test Joystick HID data reading
- Shows the current Joystick values as well as the PPM values transmitted on a Display.

## Quick start

>[!NOTE]
>**Requirements:**  
>- Arduino (Uno / Nano / ATmega328P tested)  
>- USB Host Shield (see USB Host Shield 2.0)   
>- USB joystick/gamepad  
>- USB Host Shield library: https://github.com/felis/USB_Host_Shield_2.0
>- Liquid Display LCD 20x4 with I2C backpack (Can be deemed optional)  

## Wiring:

> [!IMPORTANT]
> **Before Assembling the Circuit & Uploading**
> Please verify the pins used for PPM and USB HOST 2.0 Board before uploading!  
> Timer1 direct output pins are [`Pin 9`] and [`Pin 10`]  
> These pins are used by the USB HOST 2.0 are [`INT`] ([`Pin 9`]) and [`SS`] ([`Pin 10`])   
> With this in mind the code uses an alternative Pin for [`PPM_PIN`], [`Pin 7`]
> With this in mind, there are 2 possible options:  
> There are 2 possible options:  
> - For usage of [`PPM_PIN`] not directly tied to hardware Timer1 (ok time accuracy):
>   - Change the [`PPM_PIN`] in the header file [`PPM_TxConfig.h`](TEST_PPM_Arduino/PPM_TxConfig.h).  
>   - If you did not modify the [USB_PPM_Arduino.ino](USB_PPM_Arduino.ino) code, then the [`Pin 7`] was already selected, no further work needed.
>   
> - For usage of Direct Timer1 pin (best time accuracy):          
>   - Change the USB HOST 2.0 pins used for [`SS`] and [`INT`] as needed and route new wires accordingly.  
>   - Read Section 5 of [`USB HOST SHIELD Hardware Manual`](https://chome.nerpa.tech/usb-host-shield-hardware-manual/)  
>   - Variables to be changed are in [`UsbCore.h`] file of [`USB HOST SHIELD 2.0 Repo`](https://github.com/felis/USB_Host_Shield_2.0/blob/master/UsbCore.h)  

>[!NOTE]
>- PPM output: Connect the PPM signal to the pin defined as `PPM_PIN` in the header file [`PPM_TxConfig.h`](TEST_PPM_Arduino/PPM_TxConfig.h).  
>- USB HOST: Configure the USB HOST 2.0 jumper traces according to the desired voltages and Pin availability referring to the [USB Host 2.0 Hardware Manual](https://chome.nerpa.tech/usb-host-shield-hardware-manual/).  
>- I2C LCD: Connect the I2C lines to the I2C SDA and SCL ports of the microcontroller accordingly.  



## Configuration in [PPM_TxConfig.h](TEST_PPM_Arduino/PPM_TxConfig.h):
- PPM parameters are set according to the selected PPM Configuration. The current default is [`FLYSKY_I6X`](TEST_PPM_Arduino/PPM_TxConfig.h):
  - `CHANNEL_NUMBER` — number of channels
  - `CHANNEL_DEFAULT_VALUE` — default servo mid value (µs)
  - `THROTTLE_DEFAULT_VALUE` — default throttle (µs)
  - `PPM_FRAME_LEN` — frame length (µs)
  - `PPM_PULSE_LEN` — separation pulse length (µs)
  - `PPM_PULSE_POLARITY` — pulse polarity (0 = low pulse, 1 = high pulse)
- TX Axis mapping modes: define one of the modes at top of the sketch (`MODE_1`, `MODE_2`, `MODE_3`, `MODE_4`) — this controls how transmitter axes map to joystick axes.  
>[!IMPORTANT]
> Plan accordingly to the Tx mode you want to operate in, the current default is stated in [PPM_TxConfig.h](TEST_PPM_Arduino/PPM_TxConfig.h)
> The pin used to connect to the  transmitter, the [`PPM_PIN`] is set by default to [`Pin 7`]

## [Operation modes of a RC Transmitter](https://www.rcgroups.com/forums/showatt.php?attachmentid=714749)  

<p align="center">
<img src="/Media/stickmodes.jpg" alt="Alt Text" width="300">    
<br>
Image from RcGroups
</p>
  
### Notes about code
- The sketch uses Timer1 and an interrupt routine (`ISR(TIMER1_COMPA/B_vect)` in [USB_PPM_Arduino.ino](TEST_PPM_Arduino/PPM_TxConfig.cpp)) to generate precise PPM timing.
- USB handling is prepared via the USB Host Shield library; (see https://github.com/felis/USB_Host_Shield_2.0)

## Configuration in [THRUSTMASTER_FCS.h](TEST_USB_Arduino/THRUSTMASTER_FCS.h):
- The `THRUSTMASTER_FCS` class is declared in this header file to handle gathering the joystick data.
- This header file contains the Vendor ID and Product ID of the joystick as well as the mappings for the Axis and Buttons
- Four basic function calls are provided for higher level processing, feel free to add more:
   - `OnGamePadChanged` Called to handle the information of the joystick axis and throttle slider;  
   - `OnHatSwitch` Called for handling the directional "hat" or dpad on top of the joystick;
   - `OnButtonUp` Called for handling button key releases;
   - `OnButtonDn` Called for handling button key presses;

### Notes about code
 - The data from the joystick is parsed in using the `ParseHIDData` function in [THRUSTMASTER_FCS.cpp](TEST_USB_Arduino/THRUSTMASTER_FCS.cpp), different joysticks may require different parsing.
 - Please consult the information on [Usb Host Development Blog](https://chome.nerpa.tech/mcu/using-logitech-extreme-3d-pro-joystick-with-arduino-hid-library/)

 ## Configuration in [ARDUINO_USB_PPM.h](ARDUINO_USB_PPM.h):
- The `ThrustMasterPPM` class is declared in this header file to handle transforming the joystick data into a PPM signal.
- This class implements a `Throttle_saftety` variable to act as a switch, locking the Throttle values to 0 when active.
- Implemented the four basic function calls from `THRUSTMASTER_FCS`:
   - `OnGamePadChanged` Called to handle the information of the joystick axis and throttle slider;  
   - `OnHatSwitch` Called for handling the directional "hat" or dpad on top of the joystick;
   - `OnButtonUp` Called for handling button key releases;
   - `OnButtonDn` Called for handling button key presses;
- Provides four basic function calls from the Joystick "Hat" for higher level processing, feel free to add more:
   - `handleHatUp`
   - `handleHatDown`
   - `handleHatLeft`
   - `handleHatRight`

- The `ThrustMasterPPM_Display` class is created to override the `ThrustMasterPPM` in this header file too.
- This class receives a pointer to the LCD `LiquidCrystal_I2C` class, allowing the usage of the Lcd inside its functions.
- Implemented the four basic function calls from `ThrustMasterPPM`, to navigate the LCD information screens:
   - `handleHatUp`
   - `handleHatDown`
   - `handleHatLeft`
   - `handleHatRight`



### Notes about code

## Build & upload
1. Install PlatformIO (Arduino IDE requires changing the location of the header files to the root folder).
2. Install the USB Host Shield 2.0 library.
3. Open "USB_PPM_Arduino.ino".
4. Select board and port, then Upload.

## Usage
- Select the desired `*.ino` file:
    - [PPM Generation test example](TEST_PPM_Arduino/TEST_PPM_Arduino.ino)
    - [USB Host read Joystick test example](TEST_USB_Arduino/TEST_USB_Arduino.ino)
    - [Code to generate PPM signals based on Joystick](Arduino_USB_PPM.ino)
- Compile and upload the code.
Open Serial Monitor to view initialization and diagnostic messages.
- Connect PPM output to your transmitter/receiver input or to an oscilloscope to verify waveforms.
Usage of I2C display is not mandatory, feel free to remove its code if not needed.

## Troubleshooting
- No PPM output:
  - Verify the PPM pin matches [`PPM_PIN`](TEST_PPM_Arduino/PPM_TxConfig.h.ino), check wiring and is not used by other hardware.
  - Timer1 is not used by other libraries.
  - The USB Host Shield library is initialized successfully (check Serial output).
- If timings are off:
  -Re-check constants in [`TEST_PPM_Arduino/PPM_TxConfig.h`](TEST_PPM_Arduino/PPM_TxConfig.h.ino),  E.g. `PPM_FRAME_LEN`, `PPM_PULSE_LEN`, etc.
  - Use an oscilloscope or logic analyzer to validate pulse widths and frame timing.

# References
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
  
# License
- This project is distributed under the terms of the GNU General Public License v3. See [LICENSE](LICENSE).

