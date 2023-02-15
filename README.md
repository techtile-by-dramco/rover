
To speed-up measurements, reduce labor-intensive tasks and mitigate human-errors, we have designed a rover
to perform automated 3D sampling inside the testbed. The rover consists of a baseplate, hosting the processing
unit and batteries, and a scissor lift to move the measurement equipment to the required height.


## Baseplate 
The baseplate contains a Raspberry Pi running Robot Operating System (ROS). It controls the
wheels, reads-out the sensors and performs navigation. By adopting ROS, the development of the rover is
sped-up and is more easily extendable for future applications. ROS consists of software packages dedicated
for building robots, containing the necessary drivers and algorithms. The communication, employed in ROS,
between nodes, i.e., processing entities, is handled by an asynchronous publish/subscribe message passing
system. In this manner, ROS forces the developer to decouple different parts of the system. For synchronized
communication, the request/response pattern is used.

A 16 cells battery pack, in a 4S4P configuration, powers the system. The battery voltage ranges from 10 V to
16.2 V depending on the charge state. The battery back, based on Lithium Cobalt Oxide (LCO) technology,
has a capacity of 170W h and can handle up to 480W peak power. Besides powering the rover, the battery pack
provides power to the peripherals and measurement equipment in order to conduct the experiments. When
the battery is low, the rover is able to navigate to its charging station, recharge and resume the measurements.

## Navigation
Marvelmind indoor RTLS is used as a positioning system. It has a precision of 2 cm.
Ultrasonic beacons are used to acquire the position of the mobile beacon (mounted on the baseplate). Four
beacons are fixed in the Techtile infrastructure. The position is determined by trilateration. To filter out
outliers, we make use of a Kalman filter. The recursive algorithm uses consecutive inputs to estimate the
current state. Based on the uncertainties of the inputs and the uncertainties of the previous estimates, the
current state is approximated. Besides localization, the rover features obstacle detection to mitigate crashing into objects in the room. Ultrasonic sensors mounted on the sides of the rover’s baseplate are used and have
a resolution of 3 mm and a range of 2 cm to 400 cm.

## Scissor lift.
The baseplate of the rover can navigate through the room in 2D space. By mounting the scissor
lift (Figure 5b) on top of the baseplate, the rover is able to measure in 3D space. The lift has a range of 55 cm
to 185 cm. The equipment can be mounted on top of the lift. Power is supplied to the equipment from the
battery pack. The scissor lift can be controlled with AT-commands. The height of the lift is determined by
the VL53L1X Time-of-Flight (ToF) sensor using a 940 nm invisible Class 1 laser. An advertised accuracy and
ranging error of ± 20 mm in both ambient light and dark light conditions.
