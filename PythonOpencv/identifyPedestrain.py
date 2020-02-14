import cv2
import numpy as np
import imutils
from imutils.object_detection import non_max_suppression



# 获取hog检测器对象
hog = cv2.HOGDescriptor()
# 设置检测人的默认检测器
hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())

# 打开摄像头
# cap = cv2.VideoCapture(1)


cap = cv2.VideoCapture('D://网页下载//people.mp4')
while(cap.isOpened()):
    ret, frame = cap.read()

    # 改变图片大小
    image = imutils.resize(frame, width=min(400, frame.shape[1]))
    orig = image.copy()

    # 开启检测分类器 rects就是检测到的框框位置
    (rects, weights) = hog.detectMultiScale(image, winStride=(4, 4), padding=(8, 8), scale=1.03)

    # 应用非极大抑制方法，通过设置一个阈值来抑制那些重叠的边框
    rects = np.array([[x, y, x + w, y + h] for (x, y, w, h) in rects])
    pick = non_max_suppression(rects, probs=None, overlapThresh=0.65)

    # 打印行人个数
    print(pick.shape[0])

    # 给行人画框
    for (xA, yA, xB, yB) in pick:
        cv2.rectangle(image, (xA, yA), (xB, yB), (0, 255, 0), 2)

    cv2.imshow("person detection", image)

    if cv2.waitKey(30) == 27:
        break
cap.release()
cv2.destroyAllWindows()
