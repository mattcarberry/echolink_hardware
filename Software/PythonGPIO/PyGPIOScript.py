#/!usr/bin/python                                                                                                     
import RPi.GPIO as GPIO
import time
import logging
import logging.handlers
import argparse
import sys
import os

#**********************************************************************

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
#**********************************************************************
# Make a class we can use to capture stdout and sterr in the log
class MyLogger(object):
        def __init__(self, logger, level):
                """Needs a logger and a logger level."""
                self.logger = logger
                self.level = level

        def write(self, message):
                # Only log if there is a message (not just a new line)
                if message.rstrip() != "":
                        self.logger.log(self.level, message.rstrip())

# Replace stdout with logging to file at INFO level
sys.stdout = MyLogger(logger, logging.INFO)
# Replace stderr with logging to file at ERROR level
sys.stderr = MyLogger(logger, logging.ERROR)
#**********************************************************************
#create mixer object
mixer = alsaaudio.Mixer('Speaker') 
# Setup GPIO input
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(27, GPIO.IN,pull_up_down = GPIO.PUD_DOWN) # MUTE channel input 
#**********************************************************************
def gpio_callback(27):
   muteState=GPIO.input(27)
   print "GPIO Mute Input state: %d." % (muteState)
   if muteState == 1:
   	  mixer.setmute(1, 0) 
   	  print "Rising Edge -> Mixer Muted!" 
   else:
      mixer.setmute(0, 0) 
      print "Falling Edge -> Mixer Unmuted!" 
#**********************************************************************
GPIO.add_event_detect(27, GPIO.BOTH, callback=gpio_callback, bouncetime=200)

while True:
   #time.sleep(0.001)