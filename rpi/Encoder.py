import time
from time import sleep, perf_counter
from threading import Thread
import pigpio

MOTOR_1_ENCODER_P1 = 16
MOTOR_2_ENCODER_P1 = 20
MOTOR_3_ENCODER_P1 = 21

count = [0, 0, 0]
prev_count = [0, 0, 0]

speed_rpm = [0, 0, 0]

UPDATE_RATE = 0.1

prevtime = [time.time(), time.time()]



def runcount_1(receiver,x,y):
      global count
      count[0] += 1

def runcount_2(receiver,x,y):
      global count
      count[1] += 1

def runcount_3(receiver,x,y):
      global count
      count[2] += 1


def task():
      pi = pigpio.pi()

      pi.set_mode(MOTOR_1_ENCODER_P1, pigpio.INPUT)
      pi.set_mode(MOTOR_2_ENCODER_P1, pigpio.INPUT)
      pi.set_mode(MOTOR_3_ENCODER_P1, pigpio.INPUT)

      pi.set_pull_up_down(MOTOR_1_ENCODER_P1, pigpio.PUD_UP)
      pi.set_pull_up_down(MOTOR_2_ENCODER_P1, pigpio.PUD_UP)
      pi.set_pull_up_down(MOTOR_3_ENCODER_P1, pigpio.PUD_UP)

      cb1 =  pi.callback(MOTOR_1_ENCODER_P1, pigpio.FALLING_EDGE, runcount_1)
      cb2 =  pi.callback(MOTOR_2_ENCODER_P1, pigpio.FALLING_EDGE, runcount_2)
      cb3 =  pi.callback(MOTOR_3_ENCODER_P1, pigpio.FALLING_EDGE, runcount_3)

      while 1:
            if time.time() - prevtime[0] > UPDATE_RATE:
                  prevtime[0] = time.time()
                  for i in range(3):
                        speed_rpm[i] = ((count[i] - prev_count[i])/UPDATE_RATE)/35
                        prev_count[i] = count[i]

            # if time.time() - prevtime[1] > 1:
            #       prevtime[1] = time.time()
            #       print("New values")
            #       for i in range(3):
            #             print(speed_rpm[i])



class Encoder():

      def __init__(self):
            t1 = Thread(target=task)
            t1.start()

      def get_rpm(self, i):
            return speed_rpm[i]




