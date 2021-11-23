#ifndef RJNetUDP_h
#define RJNetUDP_h

#include <Ethernet.h> 
#include <EthernetUdp.h> 
#include <Arduino.h>

//Function that sends message to certain IP 
//Function that recieves UDP message if available 

class RJNetUDP 
{
    public:
        RJNet();
        static void sendMessage(String message, EthernetUDP &Udp, IPAddress &remoteIP);
        static void sendMessage(float num, EthernetUDP &Udp, IPAddress &remoteIP);
        static String receiveMessage(EthernetUDP &Udp);
        const static unsigned int RJNET_PORT;
};


#endif