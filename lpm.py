#!/usr/bin/python3
import time
import re
from datetime import datetime

gpio='/gpio/pin26/edge'

def setup_gpio(gpio):
    with open(gpio, "w") as f:
        f.write("rising")


#1Hz – 0.98 LPM
val=0
last_val=0
last_time=''
total_litres=0
sleep=10
Interrupts='/proc/interrupts'
# 41:        299  gpio-mxc  14 Edge      gpiolib
pattern = re.compile(r"^\s*41:\s*(\d+).*$");

setup_gpio(gpio)

while True:
    now = datetime.now()
    with open(Interrupts) as interrupts:
        lines = interrupts.readlines()
    #print ("lines %d" % len(lines))
    for line in lines:
        match = pattern.search(line)
        if match:
            val = int(match[1])
            #print('irqs %d' % val);
            if last_val == 0:
                print("Initialise last_val date %s %d" % (now, val));
                last_val = val
                last_time = now
                continue
            if val != last_val:
                edges = val - last_val
                diff_time = now - last_time
                hertz = (edges / diff_time.total_seconds())
                lpm = hertz / 0.98
                litres = lpm * diff_time.total_seconds() / 60
                total_litres = total_litres + litres
                #print('val %d edges %d seconds %d hertz %f' % (val, edges, diff_time.total_seconds(), hertz))
                print('total_litres %.1f liters %.1f lpm %.1f hertz %.1f edges %d' % (total_litres, litres, lpm, hertz, edges))
                last_val = val
                last_time = now
            continue
    time.sleep(10);
done
