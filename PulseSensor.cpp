/*********************************************************************
* PulseSensor library
* a class to control a pulse sensor connected to one of the pins
* attached to one of the external interrupt.
* version 0.3 BETA 22/09/2015
* Author: Jaime García  @peninquen
* Licence: Apache License Version 2.0.
*
**********************************************************************/
//------------------------------------------------------------------------------
// Poner DEBUG a 1 para depuración.

#define DEBUG  0

//------------------------------------------------------------------------------
// Debug directives

#if DEBUG
#   define DEBUG_PRINT(...)    Serial.print(__VA_ARGS__)
#   define DEBUG_PRINTLN(...)  Serial.println(__VA_ARGS__)
#else
#   define DEBUG_PRINT(...)
#   define DEBUG_PRINTLN(...)
#endif


#include "PulseSensor.h"


volatile unsigned int _COUNTER;
volatile unsigned long _startTime;
volatile unsigned long _finishTime;


void detectPulseISR() {
  _COUNTER++;
  _startTime = _finishTime;
  _finishTime = micros();

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
  _startTime = 0;
  _finishTime = micros();

  DEBUG_PRINT(F("Pulse Pin:")); DEBUG_PRINT(pulsePin);
  DEBUG_PRINT(F("  INT")); DEBUG_PRINTLN(digitalPinToInterrupt(pulsePin));

  pinMode(pulsePin, INPUT_PULLUP); //
  attachInterrupt(digitalPinToInterrupt(pulsePin), detectPulseISR, RISING);
}

/***************************************************************************/
/*check interval and update data, include at least one time inside loop cycle*/
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
/*read instant value in defined units */
float PulseSensor::readInstant() {
  unsigned long period;
  if ((_counter == 0) && (_COUNTER < 2)) return 0.0; //we need 2 consecutive pulses to process
  byte oldSREG = SREG;
  noInterrupts();
  period = micros() - _finishTime;
  if (period < _finishTime - _startTime) period = _finishTime - _startTime;
  SREG = oldSREG;            //enable last state interrupts register
  return (float)1000 / _rateConversion / period;
}

/***************************************************************************/
/*read acumulated value [units]*/

float PulseSensor::readAcum() {
  return (float)_acumCounter / _acumConversion;
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
