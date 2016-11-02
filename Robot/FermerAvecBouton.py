import RPi.GPIO as GPIO
import time
import os

#adjust for where your switch is connected
boutonRougePin = 19

GPIO.setmode(GPIO.BCM)
GPIO.setup(boutonRougePin,GPIO.IN)

while True:
  if (GPIO.input(boutonRougePin)):
    os.system("sudo pkill ExempleCom")
    os.system("stty -F /dev/ttyACM0 -hupcl")

