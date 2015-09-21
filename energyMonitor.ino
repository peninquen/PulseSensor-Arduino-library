/**********************************************************************/
/* PulseEnergyMonitor example
/* An example to collect data from a pulse wattmeter using PulseSensor class
/* version 0.2 ALPHA 21/09/2015
/* Author: Jaime García  @peninquen
/* License: Apache License Version 2.0.
/*
/**********************************************************************/
/*
PULSE ELECTRICAL ENERGY MONITOR
DDS238-1 SINGLE-PHASE DIN-RAIL TYPE WATT-HOUR METER
TECHNICAL DETAILS
STANDARD IEC 62053-21 (IEC61036)
Nominal voltage: 120/220/230/240 V +-10%
Basic current (It): 5 A
Maximun current (Ib1) 32 A
Minimun current: 0.02 A
Frequency: 50/60 Hz
Consumption: <2w / 10 VA
Accurancy class: 1
Display: Mechanical 5+1 digits
         LCD 5+1 / 6+1 digits
Interface: Open colector output (SO)  SO+ ---------------------- PIN 2 (INT0) INTERNAL PULLUP RESISTOR
           18-27V 27mA                SO- ---------------------- GND
Impulse length: >= 30ms                       
units conversion:
Energy: 3200 imp = 1 KWh -> 1 imp = 0,3125 Wh; 
Power: 1 imps/s = 1125 W; 
Current(voltage 230V)-> 1 imp/s = 4.89 A;
maximum impulse frequency (30 A): 6.13 imp/s -> period 163 ms 
basic frequency (5 A): 1.02 imp/s -> period 978.3 ms
minimun impulse frequency (0.02 A): 1.0 imp/min-> period 60000 ms
Mounting: DIN rail 18 mm (1 module)
include flasing led proportional to load (1000 imp/KWh)
*/


#include "PulseSensor.h"


#define REFRESH_INTERVAL  60000      // refresh time, 1 MINUTE
#define WRITE_INTERVAL 300000UL  // values send to serial port, 15 minutes (5 * 60 * 1000)
#define PULSE_PIN 18 // see external interrupt pins available on your Arduino.
                     // Conect an external 10 Kohm pull up resistor  on input pin is recomended
#define PULSES_SEC_2_WATT 8.88889e-4 //(1/1125) conversion factor from pulses/second to watts
#define PULSES_2_KWH 3200 // conversion factor from pulses to KWh
PulseSensor wattmeter; // instance to collect data
//variables to process and send values
float power;
float maxPower;
float minPower;
float energy;
float lastEnergy;
boolean firstData;

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;


void setup() {
  Serial.begin(9600);
  delay(1000);
  wattmeter.begin(PULSE_PIN, REFRESH_INTERVAL, PULSES_SEC_2_WATT, PULSES_2_KWH);
  Serial.println("time(s), average power(watt), max power, min power, energy(KWh)");

  firstData = false;
  power = 0;
  maxPower = 0;
  minPower = 0;
  energy = 0;
}

void loop() {
  sei();
  if (wattmeter.available()) {
    power = wattmeter.read();
    if (!firstData) {
      if (maxPower < power) maxPower = power;
      if (minPower > power) minPower = power;
    }
    else {
      maxPower = power;
      minPower = power;
      firstData = false;
    }
  }

  currentMillis = millis();
  if (currentMillis - previousMillis >= WRITE_INTERVAL) {
    previousMillis = currentMillis;
    energy = wattmeter.readAcum();
    power = (energy - lastEnergy) * 3600 * 1000 * 1000 / WRITE_INTERVAL; //average power KWh/s to W
    lastEnergy = energy;
    firstData = true;

    Serial.print(currentMillis / 1000);
    Serial.print(",");
    Serial.print(power);
    Serial.print(",");
    Serial.print(maxPower);
    Serial.print(",");
    Serial.print(minPower);
    Serial.print(",");
    Serial.println(energy);

  }
}
