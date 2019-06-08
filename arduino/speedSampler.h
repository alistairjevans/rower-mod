#ifndef SPEEDSAMPLER_H
#define SPEEDSAMPLER_H

class SpeedSampler 
{
    public:
        void setup(int speedPin, voidFuncPtr transitionIsr);
        void loop();

        double getCurrentSpeed();
        
        void transitionDetected();

    private:
        unsigned long msLastSample;

        // Declare as volatile, because our 
        // interrupt will be changing it
        volatile int transitionCount;
};

#endif