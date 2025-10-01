# Mechanical Clock Project

## Overview
This repository contains the code, schematics, and documentation for building a custom mechanical clock using four digit modules in a 24-hour format. Each digit is driven by a stepper motor for mechanical display, controlled via an ESP32-S3 Super Mini master module over I2C. The project includes homing functionality with Hall effect sensors and LED indicators for status.

The setup is modular, with two digit modules on each side of the master (e.g., left2 ← left1 ← master → right1 → right2). Power is supplied externally at 5V, and modules are connected using JST XH connectors.

**Key Features:**
- 24-hour time display with mechanical digits.
- Stepper motor control for each digit.
- Automatic homing using Hall sensors.
- I2C communication with voltage level shifting for safety.
- Modular design for easy assembly and expansion.

## Components
### Master Module
- ESP32-S3 Super Mini (controls I2C bus and logic).

### Per Digit Module (x4)
- PCF8575 GPIO expander (I2C to GPIO).
- ULN2003 driver board (for stepper control).
- 28BYJ-48 stepper motor (drives the digit mechanism).
- A3144 Hall effect sensor (for homing).
- Red LED (homing in progress indicator).

### Additional Hardware
- External 5V power supply (2A+ recommended).
- Bidirectional I2C level shifter (e.g., BSS138-based, 4-channel) – **critical** to bridge 3.3V (ESP32) and 5V (PCF8575).
- Resistors: 4.7kΩ (I2C pull-ups x4), 10kΩ (A3144 pull-up x4), 330Ω (LED current limiter x4).
- Capacitors: 0.1µF ceramic (decoupling for each IC), 10-100µF electrolytic (at power entry).
- Wires: 20-22 AWG for power/GND; twisted-pair for SDA/SCL.
- Connectors: JST XH 4-pin (5V, GND, SDA, SCL).

## Usage
- Power the system; ESP32 boots and initializes modules.
- Clock syncs via NTP (add WiFi code if desired) or manual set.
- Homing: On startup, each digit homes using Hall sensor; LED blinks during process.
- Display updates every minute via steppers.
---
