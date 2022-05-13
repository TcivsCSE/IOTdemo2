import cv2
import handTracking

lastPoint = handTracking.landmark(
    0, 960, 540
)
def main():
    global lastPoint
    wCam, hCam = 1280,720
    cap = cv2.VideoCapture(0)  
    cap.set(3,wCam)
    cap.set(4,hCam)
    pTime = 0
    cTime = 0
    dector = handTracking.handDector()
    
    while True:
        success, img = cap.read()
        img = dector.findHands(img)
        handList = dector.findPosition(img,draw=False)
        if len(handList) != 0:
            point = handList[0][0]

            if (point.x - lastPoint.x > 200):
                print("right")
            elif (point.x - lastPoint.x < -200):
                print("left")
            
            lastPoint = point


        cv2.imshow("Image",img)
        if cv2.waitKey(1) == ord('q'):
            cap.release()
            cv2.destroyAllWindows()
            break



if __name__ == "__main__":
    main()