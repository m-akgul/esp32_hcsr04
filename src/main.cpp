#include <Arduino.h>
#include <ultrasonic.h>

void setup()
{
    Serial.begin(115200);
    ultrasonicInit();

    Serial.println("System started");
}

void loop()
{

    // Ultrasonic
    static uint32_t lastUltrasonicTime = 0;
    uint32_t currentTime = millis();

    if (currentTime - lastUltrasonicTime >= 100)
    {
        lastUltrasonicTime = currentTime;

        float distance = ultrasonicReadDistanceCm();

        if (distance >= 0)
        {
            Serial.print("Distance: ");
            Serial.print(distance);
            Serial.println(" cm");
        }
        else
        {
            Serial.println("No echo");
        }
    }
}