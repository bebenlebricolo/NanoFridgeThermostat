#!/usr/bin/python

from io import TextIOWrapper
import sys
import math
from dataclasses import dataclass, field
from enum import Enum
from pathlib import Path

# Generates thermistor data c source files that exposes thermistor data
C_THERMISTOR_MAX_SAMPLES = 50
C_MIN_TEMP = -24
C_MAX_TEMP = 25
C_TEMP_STEP = 1
C_KELVIN_TO_DEG = 273.15
C_CALIB_TEMP = 25 + C_KELVIN_TO_DEG

@dataclass
class TempRes:
    temperature : float = 0
    resistance : float = 0

class ResistanceUnit(Enum):
    Ohms = "RESUNIT_OHMS"
    KiloOhms = "RESUNIT_KILOOHMS"
    MegaOhms = "RESUNIT_MEGAOHMS"

@dataclass
class ThermistorData :
    data : list[TempRes] = field(default_factory=list)
    unit : ResistanceUnit = ResistanceUnit.KiloOhms
    sample_count : int = 0

def print_help():
    print("This scripts generates thermistor data in the form of a C source files (header + source) pair.")
    print("They should be sourced / embedded in your application to be usable.\n")
    print(f"Usage : python {Path(__file__).name} r0 beta name (OPT)")
    print("Where : ")
    print("     r0      : base resistance of NTC thermistor (calibrated @25°C)")
    print("     beta    : β constant of this NTC thermistor")
    print("     name    : name of the output file (usually something like \"thermistor_100k_3950K\")")
    print("     (OPT)   : -h, --help  : prints this help")


def write_cplusplus_extern_begin(file : TextIOWrapper) :
    file.write("#ifdef __cplusplus\n")
    file.write("extern \"C\" {\n")
    file.write("#endif\n\n")

def write_cplusplus_extern_end(file : TextIOWrapper) :
    file.write("#ifdef __cplusplus\n")
    file.write("}\n")
    file.write("#endif\n\n")

def generate_header(filepath : Path, name : str, sample_count : int) -> None :
    header_define = name.upper() + "_HEADER"

    with open(filepath, "w") as file :
        file.write(f"#ifndef {header_define}\n")
        file.write(f"#define {header_define}\n\n")
        write_cplusplus_extern_begin(file)
        file.write(f"#include \"thermistor.h\"\n\n")
        file.write(f"#define {name.upper()}_SAMPLE_COUNT {sample_count}U\n\n")
        file.write(f"const thermistor_data_t {name}_data;\n\n")

        write_cplusplus_extern_end(file)
        file.write(f"#endif /* {header_define} */\n")

def generate_data(r0 : int, beta : int, step : int, min_temp : int, max_temp : int) -> ThermistorData :
    thermistor_data = ThermistorData()
    interval = range(min_temp, max_temp, step)

    for i in interval :
        data = TempRes()
        data.temperature = i + C_KELVIN_TO_DEG
        data.resistance = r0 * math.exp(beta * (1/(i + C_KELVIN_TO_DEG) - 1/C_CALIB_TEMP))
        thermistor_data.data.append(data)

    thermistor_data.sample_count = len(thermistor_data.data)

    # Raise a warning at the end, user might want to adjust parameters
    # or tweak C file him/herself
    if thermistor_data.sample_count >= C_THERMISTOR_MAX_SAMPLES :
        print(f"/!\\ too many samples in thermistor_data.data")
        print(f"    - samples_count : {len(interval)}")
        print(f"    - min temp : {min_temp}")
        print(f"    - max temp : {max_temp}")

    return thermistor_data

def generate_source_file(filepath : Path, name : str, data : ThermistorData) -> None :
    with open(filepath, "w") as file :
        file.write(f"#include \"{name}.h\"\n\n")
        file.write(f"const thermistor_data_t {name}_data =  {{\n")
        file.write("    .data = {\n")
        for i in range(len(data.data)):
            file.write(f"        {{{int(data.data[i].temperature - C_KELVIN_TO_DEG)}, {int(data.data[i].resistance)}}}")
            if i < len(data.data) - 1:
                file.write(",")
            file.write("\n")
        file.write("    },\n")
        file.write(f"    .unit = {ResistanceUnit.KiloOhms.value},\n")
        file.write(f"    .sample_count = {name.upper()}_SAMPLE_COUNT\n")
        file.write("};\n")

def main(args : list[str]) -> int:
    script_dir = Path(__file__).parent.parent
    output_directory = script_dir.joinpath("Generated")
    if not output_directory.exists() :
        output_directory.mkdir(parents=True, exist_ok=True)


    if "-h" in args or "--help" in args :
        print_help()
        return 0

    if len(args) != 3 :
        print(f"/!\\ Wrong number of input args. Given was : {len(args)}")
        print_help()
        return 1

    r0 = int(args[0])
    beta = int(args[1])
    name = args[2]

    print("1 - Generating thermistor data")
    thermistor_data = generate_data(r0, beta, 5, C_MIN_TEMP, C_MAX_TEMP)
    print("2 - Generating header file")
    generate_header(output_directory.joinpath(name + ".h"), name, C_THERMISTOR_MAX_SAMPLES)
    print("3 - Generating source file")
    generate_source_file(output_directory.joinpath(name + ".c"), name, thermistor_data)

    return 0





if __name__ == "__main__" :
    exit(main(sys.argv[1:]))