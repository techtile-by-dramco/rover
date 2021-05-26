//#define CHECKSUM

#define STEP_PER_REVOLUTION     1600
//  400 - 800 - 1600 - 3200 - 6400 - 12800 - 25600
//  1000 - 2000 - 4000 - 5000 - 8000 - 10000 - 20000 - 25000

#define BAUDRATE                1000000

#define ENCODER_M_1             2
#define ENCODER_M_2             3

#define STEPPER_EN_PIN          4
#define STEPPER_DIR_PIN         5
#define STEPPER_PULSE_PIN       6

#define ENDSTOP_SL_DOWNSTAIRS   10
#define ENDSTOP_SL_UPSTAIRS     11

//#define ENABLE_POWER            7

#define LED_BATTERY_GOOD        8
#define LED_PID_ON              9
#define LED_COMM                12
#define LED_ERROR               13

#define BUTTON_US               A0
#define BUTTON_DS               A1


#define BATTERY_LOW_VOLTAGE     12000   //  Value in [mV]