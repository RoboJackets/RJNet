#include "RJNetUDP.h"

const unsigned int RJNetUDP::RJNET_PORT = 8888;
Message RJNetUDP::receiveMessage(EthernetUDP &Udp)
{
    int packetSize = Udp.parsePacket();
    if(packetSize) {
        char packetBuffer[packetSize+1];
        Udp.read(packetBuffer, packetSize);
        packetBuffer[packetSize] = 0; //can only convert null terminated character arrays to String
        return {true, String(packetBuffer), Udp.remoteIP()};
    }
    return {false};
}

void RJNetUDP::sendMessage(const String &message, EthernetUDP &Udp, const IPAddress &remoteIP) 
{
    sendMessage(message.c_str(), Udp, remoteIP);
}

void RJNetUDP::sendMessage(const char message[], EthernetUDP &Udp, const IPAddress &remoteIP) 
{
    Udp.beginPacket(remoteIP, RJNET_PORT);
    Udp.write(message);
    Udp.endPacket();
}

void RJNetUDP::sendMessage(float num, EthernetUDP &Udp, const IPAddress &remoteIP) 
{
    Udp.beginPacket(remoteIP, RJNET_PORT);
    Udp.write(String(num, 5).c_str());  //rounding float to 5 decimal places
    Udp.endPacket(); 
}
