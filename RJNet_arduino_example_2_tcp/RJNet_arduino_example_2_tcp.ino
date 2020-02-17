
/*
 * Two Arduino boards each with a server and a client connecting to the other's server
 * This uses RJNet
 * Modified 9 Feb 2020
 * by Brian Cochran and Peter Wilson
 * 
 */

#include <Ethernet.h>
#include "RJNet.h" //make sure to install the RJNet library

const static int PORT = 7; //port RJNet uses

//Don't forget to switch these around depending on which board you are programming!

// Enter a MAC address and IP address for your board below
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE };
IPAddress ip(192, 168, 0, 178); //set the IP to find us at
EthernetServer server(PORT);

// Enter a IP address for other board below
IPAddress otherIP(192, 168, 0, 177); //set the IP to find us at
EthernetClient otherBoard;
 
void setup() {
  Serial.begin(9600);
  
  // In case your RJ board wires the chip in an odd config,
  // otherwise, leave commented out
  // You can use Ethernet.init(pin) to configure the CS pin
  Ethernet.init(10);  // Most Arduino shields

  // initialize the ethernet device
  Ethernet.begin(mac, ip);

  Ethernet.setSubnetMask(

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
  server.begin();

  Serial.print("Our address: ");
  Serial.println(Ethernet.localIP());

  
}
double counter = 0;
long int startTime = 0;
void loop() {
  // wait for a new client:
  EthernetClient client = server.available();
  if (client) {
    String data = RJNet::readData(client);
    if (data.length() != 0) {
      Serial.print(data); //show us what we read 
      Serial.print(" From ");
      Serial.print(client.remoteIP());
      Serial.print(":");
      Serial.println(client.remotePort());

      Serial.println(millis());
      
      //do something useful with data here, maybe reply
      RJNet::sendData(client, "Message Received");
      
    }
    else Serial.print(" Empty message recieved. ");
  }
  
  // if(millis() - startTime >= 500){
  //     startTime = millis();
    if (!otherBoard.connected()) {
      otherBoard.connect(otherIP, PORT);
      Serial.print("Trying to connect to ");
      Serial.print(otherIP);
      Serial.print(":");
      Serial.println(PORT);
    }
    else{
      RJNet::sendData(otherBoard, "Ping");
      Serial.print("Sending data to ");
      Serial.print(otherBoard.remoteIP());
      Serial.print(":");
      Serial.println(otherBoard.remotePort());
    }
  // }

}
