#pragma once 
#include <Ethernet.h> 
#include <EthernetUdp.h> 
#include <Arduino.h>

//Function that sends message to certain IP 
//Function that recieves UDP message if available 

class RJNetUDP 
{
    public:
        RJNet();
        sendMessage(String &message, EthernetUDP &Udp, IPAddress &remoteIP);
        sendMessage(float &num, EthernetUDP &Udp, IPAddress &remoteIP);
        String receiveMessage(EthernetUDP &Udp);
        const static unsigned int RJNET_PORT;
};