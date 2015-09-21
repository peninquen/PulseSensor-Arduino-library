/**********************************************************************/
/* PulseSensor library
/* a class to control a pulse sensor connected to one of the pins
/* attached to one of the external interrupt.
/* version 0.3 BETA 19/09/2015
/* Author: Jaime García  @peninquen
/* Licence: Apache License Version 2.0.
/*
/**********************************************************************/


#include "PulseSensor.h"


volatile unsigned int _COUNTER;
volatile unsigned long _startTime;
volatile unsigned long _finishTime;


void detectPulseISR() {
  _COUNTER++;
  _startTime = micros();
  _finishTime = _startTime;
}

/***************************************************************************/
/*constructor*/

PulseSensor::PulseSensor() {
}

/***************************************************************************/
/*Setup variables and initialize interrupts*/

void PulseSensor::begin(int pulsePin, unsigned int interval, float rateConversion, float acumConversion) {
  _interval = interval;
  _rateConversion = rateConversion;
  _acumConversion = acumConversion;
  _processTime = millis();       // start timer
  _flag = false;
  _acumCounter = 0;
  _counter = 0;                  // reset instance counter
  _COUNTER = 0;                  // reset global counter
  Serial.print("Pulse Pin:"); Serial.print(pulsePin);
  Serial.print("  INT"); Serial.println(digitalPinToInterrupt(pulsePin));
  pinMode(pulsePin, INPUT_PULLUP); // 
  attachInterrupt(digitalPinToInterrupt(pulsePin), detectPulseISR, RISING);
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
/*read instant value in defined units*/
float PulseSensor::readInstant() {
  unsigned long nowTime = micros();
  unsigned long period;
  byte oldSREG = SREG;
  noInterrupts();
  if (_finishTime - _startTime > nowTime - _finishTime)
    period = _finishTime - _startTime;
  else period = nowTime - _finishTime;
  SREG = oldSREG;            //enable last state interrupts register
  return (float)1000 / _rateConversion / period;
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
