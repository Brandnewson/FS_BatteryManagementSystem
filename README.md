# FS_BatteryManagementSystem
University of Leeds Formula Student 2024 EV Electrics Battery Management System (BMS).

## Overview
The Orion BMS 2 we use can only support up to 8 thermistors directly connected to the BMS. We need to measure all thermally critical cells and 30% of cells. As we need to measure equally distributed cells, we essentially need to monitor every single battery pack. The battery packs we use are the Enepaq VTC5A Sony/Murata Li-ion Battery Module. One massive advantage of this module pack is that it has a single connection that acts are a voltage shunt that is equivalent to the highest temperature measured in the module. 

Orion to sell expansions units but the modules are not compatible with the voltage shunt style temperature sensors. Therefore, we must emulate our own. Now this can be emulated in two ways either by CAN bus (which is how Orion’s own modules work) or represent the temperature sensors values to thermistor inputs. 

The following information should bring you up to speed on the key tasks that need to be completed.

## Objective
- Extend the Battery Management System by creating our own expansion modules to gain 100% temperature sensor coverage of every single battery pack in the accumulator.

## How it works
- To gain 100% temperature sensor coverage of accumulator, an expansion unit is emulated for the BMS Orion 2.
- This expansion unit is emulated by the CANbus protocol.
- A total of 132 temperature sensors in [number] battery modules line the accumulator.
- There is a thermocouple in each battery module that acts as a voltage shunt.
- We use microcontrollers to process the data from the thermocouple? at specific frequencies. [clarification needed]
- Grab the highest & lowest temp for each module. [clarification needed]
- We utilise a multiplexor to "choose" between the [number] battery modules. [clarification needed]
- Return the highest & lowest temperatures of accumulator to the BMS for safety monitoring. [clarification needed]
- Because the BMS Orion 2 can now "see" the remaining 70% module temperatures, we gain 100% temperature sensor coverage of the accumulator.

## Key data
### Circuit:
[insert picture of circuit]

### Microcontroller Framework:
Arduino Teensy chosen because of plenty of documentation regarding the topic.

### Build Platform
PlatformIO chosen because [Reason]

## Applicable Formula Student Rules
-	Most rules are covered in EV5.8, most rules are covered by the temperature sensors we are using
-	A possible one to thing about is - EV5.8.7 (Temp error must occur for more than 1s), sample rate will need to be supplied to IMD/AMS Latch team.
-	EV5.8.8 – Might need to add galvanic isolation before Teensy?
-	Another thing that needs to be ensured is all AMS signals are SCS (see T11.9)

## Keywords & information
Accumulator - Accumulators are rechargeable batteries <br>
Thermocouple - Simply a sensor that is used for measuring temperature <br>
Battery Management System - Protects the battery from operating outside its safe operating area. <br>
EMI - Electromagnetic Interference <br>
CAN bus - high-integrity serial bus system, for our purposes, we're interested in how it tackles EMI effectively. <br>
