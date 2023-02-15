## Documentation

- The `rpi` folder contains the Python scripts to collect the sensor readings from the sensor-reader (`Sensorcombiner.py`) and driving the motors (with the rotary encoderers).
- The `scissor-lift` folder contains the files to talk the the scissor lift microcontrollers (Adruino Uno + custom-made shield) through Python.
- The `sensor-reader`folder contains the source files for the microcontroller to read-out all sensors on the rover. This microcontroller is read-out by the RPI on the rover (as dicussed above).



## System Descrption
To speed-up measurements, reduce labor-intensive tasks and mitigate human-errors, we have designed a rover
to perform automated 3D sampling inside the testbed. The rover consists of a baseplate, hosting the processing
unit and batteries, and a scissor lift to move the measurement equipment to the required height.


### Baseplate 
The baseplate contains a Raspberry Pi. A 16 cells battery pack, in a 4S4P configuration, powers the system. 
The battery voltage ranges from 10 V to 16.2 V depending on the charge state. The battery back, based on Lithium Cobalt Oxide (LCO) technology,
has a capacity of 170Wh and can handle up to 480W peak power. Besides powering the rover, the battery pack
provides power to the peripherals and measurement equipment in order to conduct the experiments. When
the battery is low, the rover is able to navigate to its charging station, recharge and resume the measurements.

### Navigation
Marvelmind indoor RTLS is used as a positioning system. It has a precision of 2 cm.
Ultrasonic beacons are used to acquire the position of the mobile beacon (mounted on the baseplate). Four
beacons are fixed in the Techtile infrastructure. The position is determined by trilateration. To filter out
outliers, we make use of a Kalman filter. The recursive algorithm uses consecutive inputs to estimate the
current state. Based on the uncertainties of the inputs and the uncertainties of the previous estimates, the
current state is approximated. Besides localization, the rover features obstacle detection to mitigate crashing into objects in the room. 
Ultrasonic sensors mounted on the sides of the rover’s baseplate are used and have
a resolution of 3 mm and a range of 2 cm to 400 cm.

### Scissor lift.
The baseplate of the rover can navigate through the room in 2D space. By mounting the scissor
lift on top of the baseplate, the rover is able to measure in 3D space. The lift has a range of 55 cm
to 185 cm. The equipment can be mounted on top of the lift. Power is supplied to the equipment from the
battery pack. The scissor lift can be controlled with AT-commands. The height of the lift is determined by
the VL53L1X Time-of-Flight (ToF) sensor using a 940 nm invisible Class 1 laser. An advertised accuracy and
ranging error of ± 20 mm in both ambient light and dark light conditions.

 
