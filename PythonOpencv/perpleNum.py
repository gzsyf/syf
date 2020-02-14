#  识别图片中的所有人脸并显示出来

# 导入face_recogntion模块，可用命令安装 pip install face_recognition
import face_recognition
import cv2


# 将jpg文件加载到numpy 数组中

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()

    x, y = frame.shape[0:2]

    frame = cv2.resize(frame, (int(y / 3), int(x / 3)))

    rgb_frame = frame[:, :, ::-1]


    # 使用默认的给予HOG模型查找图像中所有人脸
    # 这个方法已经相当准确了，但还是不如CNN模型那么准确，因为没有使用GPU加速
    t1 = cv2.getTickCount()

    face_locations = face_recognition.face_locations(rgb_frame)

    t2 = cv2.getTickCount()
    time = (t2 - t1) / cv2.getTickFrequency()
    print(time)
    # 使用CNN模型
    # face_locations = face_recognition.face_locations(rgb_frame, number_of_times_to_upsample=0, model="cnn")

    # 打印：我从图片中找到了 多少 张人脸
    print("I found {} face(s) in this photograph.".format(len(face_locations)))

    # 循环找到的所有人脸
    for face_location in face_locations:
        # 打印每张脸的位置信息
        top, right, bottom, left = face_location
        # print("A face is located at pixel location Top: {}, Left: {}, Bottom: {}, Right: {}".format(top, left, bottom, right))
        # 指定人脸的位置信息，然后显示人脸图片
        cv2.rectangle(frame, (left, top), (right, bottom), (0, 0, 255), 2)

    cv2.imshow('tuxiang', frame)
    if cv2.waitKey(30) == 27:
        break

cap.release()
cv2.destroyAllWindows()

