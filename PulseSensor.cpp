/**********************************************************************/
/* PulseSensor library
/* a class to control a pulse sensor connected to one of the pins
/* attached to one of the external interrupt.
/* version 0.1 ALPHA 29/08/2015 ¡NOT TESTED!
/* Author: Jaime García  @peninquen
/* Licence: Released for public use.
/*
/**********************************************************************/


#include "PulseSensor.h"


volatile unsigned int _COUNTER;

void detectPulseISR() {
  _COUNTER++;
}

/***************************************************************************/
/*constructor*/

PulseSensor::PulseSensor() {
}

/***************************************************************************/
/*Setup variables and initialize interrupts*/

void PulseSensor::begin(int pulsePin, unsigned int interval, float rateConversion, float acumConversion) {
  int pulseInterrupt;
  _interval = interval;
  _rateConversion = rateConversion;
  _acumConversion = acumConversion;
  _processTime = millis();       // start timer
  _flag = false;
  _acumCounter = 0;
  _counter = 0;                  // reset instance counter
  _COUNTER = 0;                  // reset global counter
  switch (pulsePin) {
    case 2: pulseInterrupt = 0;
      break;
    case 3: pulseInterrupt = 1;
      break;
    case 21: pulseInterrupt = 2;
      break;
    case 20: pulseInterrupt = 3;
      break;
    case 19: pulseInterrupt = 4;
      break;
    case 18: pulseInterrupt = 5;
      break;
    default: return;
  }
  Serial.print("Pulse Pin:"); Serial.print(pulsePin);
  Serial.print("  INT"); Serial.println(pulseInterrupt);
  pinMode(pulsePin, INPUT); // conect external pull up resistor 10 Kohm on input pin
  attachInterrupt(pulseInterrupt, detectPulseISR, RISING);
}

/***************************************************************************/
/*check interval and update data, interval must be greater than loop cycle*/
void PulseSensor::refreshData() {
  unsigned long nowTime = millis();
  if (nowTime - _processTime >= _interval) {
    byte oldSREG = SREG;
    noInterrupts();
    _counter = _COUNTER;
    _COUNTER = 0;
    _acumCounter += _counter;  // increase counter
    _processTime = nowTime;     //stamp process time
    _flag = true;
    SREG = oldSREG;            //enable last state interrupts register
  }
}

/***************************************************************************/
/*read sensor value[units per second]*/

float PulseSensor::read() {
  _flag = false;
  return (float)(_counter * 1000.0 / _rateConversion / _interval);
}

/***************************************************************************/
/*read acumulated value [units]*/

float PulseSensor::readAcum() {
  return (float)(_acumCounter / _acumConversion );
}

/***************************************************************************/
/*read instance partial pulse counter*/

unsigned int PulseSensor::readPulses() {
  _flag = false;
  return _counter;
}

/***************************************************************************/
/*read instance acumulated pulse counter*/

unsigned long PulseSensor::readAcumPulses() {
  return _acumCounter;
}

/***************************************************************************/
/*reset acumulated pulse counter */

void PulseSensor::resetAcum() {
  _acumCounter = 0;
}

/***************************************************************************/
/* Is new data available */

boolean PulseSensor::available() {
  refreshData();
  return _flag;
}
