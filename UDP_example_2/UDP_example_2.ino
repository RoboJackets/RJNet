#include <Ethernet.h>
#include <EthernetUdp.h>
#include "RJNetUDP.h"

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF
};
//0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
const IPAddress ip(192, 168, 20, 175);
const IPAddress otherip(192, 168, 20, 4);
//IPAddress otherip(192, 168, 20, 255);  //Send to .255 to multicast the packet
unsigned int localPort = 8888;      // local port to listen on

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

float test = 5.0492;
void setup() {
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

  // start the Ethernet
  Ethernet.begin(mac, ip);
  //If you don't set retransmission to 0, the WIZNET will retry 8 times if it cannot resolve the
  //MAC address of the destination using ARP and block for a long time.
  //With this as 0, will only block for ~200ms
  Ethernet.setRetransmissionCount(0);

  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start UDP
  Udp.begin(localPort);

}
unsigned long start_time = 0;
void loop() {
  // if there's data available, read a packet
  Message message1 = RJNetUDP::receiveMessage(Udp);
  if(message1.received) {
    Serial.print("Message from IP: ");
    Serial.println(message1.ipaddress);
    Serial.println(message1.message);    
  }
  if(millis() - start_time >= 50) {
    RJNetUDP::sendMessage("hiiheoifjsoifejosijfoj", Udp, otherip);
    RJNetUDP::sendMessage(0.475823123, Udp, otherip);
    start_time = millis();
  }

}
