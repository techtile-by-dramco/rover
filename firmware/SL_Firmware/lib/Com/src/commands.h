// ===========================================
// ===          ***  Requests ***          ===
// ===========================================
#define SL_CONNECTION_CHECK_REQ       0x01    //  Test the connection

  //  Start stop stepper motors
#define SL_STOP_REQ                   0x10    //  Stop stepper motors
#define SL_DS_REQ                     0x11    //  Go home downstairs
#define SL_US_REQ                     0x12    //  Go home upstairs
#define SL_MOVE_REV_REQ               0x13    //  Move a number of revolutions
#define SL_MOVE_STP_REQ               0x14    //  Move a number of steps

  //  Set commands -- Stepper motor settings
#define SL_CHANGE_SPEED_RPS_REQ       0x20    //  Change speed [Revolutions per second]
#define SL_CHANGE_SPEED_SPS_REQ       0x21    //  Change speed [Steps per second]
#define SL_DIR_CW_REQ                 0x22    //  Change direction clockwise (rise scissorlift)
#define SL_DIR_CCW_REQ                0x23    //  Change direction counterclockwise (descend scissorlift)

  //  Get commands 
    //  HOME
#define SL_ISHOME_DS_REQ              0x30    //  Get scissorlift in downstairs home position status     
#define SL_ISHOME_US_REQ              0x31    //  Get scissorlift in upstairs home position status
    //  ALTITUDE
#define SL_ALTITUDE_REQ               0x32    //  Get altitude
    //  BATTERY
#define SL_BATTERY_VOLT_REQ           0x33    //  Get instantaneous battery voltage robot
#define SL_BATTERY_CURR_REQ           0x34    //  Get instantaneous current consumption
#define SL_BATTERY_PWR_REQ            0x35    //  Get instantaneous power consumption

// ===========================================
// ===       ***  Confirmations ***        ===
// ===========================================
#define SL_RX_MSG_CMF                 0xA0    //  Confirmation command received (checksum comfirmed)

// ===========================================
// ===        ***  Indications ***         ===
// ===========================================
#define SL_HOME_DS_IND                0x40    //  Indication home posistion downstairs
#define SL_HOME_US_IND                0x41    //  Indication home posistion upstairs

#define SL_POSITION_REACHED_IND       0x42

// ===========================================
// ===        ***  Responses ***           ===
// ===========================================
  //  HOME
#define SL_ISHOME_DS_RSP              0x50    //  Return scissorlift in downstairs home position status        
#define SL_ISHOME_US_RSP              0x51    //  Return scissorlift in upstairs home position status
  //  ALTITUDE
#define SL_ALTITUDE_RSP               0x52    //  Return altitude
  //  BATTERY
#define SL_BATTERY_VOLT_RSP           0x53    //  Return instantaneous battery voltage robot
#define SL_BATTERY_CURR_RSP           0x54    //  Return instantaneous current consumption
#define SL_BATTERY_PWR_RSP            0x55    //  Return instantaneous power consumption