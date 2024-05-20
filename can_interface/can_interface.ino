#include <mcp_can.h>
#include <SPI.h>

#define CS_PIN                  5 
#define ENCODER_PIN            34
#define DATA_LENGTH             8
#define STRING_LENGTH         128 
#define NO_MESSAGE_ID        9999

MCP_CAN CAN0(CS_PIN);  

void intToBytes(unsigned long long integer, unsigned char *data, unsigned short data_length)
{
  for (int i = 0; i < data_length; i++)
    data[(data_length-1) - i] = (integer >> (i * 8)) & 0xFF;
}

void speed_command()
{
  int identifier = 0x010;
  unsigned char data[DATA_LENGTH] = {'0','0','0','0','0','0','0','0'};
  
  unsigned short encoderValue = analogRead(ENCODER_PIN);
//Serial.println(String(encoderValue));

  intToBytes(encoderValue, data, DATA_LENGTH);
//for(int i=0;i<DATA_LENGTH;i++)
//Serial.println(data[i],HEX); 
    
  // send Standard CAN Frame
  byte sndStat = CAN0.sendMsgBuf(identifier, 0, DATA_LENGTH,data);
  
//if(sndStat == CAN_OK)
 //  Serial.println("Message Sent Successfully!");
 // else 
   //Serial.println("Error Sending Message...");
}

void receive_messages()
{
  long unsigned int rxId = NO_MESSAGE_ID;
  unsigned char len = 0;
  unsigned char rxBuf[DATA_LENGTH]= {'0','0','0','0','0','0','0','0'};
  char msgString[STRING_LENGTH] = "";                       

  CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
  if (rxId != NO_MESSAGE_ID)
  {
    Serial.print(String(rxId,HEX) + " " + "[" + String(len) + "] "); 
    for(int i = 0; i<len; i++)
    {
      // sprintf(msgString, " 0x%.2X", rxBuf[i]);
      Serial.print(rxBuf[i],HEX);
      Serial.print(" ");
      //  Serial.print(" ");
    }
    Serial.println();
  }
}

void setup()
{
  Serial.begin(115200);

  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
 
  else 
    Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted
}

void loop()
{
  receive_messages();
  speed_command();
  delay(1000);   // send data per 100ms
}
