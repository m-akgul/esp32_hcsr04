#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>

constexpr uint8_t ULTRASONIC_TRIG_PIN = 26;
constexpr uint8_t ULTRASONIC_ECHO_PIN = 25;

void ultrasonicInit();

float ultrasonicReadDistanceCm();

#endif