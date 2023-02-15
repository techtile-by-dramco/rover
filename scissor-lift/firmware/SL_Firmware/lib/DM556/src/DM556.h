#ifndef _DM556_H_
#define _DM556_H_

#include <Arduino.h>

#define DM556_DIR_CW     0
#define DM556_DIR_CCW    1

class DM556{
    public:
        DM556(uint8_t dir, uint8_t step, uint8_t enable);

        void init(int spr);
        void stop();

        void enable();
        void disable();

        void speed_rps(uint8_t revolution_per_sec);
        void speed_sps(uint16_t steps_per_sec);
        
        void direction(uint8_t stepper_rotation);
        void move_no_steps(uint32_t steps);
        void move_no_revolutions(uint8_t rotations);

        void pulseGen();

            //  Request commands
        uint32_t getStepCounter();
        uint32_t getStepsQueue();
        uint32_t getParamSPR();

    private:
        uint8_t dir_pin;
        uint8_t step_pin;
        uint8_t en_pin;

        uint8_t power_saving = 1;

        uint32_t steps_counter;
        uint32_t total_steps;

        uint8_t dir;
        
        uint16_t prescaler = 8;
        uint8_t step_state = 0;
        uint32_t steps_per_revolution;
};

#endif