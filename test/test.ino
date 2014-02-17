#include "variant.h"
#include <due_can.h>
#include "BmsRegisters.h"

#define CAN_BAUD_RATE CAN_BPS_250K

#define CUSTOM_1 0
#define CUSTOM_2 1

#define STD_VOLT			0x623
#define STD_CURRENT			0x624
#define STD_TEMP			0x627

#define STD_CHARGE			0
#define	STD_ENERGYFLOW		0
#define STD_RESISTANCE		0

void setup()
{
	Serial.begin(115200);

	if (CAN.init(CAN_BAUD_RATE)) {
	}
	else {
	Serial.println("CAN initialization (sync) ERROR");
	}
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

// // http://lithiumate.elithion.com/php/controller_can_specs.php#PID_messages
// void parseIndividualFrame(CAN_FRAME &frame) {
// 	switch (frame.id) {
// 	    case BATTVOLT:
// 	      	break;
// 	    case AVGTEMP:
// 	     	break;
// 	    case MAXTEMP:
// 	     	break;
// 	    case SOC:
// 	     	break;
// 	    case CCL:
// 	     	break;
// 	    case DCL:
// 	     	break;
// 	    default:
// 	    	break;
// 	}	
// }

//http://lithiumate.elithion.com/php/controller_can_specs.php#Standard
void parseStandardFrame(CAN_FRAME &frame) {
	switch (frame.id) {
	    case STD_VOLT:{
	    	Serial.print("Pack Voltage = ");
	    	Serial.print(frame.data.bytes[1],HEX);
	    	Serial.print(" ");
	    	Serial.println(frame.data.bytes[0],HEX);

	    	Serial.print("Minimum Voltage = ");
	    	Serial.print(frame.data.bytes[2],HEX);

	    	Serial.print("Maximum Voltage = ");
	    	Serial.print(frame.data.bytes[4],HEX);
	    	break;
	    }
	    case STD_CURRENT:{
	    	Serial.print("Current = ");
	    	Serial.print(frame.data.bytes[1],HEX);
	    	Serial.print(" ");
	    	Serial.println(frame.data.bytes[0],HEX);

	    	Serial.print("CCL = ");
	    	Serial.print(frame.data.bytes[2],HEX);
	    	Serial.print(" ");
	    	Serial.println(frame.data.bytes[3],HEX);

	    	Serial.print("DCL = ");
	    	Serial.print(frame.data.bytes[4],HEX);
	    	Serial.print(" ");
	    	Serial.println(frame.data.bytes[5],HEX);
	    	break;
	    }
	    case STD_TEMP:{
	    	Serial.print("Temperature = ");
	    	Serial.println(frame.data.bytes[0],HEX);

	    	Serial.print("Minimum Temperature = ");
	    	Serial.print(frame.data.bytes[2],HEX);

	    	Serial.print("Maximum Temperature = ");
	    	Serial.print(frame.data.bytes[4],HEX);
	    	break;
	    }
	    default:
	      // do something
	      break;
	}
}

// http://lithiumate.elithion.com/php/menu_setup.php#Custom_output_message
// depending on how the frame is customised
void parseCustomFrame(CAN_FRAME &frame) {
	switch (frame.id) {
		//Custom Frame 1 : information that changes more frequent
	    case CUSTOM_1:{
		    // data 0&1 = battery voltage (little endian) [022]
	    	Serial.print("Battery Voltage = ");
	    	Serial.print(frame.data.bytes[1],HEX);
	    	Serial.print(" ");
	    	Serial.println(frame.data.bytes[0],HEX);
		    // data 2&3 = battery current (little endian) [054]
		    Serial.print("Current = ");
	    	Serial.print(frame.data.bytes[3],HEX);
	    	Serial.print(" ");
	    	Serial.println(frame.data.bytes[2],HEX);
		    // data 4 = state of charge [112]
	    	Serial.print("SOC = ");
	    	Serial.println(frame.data.bytes[0],HEX);
		    break;	
	    }
	    case CUSTOM_2:{
		    // data 0 = average temp [096]
	    	Serial.print("Ave Temperature = ");
	    	Serial.println(frame.data.bytes[0],HEX);
		    // data 1 = max temp [104]
	    	Serial.print("Max Temperature = ");
	    	Serial.println(frame.data.bytes[1],HEX);
	    		//might not need 2 bytes for ccl and dcl 
		    // data 2&3 = dcl (little endian) [086] 
		    // data 4&5 = ccl (little endian) [078]
		      break;
	    }
	    default:
	      break;
	}
}

void loop()
{
	CAN_FRAME incoming;
	if (CAN.rx_avail()) {
		CAN.get_rx_buff(incoming); 
		printFrame(incoming);
	}
}