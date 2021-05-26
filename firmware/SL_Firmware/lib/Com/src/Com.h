#ifndef _BT_COM_H_
#define _BT_COM_H_

#include "Arduino.h"
#include "Driver.h"
#include "Uart.h"
#include "commands.h"

// ================================================================
// ===              Define Communication Commands               ===
// ================================================================


class COM{
    public:
        COM(Driver *sl, Uart *uart);
        
        void rsv_msg_handler();

    private:
        Driver *driver;
        Uart *uart;

};

#endif