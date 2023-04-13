/***********************************************************
File:           BM2102-9x-1.cpp
Author:         XIAO, BESTMODULES
Description:    UART/IIC communication with the BM2102_9x_1   
History:      
V1.0.0   -- initial version；2023-03-09；Arduino IDE : v1.8.16
***********************************************************/
#include "BM2102-9x-1.h"
/**********************************************************
Description: Constructor
Parameters:  *theSerial: Wire object if your board has more than one UART interface      
                         parameter range:&Serial、&Serial1、&Serial2、&Serial3、&Serial4
Return:          
Others:     
**********************************************************/
BM2102_9x_1::BM2102_9x_1(HardwareSerial *theSerial)
{
    _softSerial = NULL ;
    _wire = NULL;
    _hardSerial = theSerial;
    _ModeSlect = UART_MODE;
}

/**********************************************************
Description: Constructor
Parameters:  *theSerial: Wire object if your board has more than one IIC interface      
                         parameter range:&wire、&wire1、&wire2
Return:          
Others:     
**********************************************************/
BM2102_9x_1::BM2102_9x_1(TwoWire *theWire)
{
    _hardSerial = NULL;
    _softSerial = NULL ;
    _wire = theWire;
    _ModeSlect = IIC_MODE;
}

/**********************************************************
Description: Constructor
Parameters:  rxPin : Receiver pin of the UART
             txPin : Send signal pin of UART         
Return:          
Others:   
**********************************************************/
BM2102_9x_1::BM2102_9x_1(uint16_t rxPin,uint16_t txPin)
{
    _hardSerial = NULL;
    _wire = NULL;
    _rxPin = rxPin;
    _txPin = txPin;
    _softSerial = new SoftwareSerial(_rxPin,_txPin);
    _ModeSlect = UART_MODE;
}

/**********************************************************
Description: Initialize mode
Parameters:  frequencyBand: 
                           RF_315MHz：325MHZ
                           RF_433_92MHz：433MHz
                           RF_868MHz：868MHZ
                           RF_915MHz：915MHZ     
Return:          
Others:      If the _softSerial pointer is empty, the software serial port is initialized,
             otherwise the hardware serial port is initialized.      
**********************************************************/
void BM2102_9x_1::begin(uint8_t frequencyBand)
{
    float delaytime;
    if(_ModeSlect == UART_MODE)
    {
      if(_softSerial != NULL)
      {
        _softSerial->begin(19200);
        delaytime = (60000/19200)+100;
        _softSerial->setTimeout((uint8_t)delaytime);
      }
      else
      {
        _hardSerial->begin(19200);
        delaytime = (60000/19200)+100;
        _hardSerial->setTimeout((uint8_t)delaytime);
      }
    } 
   else if(_ModeSlect == IIC_MODE)
    {
      if(_wire != NULL)
      {
        _wire->begin();
        _wire->setClock(100000);
      }
    }
  setRFFrequencyBand(frequencyBand);
}

/**********************************************************
Description: RF starts to transmit a uint8_t of data
Parameters:  times:Number of transmission(0x00~0xff)
                   0x00:Unlimited launches
             data:One uint8_t of data(0x00~0xff) 
Return:          
Others:      1.The command execution time is 3ms.
             Run other commands after the command execution is complete   
             2. If times = 0x00, call the beginTransmission() function
                to stop an infinite number of launches. 
**********************************************************/
void BM2102_9x_1::beginTransmission(uint8_t data,uint8_t times)
{
    uint8_t sendBuf[3] = {0x23, 0x00,0x00};
    sendBuf[1] = times;
    sendBuf[2] = data;
    writeBytes(sendBuf,3);
}

/**********************************************************
Description: Stop RF data transmission
Parameters:             
Return:          
Others:      The command execution time is 3ms.
             Run other commands after the command execution is complete     
**********************************************************/
void BM2102_9x_1::endTransmission(void)
{
    uint8_t sendBuf[1] = {0x00};
    writeBytes(sendBuf,1);
}


/**********************************************************
Description:  Determine if RF is busy
Parameters:  
Return:      true: RF is  busy 
             false:RF not  busy        
Others:         
**********************************************************/
bool BM2102_9x_1::isRFbusy(void)
{
    uint8_t sendBuf[1] = {0x81};
    uint8_t buff[1] = {0};
    writeBytes(sendBuf,1);
    if(readBytes(buff,1)== SUCCESS)
    {
     if(!(buff[0] & 0x08)) 
     {
      return false; 
      delay(20);   
     }                             
     else
     { 
       delay(20);
       return true; 
     }
   } 
}

/**********************************************************
Description: Get the version information 
Parameters:  
Return:      result[1]<<8+result[0]:0b1100000000     
Others:         
**********************************************************/
uint16_t  BM2102_9x_1::getFWVer(void)
{
    uint8_t sendBuf[1] = {0x90};
    uint8_t buff[2] = {0};
    writeBytes(sendBuf,1);
    if(readBytes(buff,2)== SUCCESS)
    {
     return (buff[1]<<8+buff[0]);
    } 
}

/**********************************************************
Description: Configure the RF transmit power
Parameters:  powervalue:
                        RF_0dBm_power:0dBm
                        RF_5dBm_power:5dBm
                        RF_10dBm_power:10dBm
                        RF_13dBm_power:13dBm 
Return:          
Others:      The command execution time is 5ms.
             Run other commands after the command execution is complete     
**********************************************************/
void BM2102_9x_1::setRFPower(uint8_t powerValue)
{
    uint8_t sendBuf[2] = {0x12, 0x00};
    sendBuf[1] = powerValue;
    writeBytes(sendBuf,2);
    delay(10); 
}

/**********************************************************
Description: Configuring the RF Frequency Band
Parameters:  frequencyBand：
                           RF_315MHz：325MHZ
                           RF_433_92MHz：433MHz 
                           RF_868MHz：868MHZ
                           RF_915MHz：915MHZ
Return:          
Others:      The command execution time is 50ms.
             Run other commands after the command execution is complete     
**********************************************************/
void BM2102_9x_1::setRFFrequencyBand(uint8_t frequencyBand)
{
    uint8_t sendBuf[2] = {0x10, 0x00};
    sendBuf[1] = frequencyBand;
    writeBytes(sendBuf,2);
    delay(62);  
}

/**********************************************************
Description: writeBytes
Parameters:  wbuf[]:Variables for storing Data to be sent
             wlen:Length of data sent  
Return:   
Others:
**********************************************************/
void BM2102_9x_1::writeBytes(uint8_t wbuf[], uint8_t wlen)
{
 if(_ModeSlect==IIC_MODE)
 {
  if (_wire != NULL)
  {
    while (_wire->available() > 0)
    {
      _wire->read();
    }
    _wire->beginTransmission(BM2102_9x_1_Addr);
    _wire->write(wbuf, wlen);
    _wire->endTransmission();
  }
 }
 else if(_ModeSlect==UART_MODE)
 {
  /* Select SoftwareSerial Interface */
  if (_softSerial != NULL)
  {
    while (_softSerial->available() > 0)
    {
      _softSerial->read();
    }
    _softSerial->write(wbuf, wlen);
  }
  /* Select HardwareSerial Interface */
  else
  {
    while (_hardSerial->available() > 0)
    {
      _hardSerial->read();
    }
    _hardSerial->write(wbuf, wlen);
  }
 }
}

/**********************************************************
Description: readBytes
Parameters:  rbuf[]:Variables for storing Data to be obtained
             rlen:Length of data to be obtained
Return:   
Others:
**********************************************************/
uint8_t BM2102_9x_1::readBytes(uint8_t rbuf[], uint8_t rlen, uint16_t timeOut)
{
  uint8_t i = 0, delayCnt = 0;
  /****** IIC MODE ******/
  if(_ModeSlect==IIC_MODE)
  {
    _wire->requestFrom(BM2102_9x_1_Addr, rlen);
    if (_wire->available() == rlen)
   {
     for (i = 0; i < rlen; i++)
     {
      rbuf[i] = _wire->read();
     }
   }
   else
   {
    return TIMEOUT_ERROR;
   }
 }
 
/****** UART MODE ******/
 else if(_ModeSlect==UART_MODE)
{
  if (_softSerial != NULL)
  {
    for (i = 0; i < rlen; i++)
    {
      delayCnt = 0;
      while (_softSerial->available() == 0)
      {
        if (delayCnt > timeOut)
        {
          return TIMEOUT_ERROR; // Timeout error
        }
        delay(1);
        delayCnt++;
      }
      rbuf[i] = _softSerial->read();
    }
  }

  else
  {
    for (i = 0; i < rlen; i++)
    {
      delayCnt = 0;
      while (_hardSerial->available() == 0)
      {
        if (delayCnt > timeOut)
        {
          return TIMEOUT_ERROR; // Timeout error
        }
        delay(1);
        delayCnt++;
      }
      rbuf[i] = _hardSerial->read();
    }
  }
 }
  return SUCCESS;
}
