#ifndef SENSORDRIVER_H
#define SENSORDRIVER_H

#include "Arduino.h"
#include "Config.h"
#include "Adafruit_ZeroTimer.h"

#define DELAY_1 2
#define DELAY_2 10

// ================================================================
// ===              Define Communication Commands               ===
// ================================================================

extern bool execute_enable;


class SensorDriver{
    public:
        SensorDriver();

        void init();

            //  Timer Functions
        void init_timer();
        void timer_frequency(uint16_t freq);
        void enable_timer();
        void disable_timer();


        uint8_t* get_all_positions();

    private:
        uint8_t position_buffer [12];
        Adafruit_ZeroTimer * tmr;

        uint16_t freq = 1;
        uint16_t divider = 1;
        uint16_t compare = 0;

        tc_clock_prescaler prescaler;


};

#endif
