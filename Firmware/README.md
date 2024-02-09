# Index
- [Index](#index)
- [NanoFridgeThermostat V2](#nanofridgethermostat-v2)
- [Hardware specs (first iteration)](#hardware-specs-first-iteration)
- [Safety features](#safety-features)
    - [Automatic rerun protection](#automatic-rerun-protection)
    - [Temperature trigger Hysteresis (No PID)](#temperature-trigger-hysteresis-no-pid)
- [Roadmap](#roadmap)
  - [V2.0](#v20)
  - [V2.1](#v21)
  - [V2.2](#v22)
  - [V3 and Onwards](#v3-and-onwards)

# NanoFridgeThermostat V2
Off-the-shelf fridge thermostat using an ArduinoNano to replace a broken fridge thermomechanical thermostat.

This project was born with the sole purpose of repairing a dying fridge whose thermostat was completely dead.
The idea is first to provide a very basic cooling support, with a simple on/off cycle that can be programmed using on seconds and off seconds.

# Hardware specs (first iteration)
* Solid state switching circuitry
  * Optoisolated triac driver circuit (MOC3042 -> withstands 400V peak)
  * 600V / 8A RMS Triac (FT0814MH TU) -> TO220 package
* NTC Thermistor [amazon link](https://www.amazon.fr/gp/product/B07CQT5S7Y/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1)
  * Advertised  as 100k @ 25°C
  * Upper bridge resistor of 330kOhms gives the maximum output voltage swing from the divider bridge configuration (see [Thermistor spreadsheet](Thermistor%20values.ods) for a more detailed view).
* Phone USB charger (5V), 230V AC -> 5V DC
  * Ideally this could be replaced by a standalone AC/DC converter in the system, but for now a simple phone charger bloc will do the trick.

# Safety features
### Automatic rerun protection
As fridges' thermodynamic cycle has some constraints, it's not possible to restart the motor for a short period of time.
My fridge is equipped with a fail-safe circuitry that breaks the circuit whenever the motor is stalled. A stalled motor draws much more current than expected in normal use, and it makes a thermal circuit breaker heat up, until the contact points separate ; the arm cools down and after some time springs back in normal position and closes the circuit again.

I don't want to rely only on this safety feature, so I'll add a software timer in between motor start up so that I'm sure it's not started in a too short amount of time.

### Temperature trigger Hysteresis (No PID)
I'll use an Hysteresis instead of a PID trigger for now and keep track of the ON/OFF cycles, avoiding to start the motor too often.
The risk when starting the motor too often (as stated above) is to go into thermal runaway or to stress the motor and compressor too much with start up cycles.
This can be minimized by opening a window that relies on a hysteresis.
PID will do the trick as well, the hysteresis function is more than sufficient to keep hovering around the average temperature whilst keeping the startup cycles at a minimum.

# Roadmap
## V2.0
* Fixed 4.0 °C degrees temperature target, no UI (no screen, no buttons, no interactions)
* Fixed hysteresis window (+- 2°C around the set point)
* Start up protection delay (as mentioned in [Automatic rerun protection](#automatic-rerun-protection))

## V2.1
* Flexible target temperature (dynamically adjustable). Requires some level of human interaction
  * I2C Oled screen with buttons
    * "+" and "-" buttons
    * One "Ok" button

## V2.2
* Probably a PID

## V3 and Onwards
* Additional light control (LEDs)
* Fans to circulate the air within the fridge
* Custom PCB that hosts all of the features described above