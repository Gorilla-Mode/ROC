# ROC - Resonant Orbit Calc(ulator)

Simple program to calculate elliptical resonant orbits for Kerbal Space Program.

## Usage

When building satellite constellations, it's necessary that satellites have the exact same
orbital period and proper separation. It's also very time-consuming and inefficient to
launch a satellite one by one.

When launching multiple satellites at once, this program calculates a proper insertion orbit for your satellites.
Ensuring proper separation between satellites.

## Features

- **Resonant Orbit Calculations**: Calculate orbital parameters for resonant elliptical orbits
- **Apoapsis/Periapsis Target**: Use either the apoapsis or periapsis as the target for insertion
- **Delta-V Calculations**: Determine the delta-v required for orbital insertion
- **Celestial Body Data**: Built-in data for KSP celestial bodies
- **Orbital Error Handling**: Accounts for line of sight, SOI, atmosphere intersection, etc.

##  Build

- Standard: C23
- Compiler: MinGW-w64
- Target: Windows
- CMake 4.1 (If you use Cmake)

## Dependencies
- PDcurses

## Status
- [x] Resonant Orbit Calculations
- [x] Apoapsis/Periapsis Target
- [x] Delta-V Calculations
- [x] Celestial Body Data
- [ ] Error Handling
- [ ] User Input
- [ ] UI