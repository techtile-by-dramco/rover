
#!/usr/bin/env python

# from subprocess import HIGH_PRIORITY_CLASS
# from syslog import LOG_WARNING
from smbus import SMBus
import time
import RPi.GPIO as IO

i2cbus = 0

#===========================================
#===       ***  I2C ADRESSES ***         ===
#===========================================

I2C_IOEXP_1 = 0x20
I2C_IOEXP_2 = 0x21

#===========================================
#===         ***  Registers ***          ===
#===========================================

IODIRA      = 0x00  # IO direction A - 1= input 0 = output
IODIRB      = 0x01  # IO direction B - 1= input 0 = output    
IPOLA       = 0x02  # Input polarity A
IPOLB       = 0x03  # Input polarity B
GPINTENA    = 0x04  # Interrupt-onchange A
GPINTENB    = 0x05  # Interrupt-onchange B
DEFVALA     = 0x06  # Default value for port A
DEFVALB     = 0x07  # Default value for port B
INTCONA     = 0x08  # Interrupt control register for port A
INTCONB     = 0x09  # Interrupt control register for port B
IOCON       = 0x0A  # Configuration register
GPPUA       = 0x0C  # Pull-up resistors for port A
GPPUB       = 0x0D  # Pull-up resistors for port B
INTFA       = 0x0E  # Interrupt condition for port A
INTFB       = 0x0F  # Interrupt condition for port B
INTCAPA     = 0x10  # Interrupt capture for port A
INTCAPB     = 0x11  # Interrupt capture for port B
GPIOA       = 0x12  # Data port A
GPIOB       = 0x13  # Data port B
OLATA       = 0x14  # Output latches A
OLATB       = 0x15  # Output latches B

#===========================================
#===      ***  Status Integers ***       ===
#===========================================


#===========================================
#===       ***  Cross Define ***         ===
#===========================================

# I2C_IOEXP_2 -- PORT A -- PINS

MOTOR_1_CTRL_INA    =   0
MOTOR_1_CTRL_INB    =   1

MOTOR_2_CTRL_INA    =   2
MOTOR_2_CTRL_INB    =   3

MOTOR_3_CTRL_INA    =   4
MOTOR_3_CTRL_INB    =   5

# I2C_IOEXP_2 -- PORT B -- PINS

MOTOR_1_EN          =   0
MOTOR_2_EN          =   1
MOTOR_3_EN          =   2

# MOTORS
MOTOR_1             =   1
MOTOR_2             =   2
MOTOR_3             =   3

# VALUE
LOW                 =   0
HIGH                =   1

# PWM
PWM_FREQ            =   1000

MOTOR_1_PWM         =   13
MOTOR_2_PWM         =   19
MOTOR_3_PWM         =   26



class Hat():

    def __init__(self):
        self.i2cbus = SMBus(1)

        self.status = [0,0,0,0]

        self.i2cbus.write_byte_data(I2C_IOEXP_1, IOCON, 0x02)   # Update configuration register
        
        
        self.i2cbus.write_byte_data(I2C_IOEXP_2, IOCON, 0x02)   # Update configuration register
        
        self.i2cbus.write_byte_data(I2C_IOEXP_2, IODIRA, 0x00)  # Set Port A as outputs
        self.i2cbus.write_byte_data(I2C_IOEXP_2, IODIRB, 0x00)  # Set Port B as outputs


        IO.setwarnings(False)                       #   Do not show warnings
        IO.setmode(IO.BCM)                          #   Programming the GPIO by BCM pin numbers
        IO.setup(MOTOR_1_PWM, IO.OUT)               #   Initialize GPIO13 as an output.
        IO.setup(MOTOR_2_PWM, IO.OUT)               #   Initialize GPIO19 as an output.
        IO.setup(MOTOR_3_PWM, IO.OUT)               #   Initialize GPIO26 as an output.

        self.p1 = IO.PWM(MOTOR_1_PWM, PWM_FREQ)
        self.p2 = IO.PWM(MOTOR_2_PWM, PWM_FREQ)
        self.p3 = IO.PWM(MOTOR_3_PWM, PWM_FREQ)

        self.p1.start(0)
        self.p2.start(0)
        self.p3.start(0)

    #===========================================
    #===        ***  IO Expander ***         ===
    #===========================================    

    def set_status_reg(self, i2c, port, status_nr, bit_number, value):

        if value:
            self.status[status_nr] = self.status[status_nr] | (1 << bit_number)
        else:
            self.status[status_nr] = self.status[status_nr] & ~(1 << bit_number)

        self.i2cbus.write_byte_data(i2c, port, self.status[status_nr])
    
    
    def set_bit_port(self, i2c, port, bit_number, value):

        if i2c == I2C_IOEXP_1 and port == GPIOA:
            self.set_status_reg(I2C_IOEXP_1, GPIOA, 0, bit_number, value)

        if i2c == I2C_IOEXP_1 and port == GPIOB:
            self.set_status_reg(I2C_IOEXP_1, GPIOB, 1, bit_number, value)

        if i2c == I2C_IOEXP_2 and port == GPIOA:
            self.set_status_reg(I2C_IOEXP_2, GPIOA, 2, bit_number, value)

        if i2c == I2C_IOEXP_2 and port == GPIOB:
            self.set_status_reg(I2C_IOEXP_2, GPIOB, 3, bit_number, value)

    
    def set_bit_io2_porta(self, bit_number, value):
        self.set_bit_port(I2C_IOEXP_2, GPIOA, bit_number, value)

    def set_bit_io2_portb(self, bit_number, value):
        self.set_bit_port(I2C_IOEXP_2, GPIOB, bit_number, value)


    #===========================================
    #===         ***  Functions ***          ===
    #===========================================

    #   Motor change direction

    def set_dir(self, dir, pin1, pin2):
        if dir:
            self.set_bit_io2_porta(pin1, HIGH)
            self.set_bit_io2_porta(pin2, LOW)
        else:
            self.set_bit_io2_porta(pin1, LOW)
            self.set_bit_io2_porta(pin2, HIGH)


    def set_dir_motor(self, motor, dir):
        if motor == MOTOR_1:
            self.set_dir(dir, MOTOR_1_CTRL_INA, MOTOR_1_CTRL_INB)
        if motor == MOTOR_2:
            self.set_dir(dir, MOTOR_2_CTRL_INA, MOTOR_2_CTRL_INB)
        if motor == MOTOR_3:
            self.set_dir(dir, MOTOR_3_CTRL_INA, MOTOR_3_CTRL_INB)

    
    #   Motor enable/disable

    def enable_motor(self, motor):
        if motor == MOTOR_1:
            self.set_bit_io2_portb(MOTOR_1_EN, HIGH)
        if motor == MOTOR_2:
            self.set_bit_io2_portb(MOTOR_2_EN, HIGH)
        if motor == MOTOR_3:
            self.set_bit_io2_portb(MOTOR_3_EN, HIGH)

    def disable_motor(self, motor):
        if motor == MOTOR_1:
            self.set_bit_io2_portb(MOTOR_1_EN, LOW)
        if motor == MOTOR_2:
            self.set_bit_io2_portb(MOTOR_2_EN, LOW)
        if motor == MOTOR_3:
            self.set_bit_io2_portb(MOTOR_3_EN, LOW)
    
    def enable_all_motors(self):
        self.enable_motor(MOTOR_1)
        self.enable_motor(MOTOR_2)
        self.enable_motor(MOTOR_3)

    def disable_all_motors(self):
        self.disable_motor(MOTOR_1)
        self.disable_motor(MOTOR_2)
        self.disable_motor(MOTOR_3)
    
    def set_speed_motor(self, motor, speed):
        if motor == MOTOR_1:
            self.p1.start(speed)
        if motor == MOTOR_2:
            self.p2.start(speed)
        if motor == MOTOR_3:
            self.p3.start(speed)




#     def main():
#         '''
#         Main program function
#         '''

#         i2cbus = SMBus(1)  # Create a new I2C bus
#         i2caddress = 0x21  # Address of MCP23017 device

#         i2cbus.write_byte_data(i2caddress, IOCON, 0x02)  # Update configuration register

#         i2cbus.write_byte_data(i2caddress, IODIRA, 0x00)  # Set Port A as outputs and Port B as inputs
#         i2cbus.write_byte_data(i2caddress, IODIRB, 0xFF)  # Set Port A as outputs and Port B as inputs

#         while (True):
#             porta = i2cbus.read_byte_data(i2caddress, GPIOA)  # Read the value of Port B
#             print(porta.to_bytes(1, 'big')) # print the value of Port B
            
#             portb = i2cbus.read_byte_data(i2caddress, GPIOB)  # Read the value of Port B
#             print(portb.to_bytes(1, 'big')) # print the value of Port B

#             a = i2cbus.read_byte_data(i2caddress, IODIRA)
#             print('IODIRA ' + str(a))
#             a = i2cbus.read_byte_data(i2caddress, IODIRB)
#             print('IODIRB ' + str(a))

#             if porta == 0x00:
#                 i2cbus.write_byte_data(i2caddress, GPIOA, 0x88)  # Set pin 1 to on
#             else:
#                 i2cbus.write_byte_data(i2caddress, GPIOA, 0x00)  # Set pin 1 to on
#             time.sleep(1)  # Wait 500ms

#             # i2cbus.write_byte_data(i2caddress, GPIOA, 0x00)  # Set pin 1 to off
#             # time.sleep(0.5)  # Wait 500ms


# if __name__ == "__main__":
#     main()
