#include "DM556.h"
#include "math.h"

uint8_t step_pin_changed = 0;

// ================================================================
// ===                Static void functions                     ===
// ================================================================

static void pulseGenerator(uint8_t pin, uint8_t *state, uint32_t *counter);

// ================================================================
// ===                          ISR                             ===
// ================================================================

ISR(TIMER1_COMPA_vect){
    step_pin_changed = 1;
}

// ================================================================
// ===                         Functions                        ===
// ================================================================

DM556::DM556(uint8_t dir, uint8_t step, uint8_t enable){
    this->dir_pin = dir;
    this->step_pin = step;
    this->en_pin = enable;
}

void DM556::init(int spr){
    pinMode(dir_pin, OUTPUT);
    pinMode(step_pin, OUTPUT);
    pinMode(en_pin, OUTPUT);
    digitalWrite(en_pin, HIGH);
    delay(10);
    digitalWrite(en_pin, LOW);
    this->steps_per_revolution = spr;
}

void DM556::enable(){
    digitalWrite(en_pin, LOW);
}

void DM556::disable(){
    digitalWrite(en_pin, HIGH);
}

void DM556::pulseGen(){
    pulseGenerator(step_pin, &step_state, &steps_counter);
}

//  Move a number of revolutions
void DM556::move_no_revolutions(uint8_t rotations){
    total_steps = steps_per_revolution * 2 * rotations;
}

//  Move a number of steps
void DM556::move_no_steps(uint32_t steps){
    total_steps += steps * 2;
}

//  Stop Stepper motor
void DM556::stop(){
    digitalWrite(this->step_pin, LOW);
    total_steps = 0;
    steps_counter = 0;
}

void DM556::direction(uint8_t Stepper_rotation){
    if(Stepper_rotation == DM556_DIR_CW)     {   digitalWrite(this->dir_pin, 0);    dir = DM556_DIR_CW;   }
    if(Stepper_rotation == DM556_DIR_CCW)    {   digitalWrite(this->dir_pin, 1);    dir = DM556_DIR_CCW;  }
}

// Determine the speed [revolutions per second]
void DM556::speed_rps(uint8_t revolution_per_sec){
    uint32_t steps_per_sec = (this->steps_per_revolution * revolution_per_sec);
    speed_sps(steps_per_sec);
}

void DM556::speed_sps(uint16_t steps_per_sec){
    float devision = (16000000 / (prescaler * steps_per_sec * 2 * 1.0));
    uint32_t nr = (round(devision) - 1);
    
    cli();
    //  Timer 1
    TCCR1A = 0;         // set entire TCCR1A register to 0
    TCCR1B = 0;         // same for TCCR1B
    TCNT1  = 0;         //initialize counter value to 0
    // set compare match register for 1hz increments
    OCR1A = nr;
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set prescaler
    switch(prescaler){
        case 1:     TCCR1B |= (1 << CS10); break;
        case 8:     TCCR1B |= (1 << CS11); break;
        case 64:    TCCR1B |= (1 << CS11) | (1 << CS10); break;
        case 256:   TCCR1B |= (1 << CS12); break;
        case 1024:  TCCR1B |= (1 << CS12) | (1 << CS10); break;
        default: break;
    }
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    sei();
}





// ================================================================
// ===                      Get commands                      ===
// ================================================================


uint32_t DM556::getStepCounter(){
    return steps_counter;
}
uint32_t DM556::getStepsQueue(){
    return total_steps;
}
uint32_t DM556::getParamSPR(){
    return steps_per_revolution;
}

// ================================================================
// ===                Static void functions                     ===
// ================================================================

void pulseGenerator(uint8_t pin, uint8_t *state, uint32_t *counter){
    if(step_pin_changed){
        digitalWrite(pin, *state);
        *state = !*state;
        (*counter)++;
        step_pin_changed = 0;
    }
}
