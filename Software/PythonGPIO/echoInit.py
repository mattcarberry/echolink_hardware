# to run script in the background after closing ssh, in terminal,                                                     
# call "nohup echolinkInit.py &"                                                                                      

import RPi.GPIO as GPIO
import alsaaudio
import os
#set up GPIO using BCM numbering                                                                                      
GPIO.setmode(GPIO.BCM)
#setup GPIO using board numbering                                                                                     
#GPIO.setmode(GPIO.BOARD)                                                                                             
#setup GPIO pin as input, and enable internal pullup resistor                                                         
GPIO.setup(4, GPIO.IN,pull_up_down = GPIO.PUD_DOWN) # MUTE                                                            
#GPIO.setup(10, GPIO.IN,pull_up_down = GPIO.PUD_UP) # COS                                                             
#GPIO.setup(11, GPIO.OUT) #PTT                                                                                        

print "Script Running!"
print "Waiting for MUTE signal"

#Make sure that the svxlink user can write to the GPIO pin:                                                           
#os.chown(svxlink, /sys/class/gpio/gpio11/value)                                                                      

#Make sure that the svxlink user can read from the GPIO pin:                                                          
#os.chown(svxlink, /sys/class/gpio/gpio10/value)                                                                      

mixer = alsaaudio.Mixer('Speaker')

# to control muting and unmuting, use: Mixer.setmute(mute, channel)                                                   

#def my_callback(channel):                                                                                            
#    if GPIO.input(4):                                                                                                
#        mixer.setmute(1, 0)                                                                                          
#        print "Speaker muted!"                                                                                       
   # else:                                                                                                            
   #     mixer.setmute(0, 0)                                                                                          
   #     print "Speaker UN-muted!"                                                                                    


GPIO.add_event_detect(4, GPIO.RISING)

def my_callback():
    print 'PUSHED!'
    mixer.setmute(1, 0)
    print "Speaker Muted!"

GPIO.add_event_callback(4, my_callback)

#try:                                                                                                                 
#    GPIO.wait_for_edge(9, GPIO.BOTH)                                                                                 
#    print "edge detected! :)
#except KeyboardInterrupt:      
