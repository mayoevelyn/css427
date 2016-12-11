// ----------------------------------------------------------------------------
// CSS 427 - Embedded Systems
// Author:  Thomas Dye, John Walter - 12/9/2016
// School:  University of Washington | Bothell
// Instructor:  Dr. Peng Yang
//
// Controller  - XBee Radio Wrapper
// ----------------------------------------------------------------------------

#include "XBeeController.h"

// Constructor
XBeeController::XBeeController()
{
    // nothing to construct
}

// Constructor
XBeeController::XBeeController(long SHaddress, long SLaddress, SoftwareSerial *object)
{
    // Prepare serial connections
    Serial.begin(9600);
    xbee.setSerial(Serial);
    mySerial = object;

    // Initialize radio object
    xbee = XBee();
    
    // Sender globals
    // 64-bit addressing: This is the SH + SL address of remote XBee
    addr64 = XBeeAddress64(SHaddress, SLaddress);
    txStatus = TxStatusResponse();

    // Receiver globals
    // Create reusable response objects for responses we expect to handle 
    response = XBeeResponse();
    rx64 = ZBRxResponse();

    // Globals
    sendID = 0;
    receiveID = 0;
    retransmission = 0;
}

// Destructor
XBeeController::~XBeeController()
{
    // nothing to destruct
}

// Send Data
void XBeeController::sendData(String data)
{
    char payload[data.length()];
    strcpy(payload, data.c_str());
    
    Tx64Request tx = Tx64Request(addr64, payload, sizeof(payload));

    xbee.send(tx);
    mySerial->print(F("Sent seq: "));
    mySerial->print(sendID);
    mySerial->print(F(", data: "));
    mySerial->println(data);
}

// Retransmit
bool XBeeController::retransmit(String data)
{
    if (retransmission < TOTAL_RETRANSMITS)
    {
        sendData(data);
        retransmission++;
        return ackSentData(data);
    }
    else
    {
        retransmission = 0;
        mySerial->print(F("Error on seq: "));
        mySerial->print(sendID);
        mySerial->println(F(", maximum number of retransmissions exceeded."));
        return false;
    }
}

// Ack Sent Data
bool XBeeController::ackSentData(String data)
{
    // block and verify the payload was sent
    // after sending a tx request, we expect a status response
    // wait up to 5 seconds for the status response
    if (xbee.readPacket(5000))
    {
        // got a response!
        
        // should be a znet tx status                
        if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE)
        {
            xbee.getResponse().getTxStatusResponse(txStatus);
            
            // get the delivery status, the fifth byte
            if (txStatus.getStatus() == SUCCESS)
            {
                // success.  the remote radio reported delivery of the message
                sendID++;
                retransmission = 0;
                return true;
            }
            else
            {
                // the remote XBee did not receive our packet. is it powered on?
                mySerial->print(F("Error on seq: "));
                mySerial->print(sendID);
                mySerial->println(F(", the remote XBee did not receive our packet."));
                return retransmit(data);
            }
        }      
    }
    else if (xbee.getResponse().isError())
    {
        mySerial->print(F("Error on seq: "));
        mySerial->print(sendID);
        mySerial->print(F(", error reading packet. Error code: "));
        mySerial->println(xbee.getResponse().getErrorCode());
        return retransmit(data);
    }
    else
    {
        // local XBee did not provide a timely TX Status Response.  Radio is not configured properly or connected
        mySerial->print(F("Error on seq: "));
        mySerial->print(sendID);
        mySerial->println(F(", local XBee did not provide a timely TX Status Response."));
        return retransmit(data);
    }
}

// Receive Data
String XBeeController::receiveData()
{
    // continuously reads packets
    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable())
    {
        // got something
            
        if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
        {
            // got a rx packet
            
            xbee.getResponse().getZBRxResponse(rx64);
            uint8_t option = rx64.getOption();

            byte dataLength = rx64.getDataLength() + 1;
            char data[dataLength];
            
            // read in each byte of the incoming data
            for (int i = 0; i < dataLength - 1; i++)
            {
                data[i] = rx64.getData(i);
            }
            data[dataLength - 1] = 0;

            String payload = String(data);
            mySerial->print(F("Received seq: "));
            mySerial->print(receiveID);
            mySerial->print(F(", data: "));
            mySerial->println(payload);
            receiveID++;
            return payload;
        }
        else
        {
            // not something we were expecting
            mySerial->print(F("Error on received seq: "));
            mySerial->print(receiveID);
            mySerial->println(F(", ZB_RX_RESPONSE: format not expected"));
            return "";           
        }       
    }
    else if (xbee.getResponse().isError())
    {
        mySerial->print(F("Error on received seq: "));
        mySerial->print(receiveID);
        mySerial->print(F(", error reading packet. Error code: "));
        mySerial->println(xbee.getResponse().getErrorCode());  
        return "";      
    }

    // no data was available, radio is idle.
    return "";
}

