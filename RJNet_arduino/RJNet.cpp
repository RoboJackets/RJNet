#include "RJNet.h"

/*
    Class for handling RoboJackets networking communications (Serial and Ethernet) in a safe manner.
    Utilizes Arduino's Stream interface to allow Serial, Ethernet TCP, and any future communication methods.
    Ensures that all messages are sent with a start and end marker to get rid of debug/accidental messages
    and to ensure we get a full message before working on it.
    Messages are sent/received with spaces between data points like <startMarker>data0 data1 data2<endMarker>
    Example: $10.0 2.1 3.0 143;
*/

const char RJNet::startMarker = '$';
const char RJNet::endMarker = ';';

void RJNet() {}

/*
 *  Receives data from client you are connected to in a safe manner.
 *  @Note: not named read() to limit confusion with Arduino Stream library interface
 *
 *  client: Serial, ethernet client or server, other communication method
 */
String RJNet::readData(Stream &client) {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char rc;
    const uint8_t numChars = 128; //buffer size
    char receivedChars[numChars]; //input buffer
    boolean newData = false;

    while (client.available() > 0 && newData == false) {
        rc = client.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }

    if (newData) {
      return receivedChars;
    } else {
      return "";
    }
}

/*
 * Sends info to client (Serial, Ethernet, etc output)
 * @Note: not named print() to limit confusion with Arduino Stream library interface
 * client: Serial, ethernet client or server, other communication method
 * msg: Message to send
 */
void RJNet::sendData(Stream &client, String msg) {
    client.print(addMarkersToMessage(msg));
}

/*
 * Formats String message to have start and end markers.
 * msg: Message to send
 */
String RJNet::addMarkersToMessage(String msg) {
    //add markers
    if (msg.charAt(0) != startMarker) {
        msg = startMarker + msg;
    }
    if (msg.charAt(msg.length() - 1) != endMarker) {
        msg = msg + endMarker;
    }

    return msg;
}
