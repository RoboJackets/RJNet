
/*
 * Example of Arduino board using RJNet for ethernet TCP.
 * This also uses RJNet for serial communication.
 * created 20 Mar 2019
 * by Brian Cochran
 * 
 */

#include <Ethernet.h>
#include <RJNet.h> //make sure to install the RJNet library

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //the Wiznet 5500io chips have their own MAC, so use that
IPAddress ip(192, 168, 2, 2); //set the IP to find us at
int PORT = 7; //port we use to connect


EthernetServer server(PORT);
 
void setup() {
  Serial.begin(9600);
  
  // In case your RJ board wires the chip in an odd config,
  // otherwise, leave commented out
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields

  // initialize the ethernet device
  Ethernet.begin(mac, ip);

  //check that ethernet equipment is plugged in
  //#TODO

  // start listening for clients
  server.begin();


  Serial.print("Chat server address:");
  Serial.println(Ethernet.localIP());


  
}
double counter = 0;
void loop() {
  // wait for a new client:
  EthernetClient client = server.available();
  if (client) {
    String data = RJNet::readData(client);
    if (data.length() != 0) {
      Serial.println(data); //show us what we read      
      //do something useful with data here, maybe reply
      RJNet::sendData(client, "Message Received");
    }
  }

}
