//#include "samplerv1.h"
#include "speedSampler.h"
#include "seatCounter.h"
#include "network.h"
#include "secrets.h"

// last time you sent data to the server, in milliseconds
unsigned long lastConnectionTime = 0;    

// delay between updates, in milliseconds
const unsigned long POSTINGINTERVAL = 400L; 

// These values are defined by 'secrets.h' (which is not committed)
char ssid[]   = WIFI_SSID;       // your network SSID (name)
char pass[]   = WIFI_KEY;        // your network password (use for WPA, or use as key for WEP)
char server[] = SERVER_NAME;     // name of the server to send data to

// These are the Arduino processor pin numbers
const int SPEEDPIN = 2;
const int SEATPIN = 4;

SpeedSampler speedSampler;
SeatCounter seatCounter;
Network network = Network(ssid, pass);

void setup()
{
    Serial.begin(9600);
    // Configure my LED
    pinMode(LED_BUILTIN, OUTPUT);

    speedSampler.setup(SPEEDPIN, speedTransitionIsr);
    seatCounter.setup(SEATPIN, seatTransitionIsr);

    if(network.start() != NETWORK_OK)
    {
        Serial.println("Could not connect to WiFi");
        while(true); // stop
    }

    network.setServer(server);

    // Turn on the LED once we have WiFi
    digitalWrite(LED_BUILTIN, HIGH);
}

// Global interrupt handler for the speed pin
void speedTransitionIsr()
{
    speedSampler.transitionDetected();
}

// Global interrupt handler for the seat pin
void seatTransitionIsr()
{
    seatCounter.transitionDetected();
}

void loop()
{
    unsigned long currentTime = millis();

    // Every POSTINGINTERVAL ms, send the current state to the server.
    if(currentTime - lastConnectionTime > POSTINGINTERVAL)
    {
        lastConnectionTime = currentTime;

        network.sendData(currentTime, speedSampler.getCurrentSpeed(), seatCounter.getSeatCount());
    }
}

