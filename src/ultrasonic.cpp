#include "ultrasonic.h"
#include "config.h"

void ultrasonicInit()
{
    pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
    pinMode(ULTRASONIC_ECHO_PIN, INPUT);

    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
}

float ultrasonicReadDistanceCm()
{
    // Generate a 10 µs trigger pulse
    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

    // Measure how long ECHO stays HIGH
    unsigned long duration =
        pulseIn(ULTRASONIC_ECHO_PIN, HIGH, 30000);

    // No echo received within timeout
    if (duration == 0)
    {
        return -1.0f;
    }

    // Distance = time × speed / 2
    float distanceCm = duration * 0.0343f / 2.0f;

    return distanceCm;
}