#!/usr/bin/python

# This tool finds out the ideal resistor value for a resistor bridge which includes a NTC thermistor
# as one of the two elements.
# The NTC thermistor is considered to be the lower element of the bridge :


import sys
import math
from pathlib import Path

schematic ="""

  Vcc (5v)
    ┃
   ┏┻┓
   ┃ ┃ R
   ┗┳┛
    ┣━━━━━ Vout
   ┏┻┓
   ┃/┃ NTC
   ┗┳┛
    ┃
   ━┻━ GND
"""

def print_help():
    print("This tool computes the ideal value of R (upper resistor) that will allow for the maximum voltage swing of the bridge assuming NTC value is changing between 2 values.")
    print("Note that the wider the range, better will be the output voltage swing amplitude.")
    print(f"schematic : {schematic}\n")
    print(f"Usage : {Path(__file__).name} rmin rmax")
    print("Where : ")
    print("     rmin : minimum value of NTC resistance (high temperature)")
    print("     rmax : maximum value of NTC resistance (low temperature)")
    print("Note : select those values to match the typical temperature range that the NTC will see.")


def main(args : list[str]) -> int:
    blue = "\033[34;1m"
    normal = "\033[0m"


    if "-h" in args or "--help" in args :
        print_help()
        return 0

    if len(args) != 2 :
        print(f"/!\\ Wrong number of arguments. Expected 2 arguments, got {len(args)}")
        print_help()
        return 1

    rmin = float(args[0])
    rmax = float(args[1])

    ideal_r = math.sqrt( (math.pow(rmax, 2) * rmin - math.pow(rmin, 2)*rmax) / (rmax - rmin))
    print(f"Ideal resistance value for R should be : {blue}{ideal_r}{normal}")
    print("The unit (Ohms, KOhms, MOhms) is the same as input data")

    return 0

if __name__ == "__main__" :
    exit(main(sys.argv[1:]))