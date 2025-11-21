# Arduino_USB_PPM
Convert a USB joystick input into a PPM signal for a RC transmitter using an Arduino + USB Host Shield.

This repository contains:
- [USB_PPM_Arduino.ino](USB_PPM_Arduino.ino) — main Arduino sketch that bridges USB HID to PPM
- [TEST_PPM_Arduino](TEST_PPM_Arduino/TEST_PPM_Arduino.ino) — PPM generation test example and PPM configuration headers
- [TEST_USB_Arduino](TEST_USB_Arduino/TEST_USB_Arduino.ino) — USB host joystick test example
- [Media_Folder](/Media/) — Folder containing some useful resources 
- [LICENSE](LICENSE) — GNU GPL v3
- [.gitignore](.gitignore)

## Features
- Generates multi-channel PPM signal for FLYSKY I6X and common transmitters (basic default PPM provided).
- Configurable PPM channel count, frame length and pulse settings via header file (see [TEST_PPM_Arduino/PPM_TxConfig.h](TEST_PPM_Arduino/PPM_TxConfig.h))
- Example to test PPM Generation (with demo mode that moves a channel)
- Example to test Joystick HID data reading

## Quick start
>[!NOTE]
>Requirements:
>- Arduino (Uno / ATmega328P tested)
>- USB Host Shield (see USB Host Shield 2.0) 
>- USB joystick/gamepad
>- USB Host Shield library: https://github.com/felis/USB_Host_Shield_2.0
>
>Wiring:
>- PPM output: Connect the PPM signal to the pin defined as `PPM_PIN` in the header file [`PPM_TxConfig.h`](TEST_PPM_Arduino/PPM_TxConfig.h).
>- USB HOST: Configure the USB HOST 2.0 jumper traces according to the desired voltages and Pin availability referring to the [USB Host 2.0 Hardware Manual](https://chome.nerpa.tech/usb-host-shield-hardware-manual/).

> [!IMPORTANT]
> Before Assembling the Circuit & Uploading  
> Please verify the pins used for PPM and USB HOST 2.0 Board before uploading!
> Timer1 direct output pins are [`Pin 9`] and [`Pin 10`]
> These pins are used by the USB HOST 2.0 [`SS`] ([`Pin 10`]) and [`INT`] ([`Pin 9`])
> There are 2 possible options:
>   - Change the [`PPM_PIN`] in the header file [`PPM_TxConfig.h`](TEST_PPM_Arduino/PPM_TxConfig.h).
>           
>   - Change the USB HOST 2.0 pins used for [`SS`] and [`INT`] and route a new wire accordingly.
>     - Read Section 5 of [`USB HOST SHIELD Hardware Manual`](https://chome.nerpa.tech/usb-host-shield-hardware-manual/)
>     - Variable to be changed in in [`UsbCore.h`] file of [`USB HOST SHIELD 2.0 Repo`](https://github.com/felis/USB_Host_Shield_2.0/blob/master/UsbCore.h)


Configuration (in [PPM_TxConfig.h](TEST_PPM_Arduino/PPM_TxConfig.h)):
- PPM parameters are set according to the selected PPM Configuration. The current default is [`FLYSKY_I6X`](TEST_PPM_Arduino/PPM_TxConfig.h):
  - `CHANNEL_NUMBER` — number of channels
  - `CHANNEL_DEFAULT_VALUE` — default servo mid value (µs)
  - `THROTTLE_DEFAULT_VALUE` — default throttle (µs)
  - `PPM_FRAME_LEN` — frame length (µs)
  - `PPM_PULSE_LEN` — separation pulse length (µs)
  - `PPM_PULSE_POLARITY` — pulse polarity (0 = low pulse, 1 = high pulse)
- TX Axis mapping modes: define one of the modes at top of the sketch (`MODE_1`, `MODE_2`, `MODE_3`, `MODE_4`) — this controls how transmitter axes map to joystick axes. The current default is `MODE_3`.

[Operation modes of a RC Transmitter](https://www.rcgroups.com/forums/showatt.php?attachmentid=714749)

<img src="/Media/stickmodes.jpg" alt="Alt Text" width="300">

Image from RcGroups


Notes about code
- The sketch uses Timer1 and an interrupt routine (`ISR(TIMER1_COMPA/B_vect)` in [USB_PPM_Arduino.ino](TEST_PPM_Arduino/PPM_TxConfig.cpp)) to generate precise PPM timing.
- USB handling is prepared via the USB Host Shield library; (see https://github.com/felis/USB_Host_Shield_2.0)

Build & upload
1. Install PlatformIO (Arduino IDE requires changing the location of the header files to the root folder).
2. Install the USB Host Shield 2.0 library.
3. Open "USB_PPM_Arduino.ino".
4. Select board and port, then Upload.

Usage
- Select the desired `*.ino` file:
    - [PPM Generation test example](TEST_PPM_Arduino/TEST_PPM_Arduino.ino)
    - [USB Host read Joystick test example](TEST_USB_Arduino/TEST_USB_Arduino.ino)
    - [Code to generate PPM signals based on Joystick](Arduino_USB_PPM.ino)
- Compile and upload the code.
Open Serial Monitor to view initialization and diagnostic messages.
- Connect PPM output to your transmitter/receiver input or to an oscilloscope to verify waveforms.

Troubleshooting
- No PPM output:
  - Verify the PPM pin matches [`PPM_PIN`](TEST_PPM_Arduino/PPM_TxConfig.h.ino), check wiring and is not used by other hardware.
  - Timer1 is not used by other libraries.
  - The USB Host Shield library is initialized successfully (check Serial output).
- If timings are off:
  -Re-check constants in [`TEST_PPM_Arduino/PPM_TxConfig.h`](TEST_PPM_Arduino/PPM_TxConfig.h.ino),  E.g. `PPM_FRAME_LEN`, `PPM_PULSE_LEN`, etc.
  - Use an oscilloscope or logic analyzer to validate pulse widths and frame timing.

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

