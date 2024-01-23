# Nano Fridge Thermostat
Welcome to the Nano Fridge project !
This project started up because my fridge thermostat was dying (actually it was pretty dead and noticed the fridge was not cooling anymore) and I needed to find something very quickly in order to keep it running a bit longer.
It's aimed at replacing the cheap (but very reliable) thermomechanical switch that's used in most low-end fridges.

The very first revision operated on an Arduino Nano and used regular relays to perform its task.
It was essentially an Open-Loop system, running the compressor for 1 hour, and letting it rest for 2 hours.
This Open Loop process works fine but does not take into account the internal temperature of the fridge.
Hence, the actual temperature was oscillating a lot between 2 or 3°C and 6 to 8°C, depending on how many times we would open it.

More details on this revision can be found in the [NanoThermostat-V1](NanoThermostat-V1) folder.

# Nano Thermostat V2
A newer revision of the thermostat is currently in the works and several small improvements will be provided :
* Safe compressor start up procedure
* Thermistor support (NTC)
* Closed-Loop control algorithms (very simple hysteresis at first)
* Screen and buttons to monitor and setup the fridge parameters from outside the fridge.

More details can be found in the [NanoThermostat-V2](NanoThermostat-V2) folder.

## Tools
### Thermistor data source files generator
This repository embeds a small python ([thermistor_generator.py](Tools/thermistor_generator.py)) script whose purpose is to generate C source files for a given thermistor. It uses the basic parameters for a NTC thermistor :
* R0 : base resistance of the thermistor, at room temperature (usually, 25°Celsius)
* Beta constant : temperature constant, rated in Kelvin, for this thermistor
* name of the generated source files

It generates the default profile for an NTC thermistor, which needs to be compiled in the project in order to be used.
It's used through the small [thermistor.h](NanoThermostat-V2/thermistor.h) and [thermistor.c](NanoThermostat-V2/thermistor.c) module as the reference table for temperature reading.

Usage :
```bash
python thermistor_generator.py 100 3950 thermistor_ntc_100k_3950K
# 1 - Generating thermistor data
# 2 - Generating header file
# 3 - Generating source file
```
This generates files in the [Generated](Generated) folder.

### Resistor bridge calculator
This small python script ([resistor_bridge.py](Tools/resistor_bridge.py)) lets you calculate the ideal value for the upper resistor for a resistor bridge where a NTC thermistor is used as the lower part.
It finds the ideal value that maximizes the output voltage swing amplitude, based on the thermistor used temperature/resistance range.
Boosting the output voltage swing amplitude means that the Arduino's ADC will get a better resolution, hence better result as the usable voltage range is bigger.

Usage :
```bash
python resistor_bridge.py 100 1024
Ideal resistance value for R should be : 320.0
The unit (Ohms, KOhms, MOhms) is the same as input data
```