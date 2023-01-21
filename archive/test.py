#!/usr/bin/env micropython

from ev3dev2.sensor.lego import ColorSensor
import time
color = ColorSensor("in2")

start = time.time()
for i in range(1e4):
    color.reflected_light_intensity
print("time elapsed (micropython):", time.time() - start)