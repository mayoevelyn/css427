#ifndef XBEE_CONTROLLER_H
#define XBEE_CONTROLLER_H

#include <XBee.h>

class xbeeController
{
public:
    xbeeController();
    xbeeController(long SHaddress, long SLaddress);
    ~xbeeController();

    void sendData(String payload);
    bool ackSentData(String payload);
    //bool receiveData();
    //bool hasMessage();
    String getLastMessage();
    //char* getData();

private:
    bool retransmit(String payload);
    
    // Radio library
    XBee xbee;

    // Sender globals
    XBeeAddress64 addr64;
    TxStatusResponse txStatus;
    byte TOTAL_RETRANSMITS = 5;

    // Receiver globals
    XBeeResponse response;
    ZBRxResponse rx64;

    // Other globals
    String message;
    byte sendID;
    byte receiveID;
    byte retransmission;
};

#endif

