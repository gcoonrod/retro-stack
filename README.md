# retro-stack

A retro computer project based on stackable PCB boards. This project was largely inspired by Ben Eater's 6502 breadboard computer project.

## Project Objective

Design, prototype, and build an 8 bit computer roughly comparable to an Apple II or NES in capabilities.

### Hardware Design Goals

Create an 8 bit computer with the following basic capabilities:

- Stable at or near maximum frequency of the processor
- Modular (including reconfigurable address map)
- Character display via RS-232 Serial and LCD
- PS/2 Keyboard Input

Optional:

- Sound generation
- VGA video output
- 10baseT ethernet

### Software Design Goals

- Develop a strong understanding of Assembly programming
- Understand and implement the following protocols:
  - UART
  - SPI
  - PS/2
- Learn the basics of PLDs and CUPL to use in "glue logic"
- Develop a moderate to strong understanding of Atmel AVR MCU programming
- Implement a simple Operating System within the constraints of an 8 bit architecture
- Understand and implement both software and hardware interrupts

## Repository Layout

This repository contains all of the hardware and software assets in the following structure:

- [Hardware Schematics and PCB Layouts](./boards/)
- [Firmware](./firmware/)
- Tooling (TBD)
- Documentation (TBD)s
