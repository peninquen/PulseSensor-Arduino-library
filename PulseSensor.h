/**********************************************************************/
/* PulseSensor library
/* Arduino library to control pulse sensor like hall effect flowmeters,
/* and o water counters, electric counters using photodiode
/* the pulse sensor signal must be conected to one of the pins
/* attached to one of the external interrupt.
/* version 0.3 BETA 19/09/2015 
/* Author: Jaime García  @peninquen
/* License:  Apache License Version 2.0.
/*
/**********************************************************************/

#ifndef PulseSensor_h
#define PulseSensor_h

#include "Arduino.h"

extern void detectPulseISR();    /* Interrupt Service Routine (ISR)  */
// PIN D2  -> INT 0  UNO/MEGA
// PIN D3  -> INT 1  UNO/MEGA
// PIN D21 SCL -> INT 2  MEGA
// PIN D20 SDA -> INT 3  MEGA
// PIN D19 RX1 -> INT 4  MEGA
// PIN D18 TX1 -> INT 5  MEGA


class PulseSensor {
  public:
    //constructor
    PulseSensor();

    // Setup instance variables
    void begin(int pulsePin, unsigned int interval, float rateConversion, float acumConversion);

    // check interval and update data, interval must be greater than loop cycle
    void refreshData();

    // read _counter value in defined units
    float read();

    // read instant value in defined units
    float readInstant();

    // read _acumCounter value in defined units
    float readAcum();

    // read _AcumCounter
    unsigned int readPulses();

    //read instance acumulated pulse counter
    unsigned long readAcumPulses();

    // _acumCounter reset
    void resetAcum();

    // is a new data available?
    boolean available();

  private:
    bool _flag;                        // true when data is processed, false when data is readed
    unsigned long _acumCounter;        // Acumulated pulse counter
    unsigned long _processTime;        // last time process
    unsigned int _counter;             // last number of pulses registered
    unsigned int _interval;            // time [miliseconds] between get data from registers
    float _rateConversion;             // conversion factor from [pulse per second] to your units
    float _acumConversion;             // conversion factor from acumalated pulses to your units
};

#endif
