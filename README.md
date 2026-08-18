# Industrial Tank Level Monitoring System using STM32

## Project Overview

An embedded tank level monitoring and alarm system developed using the STM32F091RC microcontroller.

The system continuously measures the water level using an HC-SR04 ultrasonic sensor, displays the measured level and distance on a 16×2 I²C LCD, provides visual and audible alarms based on configurable thresholds, stores the user-defined low-level threshold in AT24C32 EEPROM, and transmits monitoring data through UART for PC-based observation and logging.

---

## Key Features

- Real-time tank water-level measurement using HC-SR04 ultrasonic sensor
- Percentage-based water-level calculation
- 16×2 LCD display using I²C communication
- Configurable low-level threshold using push buttons
- Non-volatile threshold storage using AT24C32 EEPROM
- Critical-level alarm using LED and buzzer
- Green LED indication for normal operating level
- Red LED indication for low-level condition
- UART data transmission for PC monitoring
- Timer-based ultrasonic measurement
- STM32 HAL-based firmware architecture

---

## Key Technical Highlights

- Configured STM32F091RC peripherals using STM32CubeMX / CubeIDE
- Implemented HC-SR04 distance measurement using TIM2 input capture
- Developed I²C communication for both LCD and AT24C32 EEPROM
- Implemented non-volatile storage of user-configurable alarm threshold
- Used UART for real-time monitoring and debugging
- Implemented GPIO-based user interface with SET, UP and DOWN buttons
- Designed threshold-based alarm logic using LEDs and buzzer
- Used STM32 HAL APIs for peripheral control
- Debugged I²C communication and EEPROM addressing using STM32 debugger
- Integrated multiple peripherals into a single firmware application

##  System Operation

The HC-SR04 ultrasonic sensor measures the distance between the sensor and the water surface.

The measured distance is converted into water level:

    Water Level = Tank Height − Measured Distance

The water level is then converted into percentage:

    Level (%) = (Water Level / Tank Height) × 100

The firmware compares the calculated level with two thresholds:

1. Critical threshold
2. User-configurable low-level threshold

Based on the measured level, the system controls the LEDs and buzzer.

---

## Alarm Logic

| Tank Level | Red LED | Green LED | Buzzer |
|------------|---------|-----------|--------|
| Critical level | ON | OFF | ON |
| Low level | ON | OFF | OFF |
| Normal level | OFF | ON | OFF |

The low-level threshold can be modified by entering the settings mode using the SET button and adjusting the value using UP/DOWN buttons.

---

## EEPROM Configuration

The AT24C32 I²C EEPROM is used to store the user-configured low-level threshold.

This allows the configured threshold to remain available after:

- MCU reset
- Power cycle
- System restart

The STM32 reads the stored threshold during startup.

If no valid value is present in EEPROM, a default threshold is loaded.

---

##  LCD Interface

A 16×2 LCD is interfaced with the STM32 through an I²C interface.

### Normal Mode

The LCD displays:

    Level: XX%
    Dist: XX.Xcm

### Settings Mode

The LCD displays:

    LOW LEVEL
       XX%

---

##  UART Monitoring

The system transmits monitoring information through UART for PC-side observation.

Example output:

    Distance: 12.5 cm
    Water Level: 11.5 cm
    Level: 48 %

This interface can be used for debugging, monitoring and future data-logging functionality.

---

##  Hardware Used

- STM32F091RCT6 / NUCLEO-F091RC
- HC-SR04 Ultrasonic Sensor
- AT24C32 I²C EEPROM
- 16×2 I²C LCD
- Red LED
- Green LED
- Buzzer
- SET push button
- UP push button
- DOWN push button
- Pull-up resistors for I²C bus
- USB-to-UART interface

---

##  Microcontroller Peripherals

| Peripheral | Purpose |
|------------|---------|
| TIM2 | Ultrasonic input capture |
| TIM6 | Microsecond timing / delay |
| I²C1 | LCD and EEPROM communication |
| USART1 | UART monitoring |
| GPIO | LEDs, buzzer and push buttons |

---

##  Communication Protocols

- **I²C** — LCD and AT24C32 EEPROM
- **UART** — PC monitoring
- **GPIO** — Buttons, LEDs and buzzer

---

##  Software & Tools

### Programming

- Embedded C
- STM32 HAL
- Interrupt-based input capture

### Development Environment

- STM32CubeIDE
- STM32CubeMX configuration
- ST-LINK debugger

### Debugging / Testing

- Multimeter
- Oscilloscope
- UART terminal
- STM32 debugger

---

##  Project Structure

```text
Industrial_Tank_Level_Monitoring/
│
├── Core/
│   ├── Inc/
│   │   ├── eeprom.h
│   │   ├── ultrasonic.h
│   │   ├── i2c_lcd.h
│   │   └── ...
│   │
│   └── Src/
│       ├── main.c
│       ├── eeprom.c
│       ├── ultrasonic.c
│       ├── i2c_lcd.c
│       └── ...
│
├── Drivers/
│
├── Startup/
│
├── Industrial_Tank_Level_Monitoring.ioc
├── .project
├── .cproject
├── .gitignore
└── README.md

##  Future Improvements

- Replace blocking delays with non-blocking timer-based scheduling
- Add EEPROM data validation using checksum/CRC
- Add ultrasonic sensor fault detection
- Implement continuous UART-based data logging
- Add RS-485 / Modbus communication for industrial integration
- Improve button debouncing using timer/interrupt-based handling
- Add a watchdog timer for system reliability


##  Author

**Adesh Patil**

Embedded Systems / Firmware Engineer