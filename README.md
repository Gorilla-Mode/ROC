# ROC - Resonant Orbit Calc(ulator)

>CLI tool to calculate elliptical resonant orbits for satellite constellations in Kerbal Space Program.

## About

Satellite constellations require satellites to maintain the exact same orbital period with proper
separation, five satellites should form a (near) perfect pentagon.

**ROC** solves this by calculating the insertion orbit for your satellites, ensuring proper separation and orbital
period for multiple satellites in a single launch.

That being said, you could always just build a big spinning gizmo and launch 100 satellites into space, prob more kerbal to
do that.

## Features

- **Resonant Orbit Calculations** - Calculate orbital parameters for resonant elliptical orbits
- **Apoapsis/Periapsis Target** - Use either the apoapsis or periapsis as the insertion target
- **Delta-V Calculations** - Determine the delta-v required for orbital insertion
- **Celestial Body Data** - Built-in data for all KSP celestial bodies
- **Orbital Error Handling** - Accounts for line of sight, SOI, and atmosphere intersection

## Showcase

<img width="1194" height="803" alt="image" src="https://github.com/user-attachments/assets/dba1bbf7-737c-4614-b0ce-acdd7ca7dc57" />


## Build

| Component | Specification |
|-----------|---------------|
| Standard  | C23           |
| Compiler  | MinGW-w64     |
| Platform  | Windows       |
| Build     | CMake 4.1+    |

## Dependencies

- **PDcurses** - Terminal UI library

## Testing

A separate executable is provided for unit testing

- **Orbital Calculations** 
- **Error Handling** 

## Feature Status

- [x] Resonant Orbit Calculations
- [x] Apoapsis/Periapsis Target
- [x] Delta-V Calculations
- [x] Celestial Body Data
- [x] Error Handling
- [x] Unit Tests (Can't be bothered to write more)
- [x] User Input
- [x] User Interface
- [ ] Visualisation
- [ ] Exporting Results
