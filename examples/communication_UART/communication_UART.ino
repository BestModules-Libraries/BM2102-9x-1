/*****************************************************************
File:         communication_UART.ino
Description:  1.The same data is sent every 100 milliseconds RF for 3 packets, 
                and the transmitted data is sent cyclically from 0~255.     
***********************************************************/

#include "BM2102-9x-1.h"
BM2102_9x_1     BMC21(5,4); //rxPin=5,txPin=4,Please comment out this line of code if you don't use SW Serial
//BM2102_9x_1     BMC21; //Please uncomment out this line of code if you use HW Serial on BMduino
//BM2102_9x_1     BMC21(&Serial1); //Please uncomment out this line of code if you use HW Serial on BMduino
//BM2102_9x_1     BMC21(&Serial2); //Please uncomment out this line of code if you use HW Serial2 on BMduino
//BM2102_9x_1     BMC21(&Serial3); //Please uncomment out this line of code if you use HW Serial3 on BMduino
//BM2102_9x_1     BMC21(&Serial4); //Please uncomment out this line of code if you use HW Serial4 on BMduino

int LED = 13;                                                   
uint8_t TXDATA = 0;                                               
void setup()
{
  pinMode(LED, OUTPUT);                                 
  digitalWrite(LED, LOW);                                                        
  BMC21.begin(RF_433_92MHz);     // Initialize the module                                                                                                                                                                                                                                          
}

void loop()
{  
  if(!BMC21.isRFbusy()) //Determine if the keys are pressed and whether the RF is idle
  {
   BMC21.beginTransmission(TXDATA,3);  //Send a sum of data
   TXDATA++;
   digitalWrite(LED, HIGH);
   delay(10);
   digitalWrite(LED, LOW);
  } 
  delay(90);  
}
