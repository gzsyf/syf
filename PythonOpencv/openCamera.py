import cv2 as cv
import numpy as np
import identifyWandB as wb
import resizeWindows as rw


cap = cv.VideoCapture(1)
while True:
    ret, img = cap.read()
    # frame = cv.flip(frame, 1)
    # frame = rw.resize(frame,480,180)

    # ------------test2测试段-------------
    # (l, r) = wb.threshold_test2(frame)
    #
    # if l == 2500:
    #     print('left')
    # if r == 2500:
    #     print('right')

    # wb.threshold_test4(frame)

    cv.imshow("hahaah",img)

    if cv.waitKey(60) == 27:
        break

cap.release()
cv.destroyAllWindows()
