# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

Arduino C++ firmware for a rover robot running on ATmega (Arduino). Targets a 16MHz AVR with ~2KB SRAM — memory is constrained. Uses `WProgram.h` / `WConstants.h` (older Arduino SDK headers, not the modern `Arduino.h`).

There is no build system in this repo. Compilation is done externally (e.g., Eclipse CDT with the AVR plugin, or `avr-g++` manually). The `arduino/` directory contains `libArduinoCore.a` — the precompiled Arduino core.

## Architecture

All project code lives in the `naxsoft` namespace. The main entry points are `setup()` and `loop()` in `main.cpp`, which manually call `init()` (Arduino core init) rather than relying on the standard Arduino runtime.

### Key subsystems

**Communication**
- `Thrift/` — Binary Thrift-inspired protocol over serial. `Protocol` handles read/write of typed fields; `XBeeATTransport` and `XBeeAPITransport` are the underlying byte transports over XBee radio modules.
- `Processor` — RPC dispatcher: reads a message from the transport and dispatches to the correct handler (motor, TLC PWM, sensors).

**I2C**
- `I2C/I2CProtocol` — Custom I2C master library (replaces Arduino's Wire). Global singleton `i2cProtocol`. Initialized with `begin()`, `pullup(true)`, `setSpeed(false)` (100kHz) or `setSpeed(true)` (400kHz).

**Sensors (all communicate via I2C)**
- `Accelermeter/Accelerometer` — LSM303DLH accelerometer (note: directory name typo is intentional/historical)
- `Compass/Compass` — LSM303DLH magnetometer
- `Gyroscope/Gyroscope` — L3G4200D 3-axis gyro, configurable for 250/500/2000 °/s
- `LSM303DLH/` — Lower-level driver for the combo accel/compass chip
- `DCM/` — Direction Cosine Matrix orientation filter (unfinished, see `DCM/README`)

**Actuation**
- `DcMotor` — H-bridge motor control. Modes: `MOTOR_CW`, `MOTOR_CCW`, `MOTOR_STOP`, `MOTOR_COAST` and PWM variants. Pins: forward=10, reverse=9, enable=8.
- `Tlc5940/` — TI TLC5940 16-channel PWM driver (LED/servo control). Global `Tlc` object; call `Tlc.init()` in setup, `Tlc.update()` each loop.

**Utilities**
- `Estimator` — Kalman-style scalar estimator (static method)
- `MemoryTest` — `availableMemory()` for SRAM diagnostics
- `DebugUtils.h` — Debug macros (enabled by `#define DEBUG 1` in `main.cpp`)
- `bitops.h` — Bit manipulation helpers
- `Matrix/MatrixMath` — Matrix operations for DCM/filter math

## Memory considerations

ATmega has very limited SRAM. Avoid heap allocation in loops. String literals should use `PROGMEM` where possible. `MemoryTest::availableMemory()` can be called to check free SRAM at runtime.

## Thrift protocol wire format

Commands are sent over XBee serial. Message structure (from `main.cpp` comments):
- `04 04 01 80` — T_ONEWAY, set TLC channel 1 to value 0x80
- `01 05` — T_CALL, read accelerometer
- `01 06` — T_CALL, read compass
- `01 07` — T_CALL, read gyroscope
