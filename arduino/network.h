#ifndef NETWORK_H
#define NETWORK_H

#define NETWORK_OK              0
#define NETWORK_ERROR_STARTUP   1
#define NETWORK_ERROR_WIFIFAIL  2


class Network 
{
    public:
        Network(char* ssid, char* pass);
        void setServer(char* server);
        int start();
        void sendData(unsigned long timestamp, double speed, int seatCount);
        bool isConnected();

    private:
        int wifiStatus;     // the Wifi radio's status
        char* ssid;
        char* pass;
        char* server;
        char buffer[256];

        void printCurrentNet();
};

#endif