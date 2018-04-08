import numpy as np
import cv2
import serial
import RPi.GPIO as GPIO
import time

# multiple cascades: https://github.com/Itseez/opencv/tree/master/data/haarcascades

#https://github.com/Itseez/opencv/blob/master/data/haarcascades/haarcascade_frontalface_default.xml
hand_cascade = cv2.CascadeClassifier('hand.xml')
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
pinOut = 17
GPIO.setup(pinOut,GPIO.OUT)
ser = serial.Serial('/dev/ttyUSB0')


def translate(val,lefMin,lefMax,riMin,riMax):
	lefSpan = lefMax - lefMin
	rigSpan = riMax - riMin
	value = float(val-lefMin)/float(lefSpan)
	return riMin + (value*rigSpan)


def sendData(xVal,yVal):
	#GPIO.output(pinOut,GPIO.HIGH)
	sendDat = 'C'+str(xVal)+'B'+str(yVal)
	ser.write(sendDat)
	print sendDat
	time.sleep(.5)
	#GPIO.output(pinOut,GPIO.LOW)

cap = cv2.VideoCapture(0)

while 1:
    ret, img = cap.read()
    img = cv2.resize(img,(0,0),fx=0.5,fy=0.5) 

    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    hands = hand_cascade.detectMultiScale(gray, 1.1, 5)
    lw,lh = img.shape[:2]
    for (x,y,w,h) in hands:
        cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,0),2)
	yVal = translate(lh-y,0,lh,0,9)
	xVal = translate(x,0,lw,0,9)
	sendData(int(xVal),int(yVal))

    cv2.imshow('img',img)
    k = cv2.waitKey(5) & 0xff
    if k == 27:
        break

ser.close()
cap.release()
cv2.destroyAllWindows()
