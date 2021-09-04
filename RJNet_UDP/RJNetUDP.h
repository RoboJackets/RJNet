#ifndef RJNetUDP_h
#define RJNetUDP_h

#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetUDP.h>

struct UDP_message {
    bool valid;   //If this is false, we didn't get a message. All other fields are invalid.
    IPAddress remote_ip;
    uint16_t remote_port;
    String data;
};

class RJNet {
    public:
        RJNet();
        static void sendData(EthernetUDP &, const String &, const IPAddress &);
        static UDP_message readData(EthernetUDP &);
        const static uint16_t RJNET_PORT;
};


#endif
