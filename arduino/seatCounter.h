#ifndef SEATCOUNTER_H
#define SEATCOUNTER_H

class SeatCounter
{
    public:
        void setup(int seatPin, voidFuncPtr transitionIsr);
        void loop();

        int getSeatCount();
        
        void transitionDetected();

    private:

        // Declare as volatile, because our 
        // interrupt will be changing them.
        volatile int seatCount;
        volatile int transitionCount;
        
        unsigned long lastTransitionTime = 0;        
};

#endif