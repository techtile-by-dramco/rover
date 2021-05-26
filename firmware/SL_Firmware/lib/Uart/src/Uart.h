#ifndef _UART_H_
#define _UART_H_

#include "Arduino.h"
#include "Config.h"

class Uart{
    public:
        Uart();

        uint8_t incoming_dataframe_handler(uint8_t *rsv_buffer, uint16_t *total_len);

            //  Transmit messages  
        void transmitFrame(uint8_t cmd, uint16_t len, uint8_t * data);
        void transmitFrame(uint8_t cmd, uint8_t data);
        void transmitFrame(uint8_t cmd);

    private:
};

#endif