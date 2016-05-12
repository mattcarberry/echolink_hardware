#/!usr/bin/python                                                                                                     
import RPi.GPIO as GPIO
import time
import logging
import logging.handlers
import argparse
import sys

LOG_FILENAME = "/tmp/ssr.log"
LOG_LEVEL = logging.INFO

parser = argparse.ArgumentParser(description="Simple power service in Pyhton")
parser.add_argument("-l", "--log", help="file to write log to (default '" + LOG_FILENAME + "')")

# If the log file is specified on the command line then override the default                                          
args = parser.parse_args()
if args.log:
   LOG_FILENAME = args.log

# Configure logging to log to a file, making a new file at midnight and keeping the last 3 day's data                 
# Give the logger a unique name (good practice)                                                                       
logger = logging.getLogger(__name__)
# Set the log level to LOG_LEVEL                                                                                      
logger.setLevel(LOG_LEVEL)
# Make a handler that writes to a file, making a new file at midnight and keeping 3 backups                           
handler = logging.handlers.TimedRotatingFileHandler(LOG_FILENAME, when="midnight", backupCount=3)
# Format each log message like this                                                                                   
formatter = logging.Formatter('%(asctime)s %(levelname)-8s %(message)s')
# Attach the formatter to the handler                                                                                 
handler.setFormatter(formatter)
# Attach the handler to the logger                                                                                    
logger.addHandler(handler)

prevBtn1 = 0
prevBtn2 = 0
prevBtn3 = 0

state1=0
state2=0
state3=0

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

GPIO.setup(23, GPIO.OUT)
GPIO.setup(24, GPIO.OUT)
GPIO.setup(25, GPIO.OUT)

GPIO.setup(17, GPIO.IN)
GPIO.setup(22, GPIO.IN)
GPIO.setup(27, GPIO.IN)

def state_change(state, ch):
   print "GPIO.%d state: %d." % (ch, state)
   if state == 1:
      GPIO.output(ch, GPIO.HIGH)
   else:
      GPIO.output(ch, GPIO.LOW)

def gpio_callback(id):
   global state1
   global state2
   global state3
   btn=GPIO.input(id)
   if btn==1:
      if id==17:
         state1 = not state1
         state_change(state1, 23)
      if id==27:
         state2 = not state2
         state_change(state2, 24)
      if id==22:
         state3 = not state3
         state_change(state3, 25)

GPIO.add_event_detect(17, GPIO.RISING, callback=gpio_callback, bouncetime=200)
GPIO.add_event_detect(27, GPIO.RISING, callback=gpio_callback, bouncetime=200)
GPIO.add_event_detect(22, GPIO.RISING, callback=gpio_callback, bouncetime=200)

while True:
   time.sleep(0.001)