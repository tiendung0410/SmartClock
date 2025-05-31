# STM32 Electronic Clock Project

## Overview

This project is an electronic clock built using the STM32F103C8T6 microcontroller. It features a real-time clock (RTC) for accurate timekeeping, a user interface for setting the time and alarms, and a DFPlayer module for playing alarm sounds. The clock is designed to be user-friendly, with intuitive button controls for easy operation.

## Features

- **Real-Time Clock (RTC)**: Keeps accurate time and date.
- **Alarm Functionality**: Set multiple alarms with customizable times.
- **DFPlayer Module**: Play alarm sounds from a microSD card.
- **Volume Control**: Adjust the volume of the alarm sounds.
- **User Interface**: Simple button controls for setting time and alarms.
- **LCD Display**: Displays current time, date, and alarm status.

## Components Required

- STM32F103C8T6 Microcontroller
- DS3231 RTC Module
- DFPlayer Mini MP3 Player Module
- MicroSD Card (for alarm sounds)
- 16x2 LCD Display (or any compatible display)
- Push Buttons (for user input)
- Resistors (for button pull-down)
- Breadboard and Jumper Wires

## Wiring Diagram

```
STM32F103C8T6      DS3231       DFPlayer      LCD
-----------------  -----------  -----------  -------------
  GND  ------------  GND        GND         GND
  3.3V ------------  VCC        VCC         VCC
  PB6  ------------  SDA        RX          RS
  PB7  ------------  SCL        TX          E
  PA9  ------------  -          D0          D4
  PA10 ------------  -          D1          D5
  PA11 ------------  -          D2          D6
  PA12 ------------  -          D3          D7
```

## Button Controls

- **Button 1 (Set Time)**: 
  - Press to enter time-setting mode.
  - Use Button 2 to increment hours and Button 3 to increment minutes.
  - Press Button 1 again to save the time.

- **Button 2 (Set Alarm)**: 
  - Press to enter alarm-setting mode.
  - Use Button 1 to increment hours and Button 3 to increment minutes.
  - Press Button 2 again to save the alarm time.

- **Button 3 (Volume Control)**: 
  - Press to adjust the volume of the alarm sound.
  - Each press increases the volume (up to a maximum level).
  - Long press to decrease the volume.

- **Button 4 (Snooze)**: 
  - Press when the alarm sounds to activate snooze mode.
  - The alarm will pause and ring again after a preset duration.

## DFPlayer Integration

The DFPlayer Mini module allows you to play MP3 files stored on a microSD card. The following features are implemented:

- **Alarm Sound Selection**: 
  - Users can select different MP3 files for different alarms.
  - The file names should be in the format `001.mp3`, `002.mp3`, etc.

- **Volume Adjustment**: 
  - The volume can be adjusted using Button 3.
  - The volume range is from 0 (mute) to 30 (maximum volume).

## Installation

1. Clone or download the project repository.
2. Open the project in your preferred IDE (e.g., STM32CubeIDE).
3. Connect the STM32F103C8T6 to your computer using a USB programmer.
4. Compile and upload the code to the microcontroller.
5. Insert the microSD card with the alarm sounds into the DFPlayer module.
6. Power the circuit and enjoy your electronic clock!

## Usage

- After powering on, the clock will display the current time.
- To set the time, press Button 1 and follow the prompts.
- To set an alarm, press Button 2 and follow the prompts.
- Adjust the volume using Button 3.
- When the alarm goes off, press Button 4 to snooze.

## Conclusion

This STM32 electronic clock project is a fun and educational way to learn about microcontrollers, real-time clocks, and audio playback. Feel free to modify and expand upon this project to add more features or improve functionality!

## License

This project is open-source and available for personal and educational use. Please give credit if you use or modify the code.

---

For any questions or issues, please open an issue in the repository or contact the project maintainer. Happy coding!
