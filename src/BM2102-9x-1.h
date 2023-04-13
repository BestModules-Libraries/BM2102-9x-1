/***********************************************************
File:               BM2102-9x-1.h
Author:             XIAO, BESTMODULES
Description:        Define classes and required variables
History:      
V1.0.0   -- initial version；2023-03-09；Arduino IDE : v1.8.16
***********************************************************/
#ifndef _BM2102_9x_1_H__
#define _BM2102_9x_1_H__

#include "Arduino.h"
#include <Wire.h> 
#include <SoftwareSerial.h>

#define   SUCCESS            0
#define   FAIL               1
#define   TIMEOUT_ERROR   3

#define   IIC_MODE            0x01
#define   UART_MODE           0x02

const uint8_t BM2102_9x_1_Addr = 0x21;    // I2C address

#define   RF_315MHz           0x00
#define   RF_433_92MHz        0x01
#define   RF_868MHz           0x02
#define   RF_915MHz           0x03

#define   RF_0dBm_power       0x01
#define   RF_5dBm_power       0x05
#define   RF_10dBm_power      0x09
#define   RF_13dBm_power      0x0D


class BM2102_9x_1
{     
   public:
          BM2102_9x_1(HardwareSerial *theSerial);
          BM2102_9x_1(uint16_t rxPin,uint16_t txPin);
          BM2102_9x_1(TwoWire *theWire = &Wire);
          void begin(uint8_t frequencyBand);              // While UART_MODE Initialize UART port and set buad-rate   
          void beginTransmission(uint8_t data,uint8_t times);                       // Set RF data and resend times, then start to TX
          void endTransmission(void);    // Stop RF TX
          bool isRFbusy(void);                                     // 
          uint16_t  getFWVer(void);                            // Read RF version         
          void setRFPower(uint8_t powerValue);                           // Set RF output power                                           
  private:
          void setRFFrequencyBand(uint8_t frequencyBand);                     // Set RF frequency band
          void writeBytes(uint8_t wbuf[], uint8_t wlen);
          uint8_t readBytes(uint8_t rbuf[], uint8_t rlen, uint16_t timeOut = 10);
         
          uint16_t _rxPin;
          uint16_t _txPin;
          uint8_t _ModeSlect;
          
          HardwareSerial *_hardSerial = NULL;
          SoftwareSerial *_softSerial = NULL ;
          TwoWire        *_wire = NULL;
};

#endif
