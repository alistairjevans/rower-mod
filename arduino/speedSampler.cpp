#include "Arduino.h"
#include "speedSampler.h"

void SpeedSampler::setup(int speedPin, voidFuncPtr transitionIsr)
{   
    pinMode(speedPin, INPUT_PULLUP);
    // Attach an interrupt waiting for the PIN to transition to low
    attachInterrupt(digitalPinToInterrupt(speedPin), transitionIsr, FALLING);
    msLastSample = millis();
}

double SpeedSampler::getCurrentSpeed()
{ 
    unsigned long currentMs = millis();
    unsigned long timeSinceLast = currentMs - msLastSample;

    // How many fractions of a second have passed?
    double secondFraction = 1000.0 / timeSinceLast;

    // Multiply up to get transitions / second.
    double result = transitionCount * secondFraction;

    // Reset
    transitionCount = 0;
    msLastSample = currentMs;

    return result;
}

void SpeedSampler::transitionDetected()
{ 
    transitionCount++;
}