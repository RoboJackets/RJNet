# EthernetSocket
---

## Initializing

The `EthernetSocket` provides an abstraction of `boost::ip::tcp` to ease communication with Ethernet devices. To initialize communication with another device via Ethernet, supply the device's ip address and port as constructor arguments:

```c++
std::string ip_addr = "192.168.1.20";
int port = 7;

EthernetSocket sock(ip_addr, port);
```

## Reading

The `EthernetSocket::readMessage()` method will read from the buffer and return its contents as a `std::string`:

```c++
// read buffer
std::string ret = sock.readMessage();
```

## Writing  

The `EthernetSocket::sendMessage()` method writes a message to the buffer:

```c++
std::string p_values = "#P30,30\n";
sock.sendMessage(p_values);
```
