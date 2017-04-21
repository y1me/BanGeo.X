# BanGeo.X
Embedded software based on pic16f18323.
the aim was to retrofit a wheel alignment system, FACOM GTR530.
Replace old electronics by a modern pcb with pic16f18323 (control), ads1115 (data acquisition), mcp1631 (battery management) and HC-05 (bluetooth connectivity).

Communication to PC or android device with bluetooth serial port.
Protocol :
first byte =  "C" : command, "R" : answer
second = "X" id
third (answer)  = "X" byte answer length
  
  Send = "CA\n"  : Device identification
  Recv = "RA3HFR\n" : H : head, P : Plate, F : front, B : back, R : right, L : left

  Send = "CB\n"  : disable charge mode
  Recv = nothing

  Send = "CC\n"  : enable charge mode
  Recv = nothing

  Send = "CD*value*\n"  : set led, valid value: 0bxxxx00xx
  Recv = nothing

  Send = "CE*value*\n"  : set ADS1115 Mux, valid value: AIN0(0x40), AIN1(0x50), AIN2(0x60), AIN3(0x70)
  Recv = nothing

  Send = "CF\n"  : get battery temperature and voltage
  Recv = "RF4*Tlsb* *Tmsb* *Vlsb* *Vmsb*\n" : Temperature = mcp9701 10bits value, Voltage = ratio 0.42 10bits value, Vref 4.096

  Send = "CG\n"  : get 4 sensors values
  Recv = "RF8*Sensor0lsb* *Sensor0msb* *Sensor1lsb* *Sensor1msb* *Sensor2lsb* *Sensor2msb* *Sensor3lsb* *Sensor3msb*\n" :
    Sensor value coded on 15bits with Vref = 4.096V

  Send = "CH*value*\n"  : Write calibration value in EEprom
  Recv = nothing
    Value to send must be a 128 contiguous set. 
    Value written in eeprom depend on value modulo 3 result 
    Start with value '0'
    '0' is address for sensor 0 zero, '1' is address for sensor 1 zero,etc...
    '3' is address for tilt sensor zero (HW : plug tilt on S3 connector)

  Send = "CI*value*\n clear led, valid value: 0bxxxx00xx
  Recv = nothing

  Send = "CK*value*\n"  : Read calibration value in EEprom
  Recv = "RK2*Caliblsb* *Calibmsb*\n" 

Bluetooth connection :

pair device
sudo rfcomm bind /dev/rfcomm0 <device address>



