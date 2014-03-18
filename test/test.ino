#include "variant.h"
#include <due_can.h>

#define CAN_BAUD_RATE CAN_BPS_500K

CAN_FRAME frame1,incoming;

void setup()
{
  Serial.begin(115200);

  if (CAN.init(CAN_BAUD_RATE) &&
    CAN2.init(CAN_BAUD_RATE)) {
  }
  else {
  Serial.println("CAN initialization (sync) ERROR");
  }

  //sets the receiving filter to only receive from 0x622 to 0x630

  int filter;
  //extended
  for (filter = 0; filter < 3; filter++) {
  CAN.setRXFilter(filter, 0, 0, true);
  CAN2.setRXFilter(filter, 0, 0, true);
  }  
  //standard
  for (int filter = 3; filter < 7; filter++) {
  CAN.setRXFilter(filter, 0x622, 0x1FFFFFF0, false);
  CAN2.setRXFilter(filter, 0x622, 0x1FFFFFF0, false);
  }  

  // frame for PID request requesting PID(F5)
  // frame1.id = 0x07E0;
  // frame1.extended = 0;
  // frame1.length = 8;
  // frame1.data.bytes[0] = 0x02;
  // frame1.data.bytes[1] = 0x21;
  // frame1.data.bytes[2] = 0xF5;
  // frame1.data.bytes[3] = 0x00;
  // frame1.data.bytes[4] = 0x00;
  // frame1.data.bytes[5] = 0x00;
  // frame1.data.bytes[6] = 0x00;
  // frame1.data.bytes[7] = 0x00;

  // CAN.sendFrame(frame1);
  // CAN2.sendFrame(frame1);
  // Serial.println("Frame Sent!");
  // printFrame(frame1);
}

void printFrame(CAN_FRAME &frame) {
   Serial.print("ID: 0x");
   Serial.print(frame.id, HEX);
   Serial.print(" Len: ");
   Serial.print(frame.length);
   Serial.print(" Data: 0x");
   for (int count = 0; count < frame.length; count++) {
       Serial.print(frame.data.bytes[count], HEX);
       Serial.print(" ");
   }
   Serial.print("\r\n");
}

//parses the standard frame sent by BMS every 1 second
void parseFrame(CAN_FRAME &frame){
  switch (frame.id) {
      //state
      case 0x622:
        Serial.print("State = ");
        Serial.print(frame.data.bytes[0],HEX);
        Serial.print("\n");
        break;
      //pack voltage
      case 0x623:
        Serial.print("Pack Voltage = ");
        Serial.print(frame.data.bytes[0],HEX);
        Serial.print(frame.data.bytes[1],HEX);
        Serial.print("V");
        Serial.print("\n");
        break;
      //current
      case 0x624:
        Serial.print("Current = ");
        Serial.print(frame.data.bytes[0],HEX);
        Serial.print(frame.data.bytes[1],HEX);
        Serial.print("A");
        Serial.print("\n");
        break;
      //SOC
      case 0x626:
        Serial.print("SOC = ");
        Serial.print(frame.data.bytes[0]);
        Serial.print("%");
        Serial.print("\n");
        break;
      //temperature
      case 0x627:
        Serial.print("Temperature = ");
        Serial.print(frame.data.bytes[0],HEX);
        Serial.print("C");
        Serial.print("\n");
        break;
      default:
        // do something
        break;
  }
}


void loop()
{
    CAN_FRAME incoming;

    if (CAN.rx_avail()) {
      CAN.get_rx_buff(incoming);
      // Serial.print("CAN1 = "); 
      // printFrame(incoming);
      parseFrame(incoming);
    }
}