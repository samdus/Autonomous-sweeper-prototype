import RPi.GPIO as GPIO
import time
import os

def callback_rouge:
    os.system("sudo pkill ExempleCom &")
    os.system("stty -F /dev/ttyACM0 -hupcl &")

def callback_jaune:
  os.system("make -C ~/Documents/Projet-RepoGit/gsd/Robot run &")

boutonRougePin = 19
boutonJaunePin = 17

GPIO.setmode(GPIO.BCM)
GPIO.setup(boutonRougePin, GPIO.IN)
GPIO.setup(boutonJaunePin, GPIO.IN)
  
GPIO.add_event_detect(boutonRougePin, GPIO.RISING, bouncetime=200)
GPIO.add_event_detect(boutonJaunePin, GPIO.RISING, bouncetime=200)

GPIO.add_event_callback(boutonRougePin, callback_rouge)
GPIO.add_event_callback(boutonJaunePin, callback_jaune)
