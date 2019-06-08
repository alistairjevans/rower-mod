
#include "Arduino.h"
#include <WiFiNINA.h>
#include "network.h"

#define TOTAL_WAIT_TIME 60000 // 1 minute to connect to WiFi
#define ATTEMPT_TIME 5000     // 5 seconds between connect attempts

WiFiClient client;

Network::Network(char* ssid, char* password) 
{
    wifiStatus = WL_IDLE_STATUS;   
    this->ssid = ssid;
    this->pass = password;
    server = NULL;
}

void Network::setServer(char* server)
{
    this->server = server;
}

int Network::start()
{
    // check for the WiFi module:
    if (WiFi.status() == WL_NO_MODULE) {
        return NETWORK_ERROR_STARTUP;
    }

    unsigned long startTime = millis();
    unsigned long lastAttemptTime = 0;

    // attempt to connect to Wifi network:
    while (wifiStatus != WL_CONNECTED) {
        
        unsigned long currentTime = millis();
        
        if(currentTime - startTime > TOTAL_WAIT_TIME)
        {
            // Exceeded timeout, fail.
            return NETWORK_ERROR_WIFIFAIL;
        }
        else if(currentTime - lastAttemptTime > ATTEMPT_TIME)
        {
            Serial.println("Attempting Wifi Connection");
            lastAttemptTime = currentTime;
            // Connect to WPA/WPA2 network:
            wifiStatus = WiFi.begin(ssid, pass);        
        }
        else 
        {
            // wait 500ms before we see how we're going
            delay(500);
        }
    }

    printCurrentNet();

    return NETWORK_OK;
}

bool Network::isConnected() 
{
    return WiFi.status() == WL_CONNECTED;
}

void Network::sendData(unsigned long timestamp, double speed, int seatCount)
{   
    int success = 1;
    char request[256];
    char body[64];
 
    if(!client.connected())
    {
        success = client.connect(server, 5000);
    }

    if(success)
    {
        // Empty the buffer 
        // (I don't really care about the response)
        while(client.read() != -1);

        // Clear the request data
        memset(request, 0, 256);
        // Clear the body data
        memset(body, 0, 64);        

        // I can't use sprintf %f for floats on the arduino, I have to use dtostrf.     
        sprintf(body, "milliseconds=%lu&count=%d&speed=", timestamp, seatCount); 
        dtostrf(speed, 2, 3, &body[strlen(body)]);

        char* currentPos = request;

        // I'm using sprintf for the fixed length strings here
        // to make it easier to read.
        currentPos += sprintf(currentPos, "POST /data/providereading HTTP/1.1\r\n");
        currentPos += sprintf(currentPos, "Host: %s:%d\r\n", server, 5000);
        currentPos += sprintf(currentPos, "Connection: keep-alive\r\n");
        currentPos += sprintf(currentPos, "Content-Length: %d\r\n", strlen(body));
        currentPos += sprintf(currentPos, "Content-Type: application/x-www-form-urlencoded\r\n");
        currentPos += sprintf(currentPos, "\r\n");
        
        // Append the body to the request
        strcpy(currentPos, body);

        // Send the entire request
        client.print(request);

        // Force the wifi module to send the packet now
        // rather than buffering any more data.
        client.flush();
    }
}

void Network::printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}