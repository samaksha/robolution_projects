import cv2
import numpy as np
import serial
import time


ser = serial.Serial('COM9',9600,timeout=0.5)

time.sleep(2)

# capturing video through webcam
cap = cv2.VideoCapture(1)

font = cv2.FONT_HERSHEY_COMPLEX

while (1):
    _, img = cap.read()
    img = cv2.flip(img, 1)
    f = 0
    # converting frame(img i.e BGR) to HSV (hue-saturation-value)
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

    # defining the Range of Blue color
    blue_lower = np.array([99, 115, 160], np.uint8)
    blue_upper = np.array([110, 255, 255], np.uint8)

    # defining the Range of yellow color
    yellow_lower = np.array([22, 60, 160], np.uint8)
    yellow_upper = np.array([31, 255, 255], np.uint8)

    green_lower = np.array([46, 36, 102], np.uint8)
    green_upper = np.array([72, 255, 255], np.uint8)

   # finding the range of red,blue and yellow color in the image
   # red = cv2.inRange(hsv, red_lower, red_upper)
    blue = cv2.inRange(hsv, blue_lower, blue_upper)
    yellow = cv2.inRange(hsv, yellow_lower, yellow_upper)
    green = cv2.inRange(hsv, green_lower, green_upper)

    # Morphological transformation, Dilation
    kernal = np.ones((5, 5), "uint8")



    blue = cv2.dilate(blue, kernal)
    res1 = cv2.bitwise_and(img, img, mask=blue)

    yellow = cv2.dilate(yellow, kernal)
    res2 = cv2.bitwise_and(img, img, mask=yellow)

    green = cv2.dilate(green, kernal)
    res3 = cv2.bitwise_and(img, img, mask=green)


    # Tracking the Blue Color
    contours, hierarchy = cv2.findContours(blue, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    for pic, contour in enumerate(contours): #ASK
        area = cv2.contourArea(contour)
        if (area > 500):
            x, y, w, h = cv2.boundingRect(contour)
            img = cv2.rectangle(img, (x, y), (x + w, y + h), (255, 0, 0), 2)
            cv2.putText(img, "Blue color", (x-100, y+100), cv2.FONT_HERSHEY_SIMPLEX, 1.50, (255, 0, 0))

    # Tracking the yellow Color
    contours, hierarchy = cv2.findContours(yellow, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    for pic, contour in enumerate(contours):
        area = cv2.contourArea(contour)
        if (area > 500):
            x, y, w, h = cv2.boundingRect(contour)
            img = cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)
            cv2.putText(img, "yellow  color", (x-100, y+100), cv2.FONT_HERSHEY_SIMPLEX, 1.50, (0, 0, 255))

    # Tracking the green Color
    contours, hierarchy = cv2.findContours(green, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    for pic, contour in enumerate(contours):
        area = cv2.contourArea(contour)
        if (area > 500):
            x, y, w, h = cv2.boundingRect(contour)
            img = cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)
            cv2.putText(img, "Green  color", (x-100, y+100), cv2.FONT_HERSHEY_SIMPLEX, 1.50, (0, 0, 255))


    contours, _ = cv2.findContours(yellow, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    for cnt in contours:
        if cv2.contourArea(cnt) >= 500:
            approx = cv2.approxPolyDP(cnt, 0.025 * cv2.arcLength(cnt, True), True)
            cv2.drawContours(img, [approx], 0, (0), 5)
            x = approx.ravel()[0]
            y = approx.ravel()[1]
            if len(approx) > 0 and len(approx) < 7:
                cv2.putText(img, "Rectangle", (x, y-25), font, 1, (255,0,0))
                ser.write(str(5))
                print("yellow rec 75")
                f = 1 
                #time.sleep(0.5)
            elif len(approx)  >=7:
                cv2.putText(img, "Circle", (x, y-25), font, 1, (255,0,0))
                ser.write(str(4))
                print("yellow circle 60")
                f = 1 
            
                
                
    
    contours, _ = cv2.findContours(blue, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    for cnt in contours:
        if cv2.contourArea(cnt) >= 500:
            approx = cv2.approxPolyDP(cnt, 0.025 * cv2.arcLength(cnt, True), True)
            cv2.drawContours(img, [approx], 0, (0), 5)
            x = approx.ravel()[0]
            y = approx.ravel()[1]
            if len(approx) > 0 and len(approx) < 7:
                cv2.putText(img, "Rectangle", (x, y-25), font, 1, (255,0,0))
                ser.write(str(2))
                print("blue rec 30")
                f = 2
                #time.sleep(0.5)
                #print(ser.read())
            elif len(approx)>= 7:
                cv2.putText(img, "Circle", (x, y-25), font, 1, (255,0,0))
                ser.write(str(1))
                print("blue circle 15")
                f = 2
                
                #print(ser.read())

    contours, _ = cv2.findContours(green, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    for cnt in contours:
        if cv2.contourArea(cnt) >= 500:
            approx = cv2.approxPolyDP(cnt, 0.025 * cv2.arcLength(cnt, True), True)
            cv2.drawContours(img, [approx], 0, (0), 5)
            x = approx.ravel()[0]
            y = approx.ravel()[1]
            
            if len(approx) > 0 and len(approx) < 7:
                cv2.putText(img, "Rectangle", (x, y-25), font, 1, (255,0,0))
                print("green rectangle 45")
                ser.write(str(3))
                f=3
            elif len(approx) >= 6:
                cv2.putText(img, "Circle", (x, y-25), font, 1, (255,0,0))
                print("green circle 90")
                ser.write(str(6))
                f=3
    """if f>0 :
         time.sleep(4.75)
         f=0"""

    #print(ser.read())
    cv2.imshow('Original', img)
    k = cv2.waitKey(5)
    if k == 27:
        break

cv2.destroyAllWindows()
cap.release()