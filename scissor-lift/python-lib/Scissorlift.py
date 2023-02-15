import time

#   Start stop stepper motors
SL_STOP_REQ                 =   b'\x10'
SL_DS_REQ                   =   b'\x11'
SL_US_REQ                   =   b'\x12'
SL_MOVE_REV_REQ             =   b'\x13'
SL_MOVE_STP_REQ             =   b'\x14'

#    Set commands -- Stepper motor settings
SL_CHANGE_SPEED_RPS_REQ     =   b'\x20'
SL_CHANGE_SPEED_SPS_REQ     =   b'\x21'
SL_DIR_CW_REQ               =   b'\x22'
SL_DIR_CCW_REQ              =   b'\x23'

#   Get commands
SL_ISHOME_DS_REQ            =   b'\x30'
SL_ISHOME_US_REQ            =   b'\x31'
SL_ALTITUDE_REQ             =   b'\x32'
SL_BATTERY_VOLT_REQ         =   b'\x33'
SL_BATTERY_CURR_REQ         =   b'\x34'
SL_BATTERY_PWR_REQ          =   b'\x35'


class Scissorlift():

    def __init__(self, port):
        self.port = port

      
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


    #   Get commands
    def read_voltage(self):
        self.port.write(b'\x02\x33\x00')
        return self.read_convert_result(self)

        
    def read_current(self):
        self.port.write(b'\x02\x34\x00')
        return self.read_convert_result(self)

    
    def read_power(self):
        self.port.write(b'\x02\x35\x00')
        return self.read_convert_result(self)


    #   Start stop stepper motors
    def stop(self):
        self.port.write(b'\x02' + SL_STOP_REQ + b'\x00')

    def home_ds(self):
        self.port.write(b'\x02' + SL_DS_REQ + b'\x00')

    def home_us(self):
        self.port.write(b'\x02' + SL_US_REQ + b'\x00')

    def move_steps(self): #TODO
        self.port.write(b'\x02\x13\x00')

    #   Set commands -- Stepper motor settings
    def change_speed_rps(self): #TODO
        self.port.write(b'\x02\x20\x00')

    def change_speed_sps(self): #TODO
        self.port.write(b'\x02\x21\x00')

    def change_dir_cw(self):
        self.port.write(b'\x02\x22\x00')
        
    def change_dir_ccw(self):
        self.port.write(b'\x02\x23\x00')
