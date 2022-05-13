import cv2
import handTracking
import paho.mqtt.client as mqtt


lastPoint = handTracking.landmark(
    0, 960, 540
)
fingerMap = {
    "[False, False, False, False, False]":0,
    "[False, True, False, False, False]":1,
    "[False, True, True, False, False]":2,
    "[False, True, True, True, False]":3,
    "[False, True, True, True, True]":4,
    "[True, True, True, True, True]":5,
    "[True, False, False, False, True]":6,
    "[True, True, False, False, False]":7,
    "[True, True, True, False, False]":8,
    "[True, True, True, True, False]":9
}
currentMode = 0

client = mqtt.Client()
client.connect("192.168.9.130", 1883, 60)

def main():
    global currentMode
    wCam, hCam = 1920,1080
    cap = cv2.VideoCapture(0)  
    cap.set(3,wCam)
    cap.set(4,hCam)
    pTime = 0
    cTime = 0
    dector = handTracking.handDector()
    while True:
        success, img = cap.read()
        img = dector.findHands(img,draw=False)
        handList = dector.findPosition(img,draw=False)
        if len(handList) != 0:
            
            new = currentMode
            hand = handList[0]

            fingers = []

            # Thumb
            fingers.append(hand[5].y > hand[4].y)
            # 4 Fingers
            for finger in [8,12,16,20]:
                fingers.append(hand[finger].y < hand[finger-2].y)

            try:
                new = fingerMap[str(fingers)]
            except:  
                pass
            point = hand[0]

            if (point.x - lastPoint.x > 200):
                new = (new+1)%9
            elif (point.x - lastPoint.x < -200):
                new = (new+8)%9
            
            if new != currentMode:
                currentMode = new
                print(currentMode)
                client.publish("tcivs/box/rainbow",currentMode)

        cv2.putText(img,f"current mode = {currentMode}",(10,70),cv2.FONT_HERSHEY_PLAIN,3,(255,0,255),3)
        
        cv2.imshow("Image",img)
        if cv2.waitKey(1) == ord('q'):
            cap.release()
            cv2.destroyAllWindows()
            break



if __name__ == "__main__":
    main()