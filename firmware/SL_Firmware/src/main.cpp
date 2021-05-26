#include "Arduino.h"
#include "Com.h"
#include "Driver.h"
#include "Uart.h"
#include "Config.h"

//#define CHECKSUM



// ================================================================
// ===                      Create objects                      ===
// ================================================================

Uart ser;
Driver sl(&ser);
COM com(&sl, &ser);

uint8_t check_buttons = 0;

// ================================================================
// ===                Static void functions                     ===
// ================================================================

static void pciSetup(byte pin);



// ================================================================
// ===                          ISR                             ===
// ================================================================

ISR(PCINT1_vect){
  check_buttons = 1;
}


// ================================================================
// ===                        Setup                             ===
// ================================================================

void setup() {

  Serial.begin(BAUDRATE);

  //pinMode(ENABLE_POWER, OUTPUT);

  pinMode(ENDSTOP_SL_DOWNSTAIRS, INPUT);
  pinMode(ENDSTOP_SL_UPSTAIRS, INPUT);

  pinMode(LED_BATTERY_GOOD, OUTPUT);
  pinMode(LED_COMM, OUTPUT);
  pinMode(LED_PID_ON, OUTPUT);
  pinMode(LED_ERROR, OUTPUT);

  Wire.begin();
  Wire.setClock(400000L);

  sl.init();

  sl.dir_cw();
  sl.speed_sps(400);

  pciSetup(A0);
  pciSetup(A1);
  
}


// ================================================================
// ===                        Loop                              ===
// ================================================================

void loop() {

  com.rsv_msg_handler();

  sl.run();

  sl.led_blink(LED_BATTERY_GOOD, 1000);

  sl.button_status(&check_buttons);

  sl.check_parameters(500);
  
}


// ================================================================
// ===                        Functions                         ===
// ================================================================

void pciSetup(byte pin){
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}