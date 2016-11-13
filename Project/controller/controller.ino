#include <XBee.h>
#include <SoftwareSerial.h>

// Initialize radio object
XBee xbee = XBee();

// Setup device interconnect over serial
SoftwareSerial mySerial(64, 65); // (A10 - blue)RX, (A11 - green)TX

// Sender globals
// 64-bit addressing: This is the SH + SL address of remote XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x40E3CD0F);
TxStatusResponse txStatus = TxStatusResponse();

// Receiver globals
// Create reusable response objects for responses we expect to handle 
XBeeResponse response = XBeeResponse();
ZBRxResponse rx64 = ZBRxResponse();

// Globals
unsigned int sequence = 0;
unsigned int ackSequence = 0;

const int interval = 6000;
unsigned long previousMillis = 0;

bool toggle = false; // diagnostic toggle

// Setup
void setup()
{
    // Prepare serial connections
    Serial.begin(9600);
    xbee.setSerial(Serial);

    // Set the data rate for the SoftwareSerial port
    mySerial.begin(4800);  
    
    // Allow radio to fully boot and establish connection to remote
    mySerial.println("Started booting Arduino Mega");
    delay(10000);
    mySerial.println("Finished booting Arduino Mega");
}

// Loop
void loop()
{
    // Get snapshot of time
    unsigned long currentMillis = millis();

    // How much time has passed, accounting for rollover with subtraction
    if ((unsigned long)(currentMillis - previousMillis) >= interval)
    {
        sendData();
        ackSentData();

        // Use the snapshot to set track time until next event
        previousMillis = currentMillis;
    }
    
    receiveData();
}

// Send Data
void sendData()
{
    // transmit payload or retransmit as necessary
    String message = getMessage();
    char payload[message.length()];
    strcpy(payload, message.c_str());

    Tx64Request tx = Tx64Request(addr64, payload, sizeof(payload));

    xbee.send(tx);
    mySerial.println("Sent seq: " + String(sequence) + ", " + message);
}

// Ack Sent Data
void ackSentData()
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
            }
            else
            {
                // the remote XBee did not receive our packet. is it powered on?
                mySerial.println("Error on seq: " + String(sequence) + ", the remote XBee did not receive our packet.");
            }
        }      
    }
    else if (xbee.getResponse().isError())
    {
        mySerial.print("Error on seq: " + String(sequence) + ", error reading packet. Error code: ");  
        mySerial.println(xbee.getResponse().getErrorCode());    
    }
    else
    {
        // local XBee did not provide a timely TX Status Response.  Radio is not configured properly or connected
        mySerial.println("Error on seq: " + String(sequence) + ", local XBee did not provide a timely TX Status Response.");
    }
}

// Receive Data
void receiveData()
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
            mySerial.println("Received ackseq: " + String(ackSequence) + ", " + eval);

            ackSequence++;
            toggle = !toggle;
        }
        else
        {
            // not something we were expecting
            mySerial.println("Error on ackseq: " + String(ackSequence) + ", ZB_RX_RESPONSE: format not expected");           
        }       
    }
    else if (xbee.getResponse().isError())
    {
        mySerial.print("Error on ackseq: " + String(ackSequence) + ", error reading packet. Error code: ");  
        mySerial.println(xbee.getResponse().getErrorCode());        
    }
}

// Get Message
String getMessage()
{
    if (toggle)
    {
        return "1.1,2.2,3.3";
    }
    else
    {
        return "2500,2500";
    }
}

