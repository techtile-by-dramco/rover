#ifndef _ENDSTOP_H_
#define _ENDSTOP_H_

#include <Arduino.h>


class Endstop{
    public:
        Endstop(uint8_t pin);
        void init();
        uint8_t get_state();

    private:
        uint8_t pin;
};
#endif