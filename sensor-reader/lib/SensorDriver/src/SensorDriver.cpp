#include "SensorDriver.h"

// ================================================================
// ===                Static void functions                     ===
// ================================================================

static void read_position_array(uint8_t* buffer, uint8_t trig, uint8_t echo1, uint8_t echo2, uint8_t echo3);


void TC3_Handler() {
  Adafruit_ZeroTimer::timerHandler(3);
}

void TimerCallback0(void){
    execute_enable = true;
}


// ================================================================
// ===                      Functions                           ===
// ================================================================

SensorDriver::SensorDriver(){
    this->tmr = new Adafruit_ZeroTimer(3);
}


void SensorDriver::init(){
    pinMode(TRIG_1, OUTPUT);
    pinMode(TRIG_2, OUTPUT);
    pinMode(TRIG_3, OUTPUT);
    pinMode(TRIG_4, OUTPUT);
    pinMode(ECHO_1, INPUT);
    pinMode(ECHO_2, INPUT);
    pinMode(ECHO_3, INPUT);
    pinMode(ECHO_4, INPUT);
    pinMode(ECHO_5, INPUT);
    pinMode(ECHO_6, INPUT);
    pinMode(ECHO_7, INPUT);
    pinMode(ECHO_8, INPUT);
    pinMode(ECHO_9, INPUT);
    pinMode(ECHO_10, INPUT);
    pinMode(ECHO_11, INPUT);
    pinMode(ECHO_12, INPUT);

    init_timer();
}


void SensorDriver::init_timer(){

    prescaler = TC_CLOCK_PRESCALER_DIV1;

    if ((freq < 24000000) && (freq > 800)) {
        divider = 1;
        prescaler = TC_CLOCK_PRESCALER_DIV1;
        compare = 48000000/freq;
    } else if (freq > 400) {
        divider = 2;
        prescaler = TC_CLOCK_PRESCALER_DIV2;
        compare = (48000000/2)/freq;
    } else if (freq > 200) {
        divider = 4;
        prescaler = TC_CLOCK_PRESCALER_DIV4;
        compare = (48000000/4)/freq;
    } else if (freq > 100) {
        divider = 8;
        prescaler = TC_CLOCK_PRESCALER_DIV8;
        compare = (48000000/8)/freq;
    } else if (freq > 50) {
        divider = 16;
        prescaler = TC_CLOCK_PRESCALER_DIV16;
        compare = (48000000/16)/freq;
    } else if (freq > 12) {
        divider = 64;
        prescaler = TC_CLOCK_PRESCALER_DIV64;
        compare = (48000000/64)/freq;
    } else if (freq > 3) {
        divider = 256;
        prescaler = TC_CLOCK_PRESCALER_DIV256;
        compare = (48000000/256)/freq;
    } else if (freq >= 0.75) {
        divider = 1024;
        prescaler = TC_CLOCK_PRESCALER_DIV1024;
        compare = (48000000/1024)/freq;
    } else {
        Serial.println("Invalid frequency");
        while (1) delay(10);
    }
}

void SensorDriver::timer_frequency(uint16_t freq){
    this->freq = freq;
    init_timer();
}

void SensorDriver::enable_timer(){
    this->tmr->enable(false);
    this->tmr->configure(prescaler,                // prescaler
            TC_COUNTER_SIZE_16BIT,          // bit width of timer/counter
            TC_WAVE_GENERATION_MATCH_PWM    // frequency or PWM mode
            );

    this->tmr->setCompare(0, compare);
    this->tmr->setCallback(true, TC_CALLBACK_CC_CHANNEL0, TimerCallback0);
    this->tmr->enable(true);
}

void SensorDriver::disable_timer(){
    this->tmr->enable(false);
}



uint8_t* SensorDriver::get_all_positions(){

    read_position_array(&position_buffer[0], TRIG_1, ECHO_1, ECHO_2, ECHO_3);
    read_position_array(&position_buffer[3], TRIG_2, ECHO_4, ECHO_5, ECHO_6);
    read_position_array(&position_buffer[6], TRIG_3, ECHO_7, ECHO_8, ECHO_9);
    read_position_array(&position_buffer[9], TRIG_4, ECHO_10, ECHO_11, ECHO_12);
    
    return &position_buffer[0];
}



// ================================================================
// ===                Static void functions                     ===
// ================================================================

float raw_buffer [3];

void read_position_array(uint8_t* buffer, uint8_t trig, uint8_t echo1, uint8_t echo2, uint8_t echo3){
    digitalWrite(trig, LOW);
    delayMicroseconds(DELAY_1);
    digitalWrite(trig, HIGH);
    delayMicroseconds(DELAY_2);
    digitalWrite(trig, LOW);
    raw_buffer[0] = pulseIn(echo1, HIGH);
    raw_buffer[1] = pulseIn(echo2, HIGH);
    raw_buffer[2] = pulseIn(echo3, HIGH);

    for(uint8_t i = 0; i < 3; i++){
        *(buffer + i) = uint8_t(*(raw_buffer + i) * 0.034 / 2);
    }
}
