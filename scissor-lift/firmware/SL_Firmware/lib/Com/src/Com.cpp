#include "Com.h"

// ================================================================
// ===                Static void functions                     ===
// ================================================================

//void move_steps_function(uint8_t *buf, uint8_t len, Driver *driver);

uint16_t convertToUINT16(uint8_t *data);
uint32_t convertToUINT32(uint8_t *data);
void convertToUINT8(uint8_t *buf, uint32_t val);
uint8_t *convertToUINT8(uint32_t val);


// ================================================================
// ===                      Functions                           ===
// ================================================================

COM::COM(Driver *driver, Uart *uart){
  this->driver = driver;
  this->uart = uart;
}

void COM::rsv_msg_handler(){


  uint16_t len;
  uint8_t rsv_buffer [100];
  if(uart->incoming_dataframe_handler(rsv_buffer, &len)){
    uart->transmitFrame(SL_RX_MSG_CMF);
    switch(*(rsv_buffer + 1)){
      case SL_CONNECTION_CHECK_REQ: break;

        // REQ - Initialisation
      case SL_DS_REQ:               driver->goDS(); break;
      case SL_US_REQ:               driver->goUS(); break;
      
        //  REQ - Motion settings
      case SL_STOP_REQ:             driver->stop_stepper(); break;
      case SL_CHANGE_SPEED_RPS_REQ: driver->speed_rps(*(rsv_buffer + 3)); break;
      case SL_CHANGE_SPEED_SPS_REQ: driver->speed_sps(2); break;
      case SL_DIR_CW_REQ:           driver->dir_cw(); break;
      case SL_DIR_CCW_REQ:          driver->dir_ccw(); break;
      case SL_MOVE_REV_REQ:         driver->move_revolutions(convertToUINT16(rsv_buffer + 3)); break;
      //case SL_MOVE_STP_REQ: move_steps_function(rsv_buffer + 3, len, driver); break;
      case SL_MOVE_STP_REQ:         driver->move_steps(convertToUINT32(rsv_buffer + 3)); break;
      
        //  REQ - Get commands
      case SL_ISHOME_DS_REQ:        uart->transmitFrame(SL_ISHOME_DS_RSP, driver->get_home_ds_value()); break;
      case SL_ISHOME_US_REQ:        uart->transmitFrame(SL_ISHOME_US_RSP, driver->get_home_us_value()); break;
      case SL_ALTITUDE_REQ:         uart->transmitFrame(SL_ALTITUDE_RSP, sizeof(uint32_t), convertToUINT8(driver->get_altitude())); break;
      case SL_BATTERY_VOLT_REQ:     uart->transmitFrame(SL_BATTERY_VOLT_RSP, sizeof(uint32_t), convertToUINT8(driver->get_battery_voltage())); break;
      case SL_BATTERY_CURR_REQ:     uart->transmitFrame(SL_BATTERY_CURR_RSP, sizeof(uint32_t), convertToUINT8(driver->get_battery_current())); break;
      case SL_BATTERY_PWR_REQ:      uart->transmitFrame(SL_BATTERY_PWR_RSP, sizeof(uint32_t), convertToUINT8(driver->get_battery_power())); break;

      default:{
      } break;

    }
  }

  digitalWrite(LED_COMM, LOW);

}


// ================================================================
// ===                Static void functions                     ===
// ================================================================

uint16_t convertToUINT16(uint8_t *data){
  return ((*(data + 1) << 8) | *data);
}

uint32_t convertToUINT32(uint8_t *data){
  return (((uint32_t)*(data + 3) << 24) | ((uint32_t)*(data + 2) << 16) | (*(data + 1) << 8) | *data);
}

void convertToUINT8(uint8_t *buf, uint32_t val){
  for(uint8_t i = 0; i < sizeof(uint32_t); i++){
      *(buf + i) = (val >> 8*i) & 0xFF;
  }
}

uint8_t buf [4];
uint8_t *convertToUINT8(uint32_t val){
   for(uint8_t i = 0; i < sizeof(uint32_t); i++){
      *(buf + i) = (val >> 8*i) & 0xFF;
  }
  return buf;
}