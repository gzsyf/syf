import cv2 as cv
import numpy as np
import os

def generate_img(dirname):

    # 人脸检测的时候的一个级联分类器 -- 给了人脸正面默认值的数据
    face_casecade = cv.CascadeClassifier(r'F:\PYrobot\opencv-master\data\haarcascades\haarcascade_frontalface_default.xml')

    # 判断是否为目录 如果不是创建目录
    if not os.path.isdir(dirname):
        os.makedirs(dirname)

    cap = cv.VideoCapture(0)
    count = 0

    while True:
        ret, frame = cap.read()

        # 把窗口缩小为原来二分之一
        x, y = frame.shape[0:2]
        small_frame = cv.resize(frame, (int(y/2), int(x/2)))

        # 提取副本
        result = small_frame.copy()

        # 灰度原图
        gray = cv.cvtColor(small_frame, cv.COLOR_BGR2GRAY)

        # 检测出所有人脸

        faces = face_casecade.detectMultiScale(gray, 1.3, 5)

        for x, y, w, h in faces:
            # 画一个矩形 -- 适合人脸
            result = cv.rectangle(result, (x, y), (x+w, y+h), (255, 0, 0), 2)

            # 从新构建一个图像 为 200*200 的像素
            f = cv.resize(gray[y:y+h, x:x+w], (200, 200))

            # 存20张
            if count < 20:
                cv.imwrite(dirname+'%s.pgm'%str(count), f)
                print(count)
                count += 1
        cv.imshow('face', result)

        if cv.waitKey(30) == 27:
            break
    cap.release()
    cv.destroyAllWindows()


generate_img(r'F:\PYrobot\pictureFace\syf\syf')
