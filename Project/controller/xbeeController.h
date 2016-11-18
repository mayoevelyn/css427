#ifndef XBEE_CONTROLLER_H
#define XBEE_CONTROLLER_H

#include <XBee.h>

class xbeeController
{
public:
    xbeeController();
    ~xbeeController();

    void sendData(String data);
    bool ackSentData();
    bool receiveData();
    String getLastMessage();

private:
    // Radio library
    XBee xbee;

    // Sender globals
    XBeeAddress64 addr64;
    TxStatusResponse txStatus;

    // Receiver globals
    XBeeResponse response;
    ZBRxResponse rx64;

    // Other globals
    String message;
    unsigned int sequence;
    unsigned int ackSequence;
};

#endif

