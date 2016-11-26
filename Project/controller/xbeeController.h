#ifndef XBEE_CONTROLLER_H
#define XBEE_CONTROLLER_H

#include <arduino.h>
#include <XBee.h>

class xbeeController
{
public:
    xbeeController(int SHaddress, int SLaddress);
    ~xbeeController();

    void sendData(char* data);
    bool ackSentData();
    bool receiveData();
    bool hasMessage();
    bool hasData();
    String getLastMessage();
    char* getData();

private:
    void clearSendBuffer();
    void clearReceiveBuffer();
    void transmit();
    bool retransmit();
    
    // Radio library
    XBee xbee;

    // Sender globals
    XBeeAddress64 addr64;
    TxStatusResponse txStatus;
    const byte TOTAL_RETRANSMITS = 5;

    // Receiver globals
    XBeeResponse response;
    ZBRxResponse rx64;

    // Other globals
    String message;
    bool sendBufferData;
    bool receiveBufferData;
    char* sendBuffer;
    char* receiveBuffer;
    byte sendID;
    byte receiveID;
    byte retransmission;
};

#endif

