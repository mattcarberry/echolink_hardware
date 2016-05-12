
#!/usr/bin/env python

# to run script in the background after closing ssh, in terminal,
# call "nohup echolinkInit.py &"

import RPi.GPIO as GPIO
import alsaaudio
import logging
import logging.handlers
import argparse
import sys
import os
import time
import shutil

#set up GPIO using BCM numbering
GPIO.setmode(GPIO.BCM)

  #Make sure that the svxlink user can write to the GPIO pin:
GPIO.setup(17, GPIO.OUT,pull_up_down = GPIO.PUD_DOWN) # COS
os.system("chown svxlink /sys/class/gpio/gpio17/value")

GPIO.setup(27, GPIO.IN,pull_up_down = GPIO.PUD_UP) # COS
os.system("chown svxlink /sys/class/gpio/gpio27/value")

#setup GPIO pin as input, and enable internal pullup resistor                     
GPIO.setup(4, GPIO.IN,pull_up_down = GPIO.PUD_DOWN) # MUTE  

os.system("svxlink")

mixer = alsaaudio.Mixer('Speaker')

# to control muting and unmuting, use: Mixer.setmute(mute, channel)

def my_callback(channel):
    if GPIO.input(4):
        mixer.setmute(1, 0)
 #       print "Speaker muted!"
    else:
        mixer.setmute(0, 0)
 #       print "Speaker UN-muted!"

GPIO.add_event_detect(4, GPIO.BOTH, callback=my_callback)


while True:
        time.sleep(0.001)

GPIO.cleanup()