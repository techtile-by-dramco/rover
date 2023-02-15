import time

BEGIN_DELIMITER             = b'\x02'    #  Test connection

#===========================================
#===          ***  Requests ***          ===
#===========================================

TEST_CONNECTION             = b'\x01'    #  Test connection

#   Set Commands

SET_UPDATE_RATE             = b'\x10'    #  Set Update Rate
SET_RANGE                   = b'\x11'    #  Set maximum range, values above are ignored
SET_START_SCANNING          = b'\x12'    #  Start scanning
SET_STOP_SCANNING           = b'\x13'    #  Stop scanning
SET_OBJECT_DETECTION_RANGE  = b'\x15'    #  Minimum range before alert message is generated
SET_SCANNING_FREQ           = b'\x16'    #  Change scanning rate

SET_ALERT_RANGE             = b'\x20'   #  Set ALERT range
SET_ALERT_ENABLE            = b'\x21'   #  Enable alert message (object dete
SET_ALERT_DISABLE           = b'\x22'   #  Disable alert message (object det

#   Get commands
GET_POSITIONS_ALL           = b'\x30'   #  Return all 12 positions
GET_POSITION_FROM_NR        = b'\x31'   #  Return position from given number

#===========================================
#===       ***  Confirmations ***        ===
#===========================================

RX_MSG_CMF                  = b'\xA0'   #  Confirmation command received (checksum comfirmed)

#===========================================
#===        ***  Indications ***         ===
#===========================================

ALERT_IND                   = b'\x40'   #  Alert indication
AUTO_POS_IND                = b'\x41'   #  If scanning enable, send positions over SERIAL bus

#===========================================
#===        ***  Responses ***           ===
#===========================================

POSITIONS_ALL_RSP           = b'\x50'   #  Return all positions



class Sensorcombiner():

    def __init__(self, port):
        self.port = port

    def test_connection(self):
        self.send_message_no_data(TEST_CONNECTION)
        #return self.read_convert_result()
        return self.read_respons()

    def start_scanning(self):
        self.port.write(b'\x02\x12\x00')

    def stop_scanning(self):
        self.port.write(b'\x02\x13\x00')




#===        ***  Communication Functions ***           ===

    #   Readings

    def read_convert_result(self):
        result = 0 
        response = 0
        time.sleep(0.001)
        while self.port.in_waiting > 0:
            if self.port.read(1) == b'\x02':
                res = self.port.read(2)
                response = res [0]
                num = int.from_bytes(res[1:2], "big")
                #print("num is:" + str(num))
                if(num != 0):
                    ab = self.port.read(num)
                    result = ab[0] | (ab[1] << 8) | (ab[2] << 16) | (ab[3] << 24)
                    break
        return result, response


    def read_respons(self):
        result = 0
        response = 0
        res = 0
        dat_buf = 0
        num = 0
        while self.port.in_waiting > 0:
            if self.port.read(1) == BEGIN_DELIMITER:
                res = self.port.read(2)
                response = res [0]
                num = int.from_bytes(res[1:2], "big")
                if(num != 0):
                    dat_buf = bytes(self.port.read(1))
                    for i in range(num - 1):
                        dat_buf = dat_buf + bytes(self.port.read(1))
        result = BEGIN_DELIMITER + bytes(res) + bytes(dat_buf)
        length = 3 + num
        return result, response, length
    
    #   Writings

    def send_message_no_data(self, command):
        self.port.write(BEGIN_DELIMITER + command)


    #   Writings

    def task():
        a = 1
        # start task reading the values