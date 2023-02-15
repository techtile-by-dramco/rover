#include "SerialComm.h"

// ================================================================
// ===                Static void functions                     ===
// ================================================================
uint8_t calculateCS(uint8_t *data, uint8_t len);
void write_array(uint8_t * data, uint8_t len);


uint8_t read_value_from_buffer(uint32_t timeout, uint8_t * readout);

// ================================================================
// ===                      Functions                           ===
// ================================================================

SerialComm::SerialComm(){
}



uint8_t SerialComm::incoming_dataframe_handler(uint8_t *rsvbuf, uint8_t *total_len){
  if(SER.available() > 0){
    if(SER.read() == 0x02){
      *(rsvbuf + 0) = 0x02;
      read_value_from_buffer(10, &rsvbuf[1]);
      read_value_from_buffer(10, &rsvbuf[2]);
      uint16_t len = rsvbuf[2];

      //  The incoming data
      if(len < 255){
        uint8_t i;
        for(i = 0; i < len; i++){
          if(SER.available() > 0){
            read_value_from_buffer(10, &rsvbuf[3 + i]);
          }
          else break;
        }
        #ifdef CHECKSUM
          if(i == len){
            *total_len = len + 3;
            if(calculateCS(rsvbuf, total_len[0]) == SER.read()) {   return 1;   }
            else                                                {   return 0;   }
          }
        #else
          //for(uint8_t i = 0; i < 10; i++){if(SER.available() > 0){SER.flush();}}
          return 1;
        #endif
      }
    }
  }
  return 0;
}


uint8_t read_value_from_buffer(uint32_t timeout, uint8_t * readout){
  uint32_t start_time = millis();
  uint8_t status = 1;

  while(millis() - start_time < timeout){
    if(SER.available() > 0){
      *readout = SER.read();
      break;
    }
    else{
      status = 0;
    }
  }
  return status;
}


// ================================================================
// ===                    TX Communication                      ===
// ================================================================

void SerialComm::transmitFrame(uint8_t cmd, uint8_t len, uint8_t *data){
    uint8_t tx_buffer [len + 4];
    tx_buffer [0] = 0x02;
    tx_buffer [1] = cmd;
    tx_buffer [2] = (uint8_t)len;
    for(uint8_t i = 0; i < len; i++){
        tx_buffer [i + 3] = *(data + i);
    }
    
    write_array(tx_buffer, (3 + len));
}

void SerialComm::transmitFrame(uint8_t cmd, uint8_t data){
    uint8_t tx_buffer [5];
    tx_buffer [0] = 0x02;
    tx_buffer [1] = cmd;
    tx_buffer [2] = 0x01;
    tx_buffer [3] = data;
    write_array(tx_buffer, 4);
}

void SerialComm::transmitFrame(uint8_t cmd){
    uint8_t tx_buffer [5];
    tx_buffer [0] = 0x02;
    tx_buffer [1] = cmd;
    tx_buffer [2] = 0x00;
    write_array(tx_buffer, 3);
}


// ================================================================
// ===                Static void functions                     ===
// ================================================================

void write_array(uint8_t * data, uint8_t len){
    uint8_t cs = calculateCS(data, len);
    SER.write(data, len);
    SER.write(cs);
}

uint8_t calculateCS(uint8_t * data, uint8_t len){
    uint8_t checksum = *(data);
    for(uint8_t i = 1; i < len; i++){
        checksum = checksum ^ *(data + i);
    }
    return checksum;
}