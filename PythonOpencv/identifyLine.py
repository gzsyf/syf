import cv2 as cv
import  numpy as np
cap = cv.VideoCapture(0)

while True:

    ret , frame = cap.read()
    #返回framed的维度0 1 2的长度
    x,y = frame.shape[0:2]
    #改变视频窗口的大小
    small_frame = cv.resize(frame,(int(y/2),int(x/2)))

    cv.imshow('small',small_frame)

    #改为灰度图
    gray = cv.cvtColor(small_frame, cv.COLOR_BGR2GRAY)

    #从灰度图中找出边缘图
    edges = cv.Canny(gray,50,120)

    minLineLength = 10
    maxLineGap = 5

    #霍夫变换直线检测 参数3:阀值，低于会忽略 参数4:最小直线长度 参数5:最大线间隙
    lines = cv.HoughLinesP(edges ,1,np.pi /180,100,minLineLength,maxLineGap)


    if lines == 0 or len(lines) ==0:
        continue
    for x1,y1,x2,y2 in lines[0]:
        cv.line(small_frame,(x1,y1),(x2,y2),(0,255,0),2)

    cv.imshow('Final',small_frame)



    if cv.waitKey(50) == 27:
        break

cap.release()
cv.destroyAllWindows()
