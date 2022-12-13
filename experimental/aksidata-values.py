from ev3dev2.motor import LargeMotor, OUTPUT_A
from ev3dev2.power import PowerSupply
import csv

motor = LargeMotor(OUTPUT_A)
battery = PowerSupply()

motor.duty_cycle_sp = 100
motor.run_direct()

csvfile = open('/home/Repositories/frt-ev3-library/experimental/values.csv','w')
wrt = csv.writer(csvfile)
wrt.writerow()

while True:
    #read voltage, current
    mvolt = battery.measured_voltage
    mapms = battery.measured_current
    # write volts, ams to a new file

    # record pos
    #wait 1 sec

    # calculate speed from pos diff, in dps

    # record timestamp, speed, volts, amps separated by commas