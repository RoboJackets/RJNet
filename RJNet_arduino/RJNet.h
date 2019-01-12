#ifndef RJNet_h
#define RJNet_h

#include <Arduino.h>
#include "Stream.h"

class RJNet {
    public:
        RJNet();
        static void sendData(Stream &client, String msg);
        static String readData(Stream &client);
        const static char startMarker;
        const static char endMarker;
    private:
        static String addMarkersToMessage(String msg);
        static String formatMessage(String msg);
};


#endif
