import os
import sys      # 与交互器强烈交互
import cv2 as cv
import numpy as np


# 读取图片集的图片 然后打包返回一个字典
def read_images(path, sz=None):
    c = 0                   # 骗号
    X, y = [], []           # 图像集
    names = []              # 姓名

    # 这两个循环 是找出 path 里所有文件的路径(包括路径的子路径)
    for dirname, dirnames, filenames in os.walk(path):
        for subdirname in dirnames:
            subject_path = os.path.join(dirname, subdirname)

            # 查找 sub路径 下的 filename文件
            for filename in os.listdir(subject_path):

                try:
                    if filename == "directory":
                        continue

                    # 路径拼接
                    filepath = os.path.join(subject_path, filename)

                    # 先将图片变成灰度 再返回
                    im = cv.imread(os.path.join(subject_path, filename), cv.IMREAD_GRAYSCALE)

                    if im is None:
                        print("image" + filepath + "is None")

                    # 参数sz 改变图片的大小
                    if sz is not None:
                        im = cv.resize(im, sz)

                    # 将X 装入图片im的数组形式
                    X.append(np.asarray(im, dtype=np.uint8))
                    y.append(c)
                except:
                    print("unexpected error")
                    raise
            c = c + 1
            names.append(subdirname)

    return [names, X, y]


def face_rec():

    read_dir = r'F:\PYrobot\pictureFace'
    [names, X, y] = read_images(read_dir)

    # 将y变成一个32位数组
    y = np.asarray(y, dtype=np.int32)

    # 训练人脸
    model = cv.face_EigenFaceRecognizer.create()
    model.train(np.asarray(X), np.asarray(y))

    face_casecade = cv.CascadeClassifier(r'F:\PYrobot\opencv-master\data\haarcascades\haarcascade_frontalface_default.xml')

    cap = cv.VideoCapture(0)

    while True:
        ret, frame = cap.read()


        x, y = frame.shape[0:2]

        small_frame = cv.resize(frame, (int(y / 2), int(x / 2)))

        # 提取副本
        result = small_frame.copy()

        # 灰度原图
        gray = cv.cvtColor(small_frame, cv.COLOR_BGR2GRAY)

        # 检测出所有人脸
        faces = face_casecade.detectMultiScale(gray, 1.3, 5)

        for x, y, w, h in faces:
            # 画一个矩形 -- 适合人脸
            result = cv.rectangle(result, (x, y), (x+w, y+h), (255, 0, 0), 2)

            # 提取感兴趣区域
            roi = gray[x:x+w, y:y+h]

            try:
                roi = cv.resize(roi, (200,200), interpolation=cv.INTER_LINEAR)

                # 预言人脸是谁
                [p_label, p_confidence] = model.predict(roi)
                cv.putText(result, names[p_label], (x, y-20), cv.FONT_HERSHEY_SIMPLEX, 1, 255, 2)

            except:
                continue

        cv.imshow('face', result)

        if cv.waitKey(30) == 27:
            break
    cap.release()
    cv.destroyAllWindows()

face_rec()
