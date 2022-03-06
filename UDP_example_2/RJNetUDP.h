#ifndef RJNetUDP_h
#define RJNetUDP_h

#include <Ethernet.h> 
#include <EthernetUdp.h> 
#include <Arduino.h>

struct Message {
  bool received;
  String message;
  IPAddress ipaddress;
};

//Function that sends message to certain IP 
//Function that recieves UDP message if available 

class RJNetUDP 
{
    public:
        RJNet();
        static void sendMessage(String message, EthernetUDP &Udp, const IPAddress &remoteIP);
        static void sendMessage(float num, EthernetUDP &Udp, const IPAddress &remoteIP);
        static Message receiveMessage(EthernetUDP &Udp);
        const static unsigned int RJNET_PORT;
};


#endif
