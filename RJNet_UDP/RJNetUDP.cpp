#include "RJNetUDP.h"

/*
This class sends data via UDP. Unlike the previous version of this class, this class assumes
every message can fit in a UDP dataframe.
*/

const uint16_t RJNet::RJNET_PORT = 6915;

void RJNet() {}

/*
 *  Receives data from UDP if that is available. Returns NULL if not available
 */
UDP_message RJNet::readData(EthernetUDP &udp) {
    if(udp.parsePacket()){
        //We got a packet
        int numBytesRead = udp.available();
        char packetBuffer[numBytesRead + 1];
        udp.read(packetBuffer, numBytesRead);
        packetBuffer[numBytesRead] = 0;     //Make sure it is null-terminated
        
        return {true, udp.remoteIP(), udp.remotePort(), String(packetBuffer)};
        
    }
    else{
        //No packet
        return {false};
    }
}

/*
 * Sends info to the listed IP address. The data must be short enough to fit in a UDP dataframe.
 * @Note: not named print() to limit confusion with Arduino Stream library interface
 * EthernetUDP: the global EthernetUDP instance. We write to it.
 * msg: Message to send
 * destination_ip: The IP address to send to. We automatically insert the RJNET port.
 */
void RJNet::sendData(EthernetUDP &udp, const String &msg, const IPAddress &destination_ip) {
    udp.beginPacket(destination_ip, RJNET_PORT);
    udp.write(msg.c_str(), msg.length());
    udp.endPacket();    //Sends the packet
}

