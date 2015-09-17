/**********************************************************************/
/* EnergyMonitor example
/* An example to collect data from a pulse wattmeter using PulseSensor class
/* version 0.1 ALPHA 17/09/2015
/* Author: Jaime García  @peninquen
/* Licence: Apache License Version 2.0.
/*
/**********************************************************************/
/*
PULSE ELECTRICAL ENERGY METER
TECHNICAL DETAILS

STANDARD IEC 62053-21 (IEC61036)
Nominal voltage: 120/220/230/240 V +-10%
Basic current (Ib2): 5 A
Maximun current (Ib1) 32 A
Minimun current: 0.02 A
Frequency: 50/60 Hz
Consumption: <2w / 10 VA
Accurancy class: 1
Display: Mechanical 5+1 digits
         LCD 5+1 / 6+1 digits
Interface: Open colector output (SO)
           18-27V 27mA
Impulse length: >= 30ms
Conversion factor:  3200 imp/KWh
maximum impulse frequency: 6.13 imp/s
minimun impulse frequency: 1.0 imp/min
Mounting: DIN rail 18 mm (1 module)
include flasing led proportional to load (1000 imp/KWh)
*/
