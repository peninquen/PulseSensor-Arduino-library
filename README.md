# PulseSensor-Arduino-library
Arduino library to control pulse sensor like hall effect flowmeters, water and electric counters using photodiode or pulse output.
Connect yor 0-5V signal to an external interrupt of the Arduino UNO or MEGA 2560

PIN D2  -> INT 0  UNO/MEGA

PIN D3  -> INT 1  UNO/MEGA

PIN D21 SCL -> INT 2  MEGA

PIN D20 SDA -> INT 3  MEGA

PIN D19 RX1 -> INT 4  MEGA

PIN D18 TX1 -> INT 5  MEGA


First, declare an instance of the PulseSensor class.

Inside Setup(), initialize your instance. Use conversion factors from your technical datasheet.

Inside loop(), refreshData() or ask if a new value is available().
use diferent read functions to access pulse rate and acumulated pulses, also converted to your defined units.



I test it with a water flow sensor YF-402, arduino UNO and Arduino MEGA 2560 and works (precision not tested)  

TODO: use pin change interrupt to allow every pin.

TODO: integrate with pool controller to detect water flows through pH and ORP sensors

TODO: integrate with photodiode to detect water leakage (reading water counter)

TODO: integrate with ArduinoDomoticExcontrol
