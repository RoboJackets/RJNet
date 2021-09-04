
/*
 * Example of Arduino board using RJNet for ethernet TCP.
 * This also uses RJNet for serial communication.
 * created 20 Mar 2019
 * by Brian Cochran
 * 
 */

#include <Ethernet.h>
#include "RJNetUDP.h" //make sure to install the RJNet library

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //the Wiznet 5500io chips have their own MAC, so use that
IPAddress ip(192, 168, 0, 177); //set the IP to find us at


EthernetUDP UDP;

//Computer 
IPAddress computerIP(192, 168, 0, 2);
 
void setup() {
  Serial.begin(115200);
  Serial.println("Beginning");
  
  // In case your RJ board wires the chip in an odd config,
  // otherwise, leave commented out
  // You can use Ethernet.init(pin) to configure the CS pin
  Ethernet.init(10);  // Most Arduino shields

  // initialize the ethernet device
  Ethernet.begin(mac, ip);

  //check that ethernet equipment is plugged in
  while (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    delay(500);
  }
  while(Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
    delay(500);
  }

  // start listening for clients
  UDP.begin(RJNet::RJNET_PORT);

  Serial.print("Our address:");
  Serial.println(Ethernet.localIP());


  
}
double counter = 0;
unsigned long startTime = 0;
void loop() {
    //Check if there is a new message
    UDP_message message = RJNet::readData(UDP);
    if (message.valid) {
        //If we got a new message print it out
        Serial.print(message.data); //show us what we read 
        Serial.print(" From ");
        Serial.print(message.remote_ip);
        Serial.print(":");
        Serial.println(message.remote_port);
    }

    if(millis() - startTime >= 500){
        RJNet::sendData(UDP, "Ping", computerIP);
        Serial.println("Sending data");
    }
    delay(1);

  

}
