#ifndef _MSGHANDLER_H_
#define _MSGHANDLER_H_

#include "Arduino.h"
#include "Messages.h"
#include "SensorDriver.h"
#include "SerialComm.h"

// ================================================================
// ===              Define Communication Commands               ===
// ================================================================


class MsgHandler{
    public:
        MsgHandler(SensorDriver *driv);
        
        void alert_handler();
        void tx_msg_handler(uint8_t expression);
        void rx_msg_handler();

        void run();

    private:
        SensorDriver *driv;
        SerialComm *ser;


        bool alert_msgs = false;
        uint8_t alert_sensor_number;
        uint8_t range = 10;
        uint8_t *buffer [NR_OF_SENSORS];

};

#endif
