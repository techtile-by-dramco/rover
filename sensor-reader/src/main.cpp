#include <Arduino.h>
#include "Config.h"
#include "MsgHandler.h"
#include "SensorDriver.h"

//#define DEBUG

SensorDriver sensor_driver;
MsgHandler com_handler(&sensor_driver);

bool execute_enable;

void setup() {

  #ifdef DEBUG
    SerialUSB.begin(115200);
  #endif

  sensor_driver.init();

  SER.begin(2000000);

}


void loop() {

  com_handler.rx_msg_handler();

  com_handler.run();


}

