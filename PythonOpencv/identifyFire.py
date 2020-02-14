import cv2 as cv
import numpy as np
cap = cv.VideoCapture(0)

R_min = np.array([0,128,46])
R_max = np.array([5,255,255])
R2_min = np.array([156,128,46])
R2_max = np.array([180,255,255])
Y_min = np.array([15,128,46])
Y_max = np.array([50,255,255])

while True:

    ret, frame = cap.read()


    # 返回framed的维度0 1 2的长度
    x, y = frame.shape[0:2]
    # 改变视频窗口的大小
    small_frame = cv.resize(frame,(int(y/2),int(x/2)))
    cv.imshow('figure',frame)

    # small_frame创建副本
    scr = small_frame.copy()
    res = scr.copy()

    # 创建HSV图
    hsv = cv.cvtColor(scr,cv.COLOR_BGR2HSV)

    # 找出红色与黄色区域
    mask_R = cv.inRange(hsv,R_min,R_max)
    mask_R1 = cv.inRange(hsv, R2_min, R2_max)
    mask_Y = cv.inRange(hsv, Y_min, Y_max)

    # 制作掩模
    mask = cv.bitwise_or(mask_R1,mask_R)
    mask = cv.bitwise_or(mask,mask_Y)

    # 掀起掩模
    res = cv.bitwise_and(scr,scr,mask=mask)

    # h,w = res.shape[:2]
    # 将提出的东西模糊滤波 值越大模糊越厉害
    blured = cv.blur(res, (5, 5))

    # 将模糊的图片二值化
    m, bright = cv.threshold(blured, 10, 255, cv.THRESH_BINARY)

    # 将二值图灰度化
    gray = cv.cvtColor(bright,cv.COLOR_BGR2GRAY)
    # 构造特定大小的元素
    kernel = cv.getStructuringElement(cv.MORPH_RECT,(5,5))

    # 对灰度图连续开闭运算 消除小黑点与小孔洞
    opened = cv.morphologyEx(gray,cv.MORPH_OPEN,kernel)
    closed = cv.morphologyEx(opened,cv.MORPH_CLOSE,kernel)

    # 找出轮廓
    R, C, H = cv.findContours(closed, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)

    cv.drawContours(scr, C, -1, (255, 0, 0))

    cv.imshow('Final', scr)

    if cv.waitKey(50) == 27:
        break

cap.release()
cv.destroyAllWindows()
