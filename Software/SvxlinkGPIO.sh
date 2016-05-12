#USING GPIO
#GPIO (General Purpose IO) is used to access hardware pins that are made available for example on an embedded
#system. Before starting to use a pin in SvxLink some setup need to be done in the operating system. The example
#below will set up pin 31 as an output for PTT use and pin 30 is used as input for squelch. Make sure that the
#commands are run as user root.


  #Enable the PTT pin for GPIO:
  echo 31 > /sys/class/gpio/export


  #Set the direction to output for the pin:
  echo out > /sys/class/gpio/gpio31/direction


  #Make sure that the svxlink user can write to the GPIO pin:
  chown svxlink /sys/class/gpio31/value


  #Enable the squelch pin for GPIO:
  echo 30 > /sys/class/gpio/export


  #Set the direction to input for the pin:
  echo in > /sys/class/gpio/gpio30/direction


  #Make sure that the svxlink user can read from the GPIO pin:
  chown svxlink /sys/class/gpio30/value

#These steps have to be performed whenever the system is rebooted. Putting the commands in a
#script is recommended.