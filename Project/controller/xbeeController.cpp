#include "xbeeController.h"

// Constructor
xbeeController::xbeeController()
{
    // nothing to construct
}

// Constructor
xbeeController::xbeeController(long SHaddress, long SLaddress)
{
    // Prepare serial connections
    Serial.begin(9600);
    xbee.setSerial(Serial);

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
xbeeController::~xbeeController()
{
    // nothing to destruct
}

// Send Data
void xbeeController::sendData(String data)
{
    char payload[data.length()];
    strcpy(payload, data.c_str());
    
    Tx64Request tx = Tx64Request(addr64, payload, sizeof(payload));

    xbee.send(tx);
    message = "Sent seq: " + String(sendID) + ", " + String(sizeof(payload)) + " bytes";
}

// Retransmit
bool xbeeController::retransmit(String data)
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
        return false;
    }
}

// Ack Sent Data
bool xbeeController::ackSentData(String data)
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
                message = "Error on seq: " + String(sendID) + ", the remote XBee did not receive our packet.";
                return retransmit(data);
            }
        }      
    }
    else if (xbee.getResponse().isError())
    {
        message = "Error on seq: " + String(sendID) + ", error reading packet. Error code: " + String(xbee.getResponse().getErrorCode());
        return retransmit(data);
    }
    else
    {
        // local XBee did not provide a timely TX Status Response.  Radio is not configured properly or connected
        message = "Error on seq: " + String(sendID) + ", local XBee did not provide a timely TX Status Response.";
        return retransmit(data);
    }
}

//// Receive Data
//bool xbeeController::receiveData()
//{
//    // continuously reads packets
//    xbee.readPacket();
//    
//    if (xbee.getResponse().isAvailable())
//    {
//        // got something
//            
//        if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
//        {
//            // got a rx packet
//            
//            xbee.getResponse().getZBRxResponse(rx64);
//            uint8_t option = rx64.getOption();
//
//            byte dataLength = rx64.getDataLength() + 1;
////            receiveBuffer = (char*)malloc(dataLength);
////            receiveBufferData = true;
////            
////            // read in each byte of the incoming data
////            for (int i = 0; i < dataLength - 1; i++)
////            {
////                receiveBuffer[i] = rx64.getData(i);
////            }
////            receiveBuffer[dataLength - 1] = 0;
//            
//            message = "Received seq: " + String(receiveID);
//
//            receiveID++;
//            return true;
//        }
//        else
//        {
//            // not something we were expecting
//            message = "Error on ackseq: " + String(receiveID) + ", ZB_RX_RESPONSE: format not expected";
//            return false;           
//        }       
//    }
//    else if (xbee.getResponse().isError())
//    {
//        message = "Error on ackseq: " + String(receiveID) + ", error reading packet. Error code: " + String(xbee.getResponse().getErrorCode());  
//        return false;      
//    }
//
//    // no data was available, radio is idle.
//    return false;
//}

//// Has Message
//bool xbeeController::hasMessage()
//{
//    return message.length() != 0;
//}

// Get Last Message
String xbeeController::getLastMessage()
{
    String output = message;
    message = "";
    return output;
}

//// Get Data
//char* xbeeController::getData()
//{
////    byte dataLength = sizeof(receiveBuffer);
////
////    char data[dataLength];
////    for (int i = 0; i < dataLength; i++)
////    {
////        data[i] = receiveBuffer[i];
////    }
////
////    clearReceiveBuffer();
//    char data[0];
//    return data;
//}

