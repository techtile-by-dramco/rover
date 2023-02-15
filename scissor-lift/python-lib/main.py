
import serial
import time
import numpy as np
from Scissorlift import Scissorlift


def main():


    #with serial.Serial('/dev/ttyUSB0', 1000000, timeout=1) as ser:
        # Wait some time before ATMEGA328P is ready (reset)
    time.sleep(5)
    while 1:
        print("Voltage is: " + str(sl.read_voltage(sl)))
        print("Current is: " + str(sl.read_current(sl)))
        print("Power is: " + str(sl.read_power(sl)))

        sl.home_us()
        time.sleep(2)
        sl.stop()
        break



        time.sleep(1)


if __name__ == "__main__":
    print("start")

    port = serial.Serial('/dev/ttyUSB0', 1000000, timeout=1)
    sl = ScissorliftClass(port)
    main()