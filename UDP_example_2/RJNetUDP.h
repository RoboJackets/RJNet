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
        static void sendMessage(const String &message, EthernetUDP &Udp, const IPAddress &remoteIP);  //Sends message in Arduino String format
        static void sendMessage(const char message[], EthernetUDP &Udp, const IPAddress &remoteIP);   //Sends message in C-stye char[] format
        static void sendMessage(float num, EthernetUDP &Udp, const IPAddress &remoteIP);
        static Message receiveMessage(EthernetUDP &Udp);
        const static unsigned int RJNET_PORT;
};


#endif
