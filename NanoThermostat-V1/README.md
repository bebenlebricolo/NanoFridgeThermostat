# NanoFridgeThermostat (V1)
Off-the-shelf fridge thermostat using an ArduinoNano to replace a broken fridge thermomechanical thermostat.

This project was born with the sole purpose of repairing a dying fridge whose thermostat was completely dead.
The idea is first to provide a very basic cooling support, with a simple on/off cycle that can be programmed using on seconds and off seconds.

The project also features a "bypass" switch that allows to manually trigger the fridge cooling process (useful for a fridge that first needs to be cooled down quite a lot before entering a "steady" on/off cycle ).

Polarity of said switch is meant for a circuit breaker switch type (because that's what I had around, so that explains why the logic is reversed.)
All components were components I had lying around :
* Arduino nano (clone, using the CH341 UART to USB bridge)
* Double relay board, with optoisolators as input.
* Wires and female header (single row, 2.54 mm pitch)
* Electrical tape to isolate components

# Future improvements (in V2)
* Using a triac and triac driver as the main switching elements
    * Solid state is cool and removes the issue of relay contact points wearing down
* Adding a thermistor to measure temperature within the fridge and cycle towards a target temperature
See [NanoThermostatV2 Readme](../NanoThermostat/README.md).
