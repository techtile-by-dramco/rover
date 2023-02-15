from Hat import Hat
from Encoder import Encoder
import time
from simple_pid import PID
from time import sleep, perf_counter
from threading import Thread


prev = time.time()


class Motordriver():

      def __init__(self, hat, ec):
            self.hat = hat
            self.ec = ec

            self.speed = [30, 0, 0]

            self._running = False
            self.speedchanged = False

            self.pid_buffer = [PID(0.2, 1.5, 0.01), PID(0.2, 1.5, 0.01), PID(0.2, 1.5, 0.01)]
            self.pid_controller_enable = [False, False, False]
            self.duty_cycle = [0, 0, 0]

            t1 = Thread(target=self.task)
            t1.start()

            
      def print_rpm(self):
            print("Rotates Per Minute Update")
            print("Motor 1 RPM: " + str(self.ec.get_rpm(0)))
            print("Motor 2 RPM: " + str(self.ec.get_rpm(1)))
            print("Motor 3 RPM: " + str(self.ec.get_rpm(2)))


      # THREAD
      def task(self):
            while 1:
                  time.sleep(0.1)
                  for i in range(3):
                        self.duty_cycle[i] = self.pid_buffer[i](self.ec.get_rpm(i))
                        
                        if self.pid_controller_enable[i] and self.duty_cycle[i] < 100 and self.duty_cycle[i] > 0:
                              self.hat.set_speed_motor(i + 1, self.duty_cycle[i])
                  
                  #self.print_motor_updates()


      def print_motor_updates(self):      
            global prev            
            if time.time() - prev > 1:
                  prev = time.time()
                  print("Update motors")
                  for i in range(3):
                        print('Duty Cycle: ' + str(self.duty_cycle[i]) + ' -- RPM: ' + str(self.ec.get_rpm(i)))

      def update_speed(self, motor, rpm):
            self.pid_buffer[motor].setpoint = rpm
            if rpm == 0:
                  self.pid_buffer[motor].auto_mode = False
                  self.pid_buffer[motor] = PID(0.2, 1.5, 0.01)
                  self.pid_controller_enable[motor] = False
                  self.hat.set_speed_motor(motor + 1, 0)
            else:
                  self.pid_buffer[motor].auto_mode = True
                  self.pid_controller_enable[motor] = True
















      # # def task1(self):
  
      # #       pid = PID(0.2, 1.5, 0.2, setpoint=self.speed[0])
      # #       while 1:

      # #             if self.speedchanged:
      # #                   self.speedchanged = False
      # #                   a = self.ec.get_rpm(0)
      # #                   pid = PID(0.2, 1.5, 0.2, setpoint=self.speed[0])
      # #                   self.hat.set_speed_motor(1, a)

      # #             if self._running:

      # #                   control = pid(self.ec.get_rpm(0))
                        
      # #                   if control < 100 and control > 0:
      # #                         self.hat.set_speed_motor(1, control)
      # #                   if control > 100:
      # #                         self.hat.set_speed_motor(1, 100)
      # #                   if control < 0:
      # #                         self.hat.set_speed_motor(1, 0)

      # #                   global prev
                        
      # #                   if time.time() - prev > 1:
      # #                         prev = time.time()
      # #                         print(control)
      # #                         print('RPM ' + str(self.ec.get_rpm(0)))

      # #             else:
      # #                   self.hat.set_speed_motor(1, 0)
      # #                   pid = PID(0.2, 1.5, 0.2, setpoint=self.speed[0])


      # # def task2(self):
  
      # #       pid = PID(0.2, 1.5, 0.2, setpoint=self.speed[1])
      # #       while 1:

      # #             if self.speedchanged:
      # #                   self.speedchanged = False
      # #                   a = self.ec.get_rpm(0)
      # #                   pid = PID(0.2, 1.5, 0.2, setpoint=self.speed[1])
      # #                   self.hat.set_speed_motor(1, a)

      # #             if self._running:

      # #                   control = pid(self.ec.get_rpm(0))
                        
      # #                   if control < 100 and control > 0:
      # #                         self.hat.set_speed_motor(2, control)
      # #                   if control > 100:
      # #                         self.hat.set_speed_motor(2, 100)
      # #                   if control < 0:
      # #                         self.hat.set_speed_motor(2, 0)

      # #                   global prev
                        
      # #                   if time.time() - prev > 1:
      # #                         prev = time.time()
      # #                         print(control)
      # #                         print('RPM ' + str(self.ec.get_rpm(1)))

      # #             else:
      # #                   self.hat.set_speed_motor(2, 0)
      # #                   pid = PID(0.2, 1.5, 0.2, setpoint=self.speed[1])

      # # def task3(self):
  
      # #       pid = PID(0.2, 1.5, 0.2, setpoint=self.speed[0])
      # #       while 1:

      # #             if self.speedchanged:
      # #                   self.speedchanged = False
      # #                   a = self.ec.get_rpm(0)
      # #                   pid = PID(0.2, 1.5, 0.2, setpoint=self.speed[0])
      # #                   self.hat.set_speed_motor(1, a)

      # #             if self._running:

      # #                   control = pid(self.ec.get_rpm(0))
                        
      # #                   if control < 100 and control > 0:
      # #                         self.hat.set_speed_motor(1, control)
      # #                   if control > 100:
      # #                         self.hat.set_speed_motor(1, 100)
      # #                   if control < 0:
      # #                         self.hat.set_speed_motor(1, 0)

      # #                   global prev
                        
      # #                   if time.time() - prev > 1:
      # #                         prev = time.time()
      # #                         print(control)
      # #                         print('RPM ' + str(self.ec.get_rpm(0)))

      # #             else:
      # #                   self.hat.set_speed_motor(1, 0)
      # #                   pid = PID(0.2, 1.5, 0.2, setpoint=self.speed[0])








      
      # # def set_speed_rpm(self, rpm):
      # #       self.speedchanged = True
      # #       self.speed[1] = rpm

      # def start(self):
      #       self._running = True

      # def stop(self):
      #       self._running = False

      # def set_speed_rpm(self, rpm):     
      #       # print('ok-1')

      #       pid = PID(0.2, 1.5, 0.01, setpoint=rpm)

      #       # print('ok0')

      #       # t1 = Thread(target=task_pid1(self, pid))

            
      #       # #self.hat.set_speed_motor(1, 40)

      #       # print('ok')

      #       # t1.start()

      #       # print('ok2')

      #       self.pid_controller(pid)


      # def pid_controller(self, pid):
      #       while 1:
      #             motor = 1
      #             control = pid(self.ec.get_rpm(motor-1))
      #             time.sleep(0.1)
                  
      #             if control < 100 and control > 0:
      #                   self.hat.set_speed_motor(motor, control)

      #             global prev
                  
      #             if time.time() - prev > 1:
      #                   prev = time.time()
      #                   print(control)
      #                   print('RPM' + str(self.ec.get_rpm(motor-1)))