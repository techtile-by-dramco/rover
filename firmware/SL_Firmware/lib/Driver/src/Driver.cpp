#include "Driver.h"

void endstop_manager(Driver *drv, Uart* uart, uint8_t tx_msg, uint8_t es_status, uint8_t home);

Driver::Driver(Uart *uart){
    this->motor = new DM556(STEPPER_DIR_PIN, STEPPER_PULSE_PIN, STEPPER_EN_PIN);
    this->es_sl_ds = new Endstop(ENDSTOP_SL_DOWNSTAIRS);
    this->es_sl_us = new Endstop(ENDSTOP_SL_UPSTAIRS);
    this->ads = new Adafruit_ADS1115; 
    this->uart = uart;
    this->ds = new SFEVL53L1X();
}

void Driver::init(){
    ds->begin();
    ds->setDistanceModeLong();
    ds->startRanging();

    motor->init(STEP_PER_REVOLUTION);
    motor->disable();

    ads->begin();
    ads->setGain(GAIN_ONE);

    volt_logic = get_internal_voltage();

    if(es_sl_ds->get_state()) home_ds = 1;
    if(es_sl_us->get_state()) home_us = 1;
}

void Driver::goDS(){
    state = HOME_DS;
}

void Driver::goUS(){
    state = HOME_US;
}

void Driver::enable_power_saving(){
    power_saving = 1;
}

void Driver::disable_power_saving(){
    power_saving = 0;
}

void Driver::speed_rps(uint8_t rps){
    motor->speed_rps(rps);
}
void Driver::speed_sps(uint16_t sps){
    motor->speed_sps(sps);
}

void Driver::dir_ccw(){
    motor->direction(DM556_DIR_CCW);
}

void Driver::dir_cw(){
    motor->direction(DM556_DIR_CW);
}

void Driver::move_steps(uint32_t steps){
    motor->move_no_steps(steps);
    motor->enable();
    state = MOVE;
}

void Driver::move_revolutions(uint16_t revolutions){
    motor->move_no_revolutions(revolutions);
    motor->enable();
    state = MOVE;
}

void Driver::home_downstairs(){
    state = HOME_DS;
}

void Driver::home_upstairs(){
    state = HOME_DS;
}

void Driver::run(){
    if(!error){
        switch(state){
            case IDLE:{

            } break;
            case MOVE:{
                if(motor->getStepCounter() < motor->getStepsQueue()){
                    move_stepper();
                }
                else if(motor->getStepCounter() == motor->getStepsQueue()){  
                    stop_stepper();
                }
                check_endstops();
            } break;

            case HOME_DS:{
                dir_ccw();
                motor->enable();
                state = MOVE_DS;
            } break;

            case HOME_US:{
                dir_cw();
                motor->enable();
                state = MOVE_US;
            } break;

            case MOVE_DS:{
                if(es_sl_ds->get_state()){
                    uart->transmitFrame(SL_HOME_DS_IND);
                    stop_stepper();
                    home_ds = 1;
                }
                else motor->pulseGen();
            } break;

            case MOVE_US:{
                if(es_sl_us->get_state()){
                    uart->transmitFrame(SL_HOME_US_IND);
                    stop_stepper();
                    home_us = 1;
                }
                else motor->pulseGen();
            } break;

            default:
            break;
        }
    }
    else error_blink();
}

void Driver::move_stepper(){
    //motor->enable();
    if(home_ds && es_sl_ds->get_state()){ 
        dir_cw();
        motor->pulseGen();
    }
    else if(home_us && es_sl_us->get_state()){
        dir_ccw();
        motor->pulseGen();  
    }
    else{
        motor->pulseGen(); 
    }
}

void Driver::stop_stepper(){
    motor->stop();
    delay(50);
    if(power_saving) motor->disable();
    state = IDLE;
}

void Driver::check_endstops(){
    endstop_manager(this, uart, SL_HOME_DS_IND, es_sl_ds->get_state(), home_ds);
    endstop_manager(this, uart, SL_HOME_US_IND, es_sl_us->get_state(), home_us);
}

void Driver::led_blink(uint8_t pin, uint32_t delay_ms){
    if(millis() > old_time + delay_ms){
        digitalWrite(pin, !digitalRead(pin));
        //if(error) digitalWrite(LED_ERROR, !digitalRead(LED_ERROR));
        old_time = millis();
    }
}

void Driver::error_blink(){
    if(millis() > old_time_err + 250){
        digitalWrite(LED_ERROR, !digitalRead(LED_ERROR));
        old_time_err = millis();
    }
}

void Driver::button_status(uint8_t *status){
    if(*status){
        *status = 0;

        if(!digitalRead(BUTTON_US)){
            lastFlankUS = millis();
            if(state == MOVE_US){
                stop_stepper();     // state is modified inside
            }
            else{
                state = HOME_US;
            }
        }
        else{
            if(state == MOVE_US){
                uint32_t currentFlank = millis();
                if(currentFlank - lastFlankUS > 1000){
                    stop_stepper(); // state is modified inside
                }         
                lastFlankUS = currentFlank;
            }
        }

        if(!digitalRead(BUTTON_DS)){
            lastFlankDS = millis();
            if(state == MOVE_DS){
                stop_stepper();     // state is modified inside
            }
            else{
                state = HOME_DS;
            }
        }
        else{
            if(state == MOVE_DS){
                uint32_t currentFlank = millis();
                if(currentFlank - lastFlankDS > 1000){
                    stop_stepper(); // state is modified inside
                }         
                lastFlankDS = currentFlank;
            }
        }

    }

}

void Driver::check_parameters(uint32_t delay_ms){
    if(millis() > old_time_c_p + delay_ms){
        
            //  Check Battery Voltage
        if(get_battery_voltage() < BATTERY_LOW_VOLTAGE){
            stop_stepper();
            error = 1;
        }
        else{
            digitalWrite(LED_ERROR, LOW);
            error = 0;
        }
            
            //  Check current
        // ...



            //  Check distance
        if(get_altitude() < 1000){
            speed_sps(200);
        }
        else{
            speed_sps(400);
        }

        old_time_c_p = millis();
    }
}


// ================================================================
// ===                      Get commands                        ===
// ================================================================


uint8_t Driver::get_endstops(){
    return (es_sl_ds->get_state() | es_sl_us->get_state());
}

uint8_t Driver::get_home_ds_value(){
    return home_ds;
}

uint8_t Driver::get_home_us_value(){
    return home_us;
}

uint32_t Driver::get_altitude(){
  uint32_t alt;
  while(1){
    if(ds->checkForDataReady()){
      alt = ds->getDistance();
      ds->clearInterrupt();
      return alt;
    }
  }
}

    //  Value returned in [mV]
uint32_t Driver::get_battery_voltage(){
    int16_t val = ads->readADC_SingleEnded(CHAN_BATTERY_VOLTAGE);
    return val*0.000125*5.7*1000;
}

    //  Value returned in [V]
float Driver::get_internal_voltage(){
    int16_t val = ads->readADC_SingleEnded(CHAN_5V_VOLTAGE);
    return val*0.000125*2;
}

    //  Value returned in [mA]
uint32_t Driver::get_battery_current(){
    int16_t val = ads->readADC_SingleEnded(CHAN_INPUT_CURRENT);
    return abs(val*0.000125-(volt_logic/2))/0.066*1000;
}

    //  Value returned in [mW]
uint32_t Driver::get_battery_power(){
    return get_battery_voltage()*get_battery_current()/1000;
}

/*
uint8_t check_battery_voltage(){
    if(get_battery_voltage() < BATTERY_LOW_VOLTAGE){
        digitalWrite(LED_BATTERY_GOOD, HIGH);
        #if DEBUG:
            Serial.println("Battery voltage ok");
        #endif
        return 0;
    }
    else{
        digitalWrite(LED_BATTERY_GOOD, LOW);
        #if DEBUG:
            Serial.println("Battery voltage to low");
        #endif
        return 1;
    }
}

void get_battery_voltage(){

}
*/

void endstop_manager(Driver *drv, Uart *uart, uint8_t tx_msg, uint8_t es_status, uint8_t home){
    if(es_status){
        drv->stop_stepper();
        home = 1;
        uart->transmitFrame(tx_msg);
    }
}