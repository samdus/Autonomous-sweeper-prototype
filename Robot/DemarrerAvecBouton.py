import RPi.GPIO as GPIO
import time
import os
from threading import Thread

def threaded_function():
  #adjust for where your switch is connected
  boutonJaunePin = 17

  GPIO.setmode(GPIO.BCM)
  GPIO.setup(boutonJaunePin,GPIO.IN)

  while True:
    if (GPIO.input(boutonJaunePin)):
      os.system("make -C ~/Documents/Projet-RepoGit/gsd/Robot run")

thread = Thread(target = threaded_function)
thread.start()
