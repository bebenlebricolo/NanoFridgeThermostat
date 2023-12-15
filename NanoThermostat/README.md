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
  * Maybe 510k as the upper bridge resistor
* Phone USB charger (5V), 230V AC -> 5V DC
* 
 
# Roadmap :
