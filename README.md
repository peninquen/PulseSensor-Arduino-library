# PulseSensor-Arduino-library
Arduino library to control pulse sensor like hall effect flowmeters, and o water counters, electric counters using photodiode
Conect yor 0-5V signal to an external interrupt of the Arduino UNO or MEGA 2560

PIN D2  -> INT 0  UNO/MEGA

PIN D3  -> INT 1  UNO/MEGA

PIN D21 SCL -> INT 2  MEGA

PIN D20 SDA -> INT 3  MEGA

PIN D19 RX1 -> INT 4  MEGA

PIN D18 TX1 -> INT 5  MEGA

if necesary, use a pullup 10K resistor.

First, declare an instance of the PulseSensor class.

Inside Setup(), initilize your instance. Use conversion factors from your technical datasheet.

Inside loop(), refreshData() or ask if a new value is available.
Read() the pulse rate (float) or the acumulated pulses (unsigned long). 

I test it with a water flow sensor YF-402, arduino UNO and Arduino MEGA 2560 and works (precision not tested)  

TODO: use pin change interrupt to allow every pin.
TODO: integrate with pool controler to detect water flows through pH and ORP sensors
TODO: integrate with photodiode to detect water leakage (reading water counter)
TODO: integrate with ArduinoDomoticExcontrol
