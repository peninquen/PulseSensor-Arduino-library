/**********************************************************************/
/* Flowmeter example
/* An example to collect data from a flowmeter using PulseSensor class
/* version 0.3 BETA 22/09/2015
/* Author: Jaime García  @peninquen
/* Licence: Apache License Version 2.0.
/*
/**********************************************************************/
/*
EFFECT HALL FLOWMETER
TECHNICAL DETAILS

 Electrical:
Working Voltage: 5 to 18VDC
Max current draw: 15mA @ 5V
Working Flow Rate: 1 to 30 Liters/Minute
Working Temperature range: -25 to 80°C
Working Humidity Range: 35%-80% RH
Maximum water pressure: 2.0 MPa
Output duty cycle: 50% +-10%
Output rise time: 0.04us
Output fall time: 0.18us
Flow rate pulse characteristics: 
Flow rate: 1 pulse/s = 7.5 L/min
Acumulated volume: 1 Liter = 450 pulses
maximum pulse frequency: 30 L/min-> 225 imp/s -> min period 4.44 ms  (delta 0.1 L/min->0.015 ms)
                          5 L/min-> 37.5 imp/s->     period 26.66 ms (delta 0.1 L/min->0.523 ms)
minimun pulse frequency:  1 L/min-> 7.5 imp/s -> max period 133.3 ms (delta 0.1 L/min->12.1 ms)
Durability: minimum 300,000 cycles
Mechanical:
1/2" NPS nominal pipe connections, 0.78" outer diameter, 1/2" of thread
Size: 2.5" x 1.4" x 1.4"
*/

#include "PulseSensor.h"


#define REFRESH_INTERVAL  1000          // refresh time, 1 second
#define WRITE_INTERVAL 5000             // values send to serial port, 5 seconds (5 * 1000)
#define PULSE_PIN 18                    // see external interrupt pins available on your Arduino.
#define PULSES_2_LITERS 450             // coefficient conversion from pulses to liters
#define PULSES_SEC_2_LITERS_MINUTE 7.5  // (450/60) coefficient conversion pulses/second from to liters/minute
PulseSensor flowmeter;                  // instance to collect data
//variables to process and send values
float flowRate;
float maxRate;
float minRate;
float acumFlow;
float lastAcumFlow;
boolean firstData;

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;


void setup() {
  Serial.begin(9600);
  delay(1000);
  flowmeter.begin(PULSE_PIN, REFRESH_INTERVAL, PULSES_SEC_2_LITERS_MINUTE, PULSES_2_LITERS);
  Serial.println("time(s), average flowrate(l/min), max rate, min rate, volume(liters)");

  firstData = false;
  flowRate = 0;
  maxRate = 0;
  minRate = 0;
  acumFlow = 0;
}

void loop() {
  sei();
  if (flowmeter.available()) {
    flowRate = flowmeter.read();
    if (!firstData) {
      if (maxRate < flowRate) maxRate = flowRate;
      if (minRate > flowRate) minRate = flowRate;
    }
    else {
      maxRate = flowRate;
      minRate = flowRate;
      firstData = false;
    }
  }

  currentMillis = millis();
  if (currentMillis - previousMillis >= WRITE_INTERVAL) {
    previousMillis = currentMillis;
    acumFlow = flowmeter.readAcum();
    flowRate = (acumFlow - lastAcumFlow) * 60 * 1000 / WRITE_INTERVAL; //average flowrate
    lastAcumFlow = acumFlow;
    firstData = true;

    Serial.print(currentMillis / 1000);
    Serial.print(",");
    Serial.print(flowRate);
    Serial.print(",");
    Serial.print(maxRate);
    Serial.print(",");
    Serial.print(minRate);
    Serial.print(",");
    Serial.println(acumFlow);

  }
}
