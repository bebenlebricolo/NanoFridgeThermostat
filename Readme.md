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