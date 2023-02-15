// ===========================================
// ===          ***  Requests ***          ===
// ===========================================

#define CONNECTION_CHECK_REQ        0x01    //  Test the connection


  //  Set commands
#define SET_UPDATE_RATE             0x10    //  Set Update Rate
#define SET_RANGE                   0x11    //  Set maximum range, values above are ignored
#define SET_START_SCANNING          0x12    //  Start scanning
#define SET_STOP_SCANNING           0x13    //  Stop scanning
#define SET_OBJECT_DETECTION_RANGE  0x15    //  Minimum range before alert message is generated
#define SET_SCANNING_FREQ           0x16    //  Change scanning rate

#define SET_ALERT_RANGE             0x20    //  Set ALERT range
#define SET_ALERT_ENABLE            0x21    //  Enable alert message (object detected)
#define SET_ALERT_DISABLE           0x22    //  Disable alert message (object detected)

  //  Get commands 
#define GET_POSITIONS_ALL           0x30    //  Return all 12 positions
#define GET_POSITION_FROM_NR        0x31    //  Return position from given number


// ===========================================
// ===       ***  Confirmations ***        ===
// ===========================================
#define RX_MSG_CMF                  0xA0    //  Confirmation command received (checksum comfirmed)

// ===========================================
// ===        ***  Indications ***         ===
// ===========================================

#define ALERT_IND                   0x40    //  Alert indication

#define AUTO_POS_IND                0x41    //  If scanning enable, send positions over SERIAL bus

// ===========================================
// ===        ***  Responses ***           ===
// ===========================================

#define POSITIONS_ALL_RSP           0x50    //  Return all positions
