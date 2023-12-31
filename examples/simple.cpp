#include <Arduino.h>
#include <RotaryEncoder.h>


const byte ENCODER_BIT_RESOLUTION = 10;
const byte ENCODER_PINS[ENCODER_BIT_RESOLUTION] = {22, 13, 14, 27, 26, 25, 33, 32, 35, 34};

AbsoluteRotaryEncoder encoder(ENCODER_BIT_RESOLUTION);

void IRAM_ATTR isr();


void setup()
{
    Serial.begin(115200);
    encoder.init(ENCODER_PINS);

    // Set interrupt to encoder pins
    for (byte i = 0; i < ENCODER_BIT_RESOLUTION; i++)
    {
        attachInterrupt(ENCODER_PINS[i], isr, CHANGE);
    }
}

void loop()
{
    // Get the angle reading
    float angle = encoder.getDegree();

    // Get the position reading
    int position = encoder.getPosition();

    // Convert current position to angle
    float angle_2 = encoder.posToDeg(position);

    // Get the rotate direction
    int direction = encoder.getRotateDirection();

    // Check if the encoder on zero position
    bool isOnZero = encoder.isZero();

    // Always call this on loop
    encoder.loop();
}

void IRAM_ATTR isr()
{
  encoder.readBinISR();
}