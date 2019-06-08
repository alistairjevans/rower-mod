#include "Arduino.h"
#include "seatCounter.h"

void SeatCounter::setup(int seatPin, voidFuncPtr transitionIsr)
{   
    pinMode(seatPin, INPUT_PULLUP);
    // Attach an interrupt waiting for the PIN to transition to low
    attachInterrupt(digitalPinToInterrupt(seatPin), transitionIsr, FALLING);
    seatCount = 0;
}

int SeatCounter::getSeatCount()
{ 
    return seatCount;
}

void SeatCounter::transitionDetected()
{
    unsigned long transitionTime = millis();
    // If interrupts come faster than 100ms, assume it's a bounce and ignore
    if (transitionTime - lastTransitionTime > 100) 
    {
        transitionCount++;
        if(transitionCount % 2 == 0)
        {
            // A seat count is two transitions; one when 
            // the seat moves over it on the way out, and
            // one when the seat moves back.
            seatCount++;
        }
    }
    lastTransitionTime = transitionTime;
}