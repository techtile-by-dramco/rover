#ifndef _SERIALCOM_H_
#define _SERIALCOM_H_

#include "Arduino.h"
#include "Config.h"

class SerialComm{
    public:
        SerialComm();

        uint8_t incoming_dataframe_handler(uint8_t *rsv_buffer, uint8_t *total_len);

            //  Transmit messages  
        void transmitFrame(uint8_t cmd, uint8_t len, uint8_t * data);
        void transmitFrame(uint8_t cmd, uint8_t data);
        void transmitFrame(uint8_t cmd);

    private:

};

#endif