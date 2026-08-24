#include "ultrasonic.h"

enum UltrasonicState
{
    ULTRASONIC_IDLE,
    ULTRASONIC_WAIT_FOR_ECHO_START,
    ULTRASONIC_WAIT_FOR_ECHO_END
};


static UltrasonicState ultrasonicState = ULTRASONIC_IDLE;

static uint32_t measurementStartTime = 0;
static uint32_t echoStartTime = 0;

static float distanceCm = -1.0f;
static bool newDataAvailable = false;


// Maximum time to wait for the echo signal
constexpr uint32_t ECHO_TIMEOUT_US = 30000;


void ultrasonicInit()
{
    pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
    pinMode(ULTRASONIC_ECHO_PIN, INPUT);

    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

    ultrasonicState = ULTRASONIC_IDLE;
}


void ultrasonicStartMeasurement()
{
    // Don't start another measurement while one is running
    if (ultrasonicState != ULTRASONIC_IDLE)
    {
        return;
    }

    newDataAvailable = false;
    distanceCm = -1.0f;

    // Send trigger pulse
    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

    measurementStartTime = micros();

    ultrasonicState = ULTRASONIC_WAIT_FOR_ECHO_START;
}


void ultrasonicUpdate()
{
    uint32_t currentTime = micros();

    switch (ultrasonicState)
    {
        case ULTRASONIC_IDLE:
            break;


        case ULTRASONIC_WAIT_FOR_ECHO_START:

            // Did ECHO become HIGH?
            if (digitalRead(ULTRASONIC_ECHO_PIN) == HIGH)
            {
                echoStartTime = currentTime;

                ultrasonicState =
                    ULTRASONIC_WAIT_FOR_ECHO_END;
            }

            // No echo arrived in time
            else if (currentTime - measurementStartTime >=
                     ECHO_TIMEOUT_US)
            {
                distanceCm = -1.0f;
                newDataAvailable = true;

                ultrasonicState = ULTRASONIC_IDLE;
            }

            break;


        case ULTRASONIC_WAIT_FOR_ECHO_END:

            // Did ECHO return LOW?
            if (digitalRead(ULTRASONIC_ECHO_PIN) == LOW)
            {
                uint32_t echoDuration =
                    currentTime - echoStartTime;

                distanceCm =
                    echoDuration * 0.0343f / 2.0f;

                newDataAvailable = true;

                ultrasonicState = ULTRASONIC_IDLE;
            }

            // ECHO stayed HIGH too long
            else if (currentTime - echoStartTime >=
                     ECHO_TIMEOUT_US)
            {
                distanceCm = -1.0f;
                newDataAvailable = true;

                ultrasonicState = ULTRASONIC_IDLE;
            }

            break;
    }
}


bool ultrasonicDataAvailable()
{
    return newDataAvailable;
}


float ultrasonicGetDistanceCm()
{
    newDataAvailable = false;

    return distanceCm;
}