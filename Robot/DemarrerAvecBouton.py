import RPi.GPIO as GPIO
import time
import os

#adjust for where your switch is connected
boutonJaunePin = 17

GPIO.setmode(GPIO.BCM)
GPIO.setup(boutonJaunePin,GPIO.IN)

while True:
  if (GPIO.input(boutonJaunePin)):
    os.system("make -C ~/Documents/Projet-RepoGit/gsd/Robot run")
