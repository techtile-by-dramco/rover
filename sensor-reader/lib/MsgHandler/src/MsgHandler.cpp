#include "MsgHandler.h"

// ================================================================
// ===                Static void functions                     ===
// ================================================================

static uint16_t convertToUINT16(uint8_t *data);
static uint32_t convertToUINT32(uint8_t *data);
static void convertToUINT8(uint8_t *buf, uint32_t val);
static uint8_t *convertToUINT8(uint32_t val);

static uint8_t check_buffer(uint8_t dis, uint8_t *buf);


// ================================================================
// ===                      Functions                           ===
// ================================================================

MsgHandler::MsgHandler(SensorDriver *driv){
  this->driv = driv;
  this->ser = new SerialComm();
}

void MsgHandler::alert_handler(){

}

void MsgHandler::tx_msg_handler(uint8_t expression){

  switch(expression){

    case GET_POSITIONS_ALL: {
      
      // comment this line when timer is used
      if(!execute_enable){
        buffer[0] = driv->get_all_positions();
      }
      ser->transmitFrame(POSITIONS_ALL_RSP, 12, buffer[0]);
    } break;

    case AUTO_POS_IND:{
      //uint8_t * buffer = driv->get_all_positions();
      ser->transmitFrame(AUTO_POS_IND, 12, buffer[0]);
    } break;

    case ALERT_IND:{
      ser->transmitFrame(ALERT_IND, alert_sensor_number);
    } break;

    default:
      break;
  }
    
}

void MsgHandler::rx_msg_handler(){


  uint8_t len;
  uint8_t rsv_buffer [100];
  if(ser->incoming_dataframe_handler(rsv_buffer, &len)){
    
    switch(*(rsv_buffer + 1)){
      case CONNECTION_CHECK_REQ: ser->transmitFrame(RX_MSG_CMF); break;

      case GET_POSITIONS_ALL: tx_msg_handler(GET_POSITIONS_ALL); break;

      case SET_START_SCANNING: driv->enable_timer(); break;

      case SET_STOP_SCANNING: driv->disable_timer(); break;

      case SET_SCANNING_FREQ: driv->timer_frequency(*(rsv_buffer + 3)); break;


      case SET_ALERT_RANGE: range = *(rsv_buffer + 3); break;

      case SET_ALERT_ENABLE: alert_msgs = true; break;

      case SET_ALERT_DISABLE: alert_msgs = true; break;


      default:{
      } break;

    }
  }


}


void MsgHandler::run(){
   if(execute_enable){ 
      execute_enable = false;

        //  Update sensor readout values
      buffer[0] = driv->get_all_positions();
      
        //  Send sensor readout values over SERIAL bus
      tx_msg_handler(AUTO_POS_IND);

      //   //  Check if allowable operation range is exceeded and give warning over SERIAL bus
      //
      // CURENTLY NOT USED
      //
      // uint8_t sensor_number = check_buffer(range, buffer[0]);
      // if(sensor_number > 0){
      //   alert_sensor_number = sensor_number;
      //   tx_msg_handler(ALERT_IND);
      // }
    }
}



// ================================================================
// ===                Static void functions                     ===
// ================================================================

uint8_t check_buffer(uint8_t dis, uint8_t *buf){

  uint8_t ret_val = 0;

  for(uint8_t i = 0; i < NR_OF_SENSORS; i++){
    if(*(buf + i) < dis){
      ret_val = i + 1;
    }
  }

  return ret_val;
}


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