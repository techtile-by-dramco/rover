#ifndef _DRIVER_H_
#define _DRIVER_H_

#include <Arduino.h>
#include "Endstop.h"
#include "DM556.h"
#include "Uart.h"
#include "commands.h"
#include "Config.h"
#include "SparkFun_VL53L1X.h"
#include "Adafruit_ADS1X15.h"
#include "SPI.h"

enum State_enum {IDLE, MOVE, HOME_DS, HOME_US, MOVE_DS, MOVE_US};

#define CHAN_5V_VOLTAGE         0
#define CHAN_BATTERY_VOLTAGE    1
#define CHAN_INPUT_CURRENT      2

class Driver{
    public:
        Driver(Uart *uart);

        void init();
        
            // Control Commands
        void goDS();
        void goUS();

        void enable_power_saving();
        void disable_power_saving();

        void speed_rps(uint8_t rps);
        void speed_sps(uint16_t sps);

        void dir_ccw();
        void dir_cw();

        void move_steps(uint32_t steps);
        void move_revolutions(uint16_t revolutions);

        void home_downstairs();
        void home_upstairs();

        void run();
        void move_stepper();
        void stop_stepper();

            //  Endstops
        void check_endstops();

            //  LEDs
        void led_blink(uint8_t pin, uint32_t delay_ms);
        void error_blink();

            //  Buttons
        void button_status(uint8_t *status);


            //  Request Commands
        uint8_t get_home_ds_value();
        uint8_t get_home_us_value();
        uint8_t get_endstops();
                //  Distance sensor
        uint32_t get_altitude();
                //  External ADC
        uint32_t get_battery_voltage();
        float get_internal_voltage();
        uint32_t get_battery_current();
        uint32_t get_battery_power();


        void check_parameters(uint32_t delay_ms);

    private:
        DM556 *motor;
        Endstop *es_sl_ds;  //  Endstop upstairs
        Endstop *es_sl_us;  //  Endstop downstairs
        Uart    *uart;
        SFEVL53L1X *ds;
        Adafruit_ADS1115 *ads;

        uint8_t error;

        float volt_logic;

        uint8_t state = IDLE;

        uint8_t power_saving = 1;

        uint8_t home_ds;
        uint8_t home_us;
        uint8_t bool_gotohome = 0;

        uint32_t old_time, old_time_c_p, old_time_err = 0;
        uint32_t lastFlankUS = 0;
        uint32_t lastFlankDS = 0;
};
#endif