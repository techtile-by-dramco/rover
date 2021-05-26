#include "Uart.h"

// ================================================================
// ===                Static void functions                     ===
// ================================================================
uint8_t calculateCS(uint8_t *data, uint8_t len);

void write_array(uint8_t * data, uint8_t len);
void write_array_led(uint8_t * data, uint8_t len, uint8_t led);

// ================================================================
// ===                      Functions                           ===
// ================================================================

Uart::Uart(){
}

uint8_t Uart::incoming_dataframe_handler(uint8_t *rsvbuf, uint16_t *total_len){
  if(Serial.available() > 0){
    digitalWrite(LED_COMM, HIGH);
    //uint8_t rsvbuf [100];
    if(Serial.read() == 0x02){
      *(rsvbuf + 0) = 0x02;
      delayMicroseconds(200);
      *(rsvbuf + 1) = Serial.read();
      *(rsvbuf + 2) = Serial.read();
      uint16_t len = rsvbuf[2];

      delayMicroseconds(1000);

      //  The incoming data
      if(len < 255){
        uint8_t i;
        for(i = 0; i < len; i++){
          if(Serial.available() > 0){
            *(rsvbuf + 3 + i) = Serial.read();
          }
          else break;
        }
        #ifdef CHECKSUM
          if(i == len){
            *total_len = len + 3;
            if(calculateCS(rsvbuf, total_len) == Serial.read()) {   return 1;   }
            else                                                {   return 0;   }
          }
        #else
          return 1;
        #endif
      }
    }
  }
  return 0;
}


// ================================================================
// ===                    TX Communication                      ===
// ================================================================

void Uart::transmitFrame(uint8_t cmd, uint16_t len, uint8_t *data){
    uint8_t tx_buffer [len + 4];
    tx_buffer [0] = 0x02;
    tx_buffer [1] = cmd;
    tx_buffer [2] = (uint8_t)len;
    for(uint8_t i = 0; i < len; i++){
        tx_buffer [i + 3] = *(data + i);
    }
    
    write_array_led(tx_buffer, (4 + len), LED_COMM);
}

void Uart::transmitFrame(uint8_t cmd, uint8_t data){
    uint8_t tx_buffer [5];
    tx_buffer [0] = 0x02;
    tx_buffer [1] = cmd;
    tx_buffer [2] = 0x01;
    tx_buffer [4] = data;
    write_array_led(tx_buffer, 4, LED_COMM);
}

void Uart::transmitFrame(uint8_t cmd){
    uint8_t tx_buffer [5];
    tx_buffer [0] = 0x02;
    tx_buffer [1] = cmd;
    tx_buffer [2] = 0x00;
    write_array_led(tx_buffer, 3, LED_COMM);
}


// ================================================================
// ===                Static void functions                     ===
// ================================================================

void write_array(uint8_t * data, uint8_t len){
    uint8_t cs = calculateCS(data, len);
    Serial.write(data, len);
    Serial.write(cs);
}

void write_array_led(uint8_t * data, uint8_t len, uint8_t led){
    digitalWrite(led, HIGH);
    write_array(data, len);
    digitalWrite(led, LOW);
}

uint8_t calculateCS(uint8_t * data, uint8_t len){
    uint8_t checksum = *(data);
    for(uint8_t i = 1; i < len; i++){
        checksum = checksum ^ *(data + i);
    }
    return checksum;
}