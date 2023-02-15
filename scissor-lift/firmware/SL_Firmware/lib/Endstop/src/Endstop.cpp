#include "Endstop.h"

Endstop::Endstop(uint8_t pin){
    this->pin = pin;
}

void Endstop::init(){
    pinMode(pin, INPUT);
}

uint8_t Endstop::get_state(){
    return !digitalRead(pin);
}