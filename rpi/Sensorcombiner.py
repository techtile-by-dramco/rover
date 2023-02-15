import time
from threading import Thread

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
SET_ALERT_ENABLE            = b'\x21'   #  Enable alert message
SET_ALERT_DISABLE           = b'\x22'   #  Disable alert message

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

        t1 = Thread(target=self.task)
        t1.start()

        self.distancebuffer = [0,0,0,0,0,0,0,0,0,0,0,0]

    def test_connection(self):
        self.send_message_no_data(TEST_CONNECTION)
        return self.read_respons()

    def start_scanning(self):
        # self.port.write(b'\x02\x12\x00')
        self.send_message_no_data(SET_START_SCANNING)

    def stop_scanning(self):
        # self.port.write(b'\x02\x13\x00')
        self.send_message_no_data(SET_STOP_SCANNING)
    
    def change_update_rate(self, rate):
        self.send_message_data(SET_SCANNING_FREQ, rate)

    def get_positions(self):
        self.send_message_no_data(GET_POSITIONS_ALL)



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
            #aa = self.port.read(1)
            #print('init read: ' + str(aa))
            if self.port.read(1) == BEGIN_DELIMITER:
                res = self.port.read(2)
                #print('result: ' + str(res))
                # maybe I need to add a delay to solve the problem
                time.sleep(0.001)
                response = res [0]
                num = int.from_bytes(res[1:2], "big")
                if(num != 0):
                    dat_buf = bytes(self.port.read(1))
                    for i in range(num - 1):
                        dat_buf = dat_buf + bytes(self.port.read(1))
                checksum = self.port.read(1)
        result = BEGIN_DELIMITER + bytes(res) + bytes(dat_buf)
        length = 3 + num
        return result, response, length

    def read_distance_buffer(self):
        # print(self.distancebuffer)
        return self.distancebuffer

    
    #   Writings

    def send_message_no_data(self, command):
        self.port.write(BEGIN_DELIMITER + command)

    def send_message_data(self, command, data):
        self.port.write(BEGIN_DELIMITER + command + b'\x01' + data.to_bytes(1, 'big'))

    
    #   Time

    def current_millis_time(self):
        return round(time.time() * 1000)


    #   Task

    def task(self):

        # prev_time = self.current_millis_time()
        
        while 1:

            #   Polling positions each second ***1***
            # curr_time = self.current_millis_time()
            # if(curr_time > prev_time + 1000):
            #     self.get_positions()
            #     prev_time = curr_time


            #   Read all incoming messages

            result, respons, length = self.read_respons()
            if(respons != 0):

                # -- DEBUG --
                #   print('Resp: ' + str(respons))
                #   print(result)
                #   print(str(int.from_bytes(AUTO_POS_IND, "big")) + ' =? ' + str(respons)) 
                
                #   Currently IN USE
                if(respons == int.from_bytes(AUTO_POS_IND, "big")):
                    for i in range(12):
                        self.distancebuffer [i] = int.from_bytes(result[3+i:4+i], "big")
                
                #   Currently not used -- by polling each second 'get_positions' ***1***
                if(respons == int.from_bytes(POSITIONS_ALL_RSP, "big")):
                    for i in range(12):
                        self.distancebuffer [i] = int.from_bytes(result[3+i:4+i], "big")

                #   Currently not used
                if(respons == int.from_bytes(ALERT_IND, "big")):
                    a = 1
                    # Do something with the alert indication