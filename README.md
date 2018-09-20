# *OpenDSIM* - an opensource A/D mixed circuit simulator)

------
![Build status](https://img.shields.io/badge/opendsim-0.1-blue.svg) ![Build status](https://img.shields.io/badge/build-passing-orange.svg) ![Item status](https://img.shields.io/badge/status-unstable-lightgreen.svg)

OpenDSIM is a circuit simulator designed for analogous and digital circuit validation, which allows you to test a physical circuit in a real-time interactive environment with virtual instruments and analysis.

Based on node admittance analysis and event-driven logical model, it can simulate linear, non-linear, reactive and logical blocks (both combined and sequential logic).
There are various kinds of virtual electronic instruments such as voltage and current probe, plotter, oscilloscope and so on, which can be connected to the circuit directly and operated real-time.

All the electronic models and compnents are flexible and extensible, so more and more new models are easily adding to project tree.


# Why OpenDSIM
- Extensible and Flexible
> Unified interface of models and devices.Easy to extend new one to support new component or analysis.
- Real Time and Interactive
> Measure, analysis or operate the circuit real-time, just like a virtual laboratory where you can do what you want.
- Both Analog and Digital
> Inline AD/DA bridge that process both analogous and digital circuit. Effective digital simulation framework separated from analog part provides higher performance.
- Fast and Effective
> Streamlined low-expense simulation model helps reducing calculation time.


# Viewing Project Tree

There are mainly three directories of root dir:

| filename | description |
|:--------:|:-----------:|
| `opendsim-core`          | Core library (Backend), including utilities, scheme-lisp parser and circuit simulation framework. |
| `opendsim-frontend`      | Console and GUI part |
| `opendsim-symbols`       | Schematic symbols of all devices |
| `include`                | C/C++ Header Files that contains exported and common definitions |

`opendsim-core` directories view:

| filename | description |
|:--------:|:-----------:|
| `circuit` | Simulation framework of circuit object |
| `math` | Mathemathematical procedures, for example, matrix. |
| `models` | Primitive model library |
| `scheme-lisp` | A simple scheme-lisp parser for common purpose. |
| `util` | Utilities foundation |

`opendsim-frontend` directories view:

| filename | description |
|:--------:|:-----------:|
| `devices` | Device models library |
| `instruments` | Virtual Instrument library |
| `include` | Private C/C++ Header files that is limited within frontend |

# Concepts

Three-layout simulation system:
- `Layout-1`               : Primitive model
- `Layout-2`               : Device Package
- `Layout-3`               : Schematic

`Layout-1`: The primitive models is constructed by C/C++ language, which is accessible to node admittance matrix and logic event subsystem.They pay a low-level and important role of the whole simulator.

`Layout-2`: The device package is described by one or more primitive models and their physical connections.Maybe extra initial configuration of models should be provided as well.They also deal with what will be shown on schematic and process real-time GUI events sent by user, making a reflection if circuit status is changed. 

`Layout-3`: The schematic is what we inputed to computer and what we expected to process. A schematic consisted of devices, configuration, physical nodes (like vertex in graph theory) and connections (like edges).

# How to build

In Windows you should prepare Unix-like environment such as MinGW or CygWin that supports GCC toolchain. Ensure that Qt5.5.1 is installed properly for GUI frontend.

    cd to your source root
    
    ./configure
    
    make all
    make install
    
    
In Linux, you may need install Qt5.5.1, then just follow the procedure described above.

# Maintain
We're still in maintenance of this project though currently the item is in alpha stage. A bootstrap script is used to construct building system.
Maintenance-Mode is disabled by default, however, you can enable it if it really needs.
