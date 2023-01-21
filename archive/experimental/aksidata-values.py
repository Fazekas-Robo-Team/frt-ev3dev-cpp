from ev3dev2.motor import LargeMotor, OUTPUT_A
from ev3dev2.power import PowerSupply
from time import gmtime, strftime
from time import sleep
import csv

motor = LargeMotor(OUTPUT_A)
battery = PowerSupply()

motor.duty_cycle_sp = 100
motor.run_direct()

csvfile = open('/home/Documents/values.csv','w', encoding='UTF-8')
wrt = csv.writer(csvfile)


while battery.max_voltage > 6:
    
    #read voltage, current
    mvolt = battery.measured_voltage
    mapms = battery.measured_current   
    
    # record pos
    pos = motor.position
    #wait 1 sec
    sleep(1)
    # calculate speed from pos diff, in dps
    diff=abs(pos - motor.position)
    # record timestamp, speed, volts, amps separated by commas
    wrt.writerow(strftime("%H:%M:%S", gmtime()), diff, mvolt, mapms)
    print("data_recorded")

csvfile.close()