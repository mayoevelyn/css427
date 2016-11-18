#include "xbeeController.h"

// Constructor
xbeeController::xbeeController()
{
    // Prepare serial connections
    Serial.begin(9600);
    xbee.setSerial(Serial);

    // Initialize radio object
    xbee = XBee();

    // Sender globals
    // 64-bit addressing: This is the SH + SL address of remote XBee
    addr64 = XBeeAddress64(0x0013A200, 0x40E3CD0F);
    txStatus = TxStatusResponse();

    // Receiver globals
    // Create reusable response objects for responses we expect to handle 
    response = XBeeResponse();
    rx64 = ZBRxResponse();

    // Globals
    sequence = 0;
    ackSequence = 0;
}

// Destructor
xbeeController::~xbeeController()
{
    // nothing to destruct
}

// Send Data
void xbeeController::sendData(String data)
{
    // transmit payload or retransmit as necessary
    char payload[data.length()];
    strcpy(payload, data.c_str());

    Tx64Request tx = Tx64Request(addr64, payload, sizeof(payload));

    xbee.send(tx);
    message = "Sent seq: " + String(sequence) + ", " + data;
}

// Ack Sent Data
bool xbeeController::ackSentData()
{
    // verify the payload was sent
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
                sequence++;
                return true;
            }
            else
            {
                // the remote XBee did not receive our packet. is it powered on?
                message = "Error on seq: " + String(sequence) + ", the remote XBee did not receive our packet.";
                return false;
            }
        }      
    }
    else if (xbee.getResponse().isError())
    {
        message = "Error on seq: " + String(sequence) + ", error reading packet. Error code: " + String(xbee.getResponse().getErrorCode());
        return false;  
    }
    else
    {
        // local XBee did not provide a timely TX Status Response.  Radio is not configured properly or connected
        message = "Error on seq: " + String(sequence) + ", local XBee did not provide a timely TX Status Response.";
        return false;
    }
}

// Receive Data
bool xbeeController::receiveData()
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

            int buffSize = rx64.getDataLength() + 1;
            char data[buffSize];
            
            // read in each byte of the incoming data
            for (int i = 0; i < buffSize - 1; i++)
            {
                data[i] = rx64.getData(i);
            }
            data[buffSize - 1] = 0;
            
            String eval = String(data);
            message = "Received ackseq: " + String(ackSequence) + ", " + eval;

            ackSequence++;
            return true;
        }
        else
        {
            // not something we were expecting
            message = "Error on ackseq: " + String(ackSequence) + ", ZB_RX_RESPONSE: format not expected";
            return false;           
        }       
    }
    else if (xbee.getResponse().isError())
    {
        message = "Error on ackseq: " + String(ackSequence) + ", error reading packet. Error code: " + String(xbee.getResponse().getErrorCode());  
        return false;      
    }
}

// Get Last Message
String xbeeController::getLastMessage()
{
    String output = message;
    message = "";
    return output;
}

