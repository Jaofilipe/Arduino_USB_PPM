# Arduino_USB_PPM
<<<<<<< HEAD

Convert a USB joystick input into a PPM signal for an RC transmitter using an Arduino + USB Host Shield.

This repository contains:
- [USB_PPM_Arduino.ino](USB_PPM_Arduino.ino) — main Arduino sketch
- [LICENSE](LICENSE) — GNU GPL v3
- [.gitignore](.gitignore)

**TODO: Add PPM test code and USB to PPM code**

## Features
- Generates multi-channel PPM signal for FLYSKY I6X and other common transmitters.
- Configurable PPM channel count, frame length and pulse settings in the sketch.
- Simple demo mode that animates a channel if no USB joystick is present.

## Quick start

Requirements
- Arduino (Uno was used)
- USB Host Shield (see USB Host Shield 2.0) 
- USB joystick/gamepad
- USB Host Shield library: https://github.com/felis/USB_Host_Shield_2.0

Basic Wiring
- PPM output: connect the PPM signal to the pin defined as [`PPM_PIN`](USB_PPM_Arduino.ino).

Configuration (in [USB_PPM_Arduino.ino](USB_PPM_Arduino.ino))
- TX Axis mapping modes: define one of the modes at top of the sketch (`MODE_1`, `MODE_2`, `MODE_3`, `MODE_4`) — this controls how transmitter axes map to joystick axes. The current default is [`MODE_3`](USB_PPM_Arduino.ino).
![Operation modes of a RC Transmitter](https://www.rcgroups.com/forums/showatt.php?attachmentid=714749)
- PPM parameters are set according to the selected PPM Configuration. The current default is [`FLYSKY_I6X`](USB_PPM_Arduino.ino):
  - [`CHANNEL_NUMBER`](USB_PPM_Arduino.ino) — number of channels
  - [`CHANNEL_DEFAULT_VALUE`](USB_PPM_Arduino.ino) — default servo mid value (µs)
  - [`THROTTLE_DEFAULT_VALUE`](USB_PPM_Arduino.ino) — default throttle (µs)
  - [`PPM_FRAME_LEN`](USB_PPM_Arduino.ino) — frame length (µs)
  - [`PPM_PULSE_LEN`](USB_PPM_Arduino.ino) — separation pulse length (µs)
  - [`PPM_PULSE_POLARITY`](USB_PPM_Arduino.ino) — pulse polarity (0 = low pulse, 1 = high pulse)

Notes about code
- The sketch uses Timer1 and an interrupt routine (`ISR(TIMER1_COMPA_vect)` in [USB_PPM_Arduino.ino](USB_PPM_Arduino.ino)) to generate precise PPM timing.
- USB handling is prepared via the USB Host Shield library; (see https://github.com/felis/USB_Host_Shield_2.0)

Build & upload
1. Install Arduino IDE (or PlatformIO).
2. Install the USB Host Shield 2.0 library.
3. Open "USB_PPM_Arduino.ino" in the IDE.
4. Select board and port, then Upload.

Usage
- After upload, plug the USB joystick into the Host Shield.
- The sketch will read joystick axes (if USB code enabled) and map them to PPM channel timings; otherwise it runs a demo oscillation on channel 0.
- Use transmitter/receiver to verify channel outputs.

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

License
- This project is distributed under the terms of the GNU General Public License v3. See [LICENSE](LICENSE).
=======
## Convert USB joystick inouts into a PPM signal for an RC Transmitter

# Project was based on the following for information:
## USB HOST to PPM OUTPUT:
* https://github.com/DroneMesh/USB_PPM
* https://github.com/drorgl/USBJoystickPPM
* https://github.com/lexfp/le3dpToPPM

## PPM INPUT:
* https://github.com/joshbuker/mcu-ppm-input

## FLYSKY FSI6X PPM TIMINGS:
* https://github.com/aemarkov/PPMControl/blob/master/AudioPPM/StandartProfiles.cs

## TX AXIS OPERATION MODES:
* https://www.rcgroups.com/forums/showthread.php?1122256-whats-difference-between-mode-1-and-mode-2
![Operation modes of a RC Transmitter](https://www.rcgroups.com/forums/showatt.php?attachmentid=714749)
>>>>>>> c9eb4ea... Update README
