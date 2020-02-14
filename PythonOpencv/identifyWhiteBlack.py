import cv2 as cv
import numpy as np


# 全局阈值
def threshold_demo(image):
    gray = cv.cvtColor(image, cv.COLOR_RGB2GRAY)  # 把输入图像灰度化
    # 直接阈值化是对输入的单通道矩阵逐像素进行阈值分割。
    ret, binary = cv.threshold(gray, 0, 255, cv.THRESH_BINARY | cv.THRESH_TRIANGLE)
    print("threshold value %s"%ret)
    cv.namedWindow("binary0", cv.WINDOW_NORMAL)
    cv.imshow("binary0", binary)


# 局部阈值
def local_threshold(image):
    gray = cv.cvtColor(image, cv.COLOR_RGB2GRAY)  # 把输入图像灰度化
    # 自适应阈值化能够根据图像不同区域亮度分布，改变阈值
    binary = cv.adaptiveThreshold(gray, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C,cv.THRESH_BINARY, 25, 10)
    cv.namedWindow("binary1", cv.WINDOW_NORMAL)
    cv.imshow("binary1", binary)


# 用户自己计算阈值
def custom_threshold(image):
    gray = cv.cvtColor(image, cv.COLOR_RGB2GRAY)  # 把输入图像灰度化
    h, w = gray.shape[:2]                         # 拿出灰度图的1 2维的元素个数
    m = np.reshape(gray, [1, w*h])                # 把灰度图弄成一维的数值
    mean = m.sum()/(w*h)                          # 把所有元素相加 ÷ 元素个数 得到平均值
    print("mean:", mean)
    ret, binary = cv.threshold(gray, mean, 255, cv.THRESH_BINARY)
    cv.namedWindow("binary2", cv.WINDOW_NORMAL)
    cv.imshow("binary2", binary)


# -----------------自制二值化1(找出黑色轮廓)---------------------
def threshold_test1(image):

    # 把输入图像灰度化
    gray = cv.cvtColor(image, cv.COLOR_RGB2GRAY)

    # 将提出的东西模糊滤波 值越大模糊越厉害
    blured = cv.blur(gray, (5, 5))

    # 直接阈值化是对输入的单通道矩阵逐像素进行阈值分割。
    ret, binary = cv.threshold(blured, 35, 255, cv.THRESH_BINARY)

    # 将二值图灰度化
    # gray = cv.cvtColor(binary, cv.COLOR_BGR2GRAY)

    # 构造特定大小的元素
    kernel = cv.getStructuringElement(cv.MORPH_RECT, (5, 5))

    # 对灰度图连续开闭运算 消除小黑点与小孔洞
    opened = cv.morphologyEx(binary, cv.MORPH_OPEN, kernel)
    closed = cv.morphologyEx(opened, cv.MORPH_CLOSE, kernel)

    # 找出轮廓
    R, C, H = cv.findContours(closed, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)

    # 画出轮廓
    cv.drawContours(image, C, -1, (255, 0, 0), thickness=5)

    cv.namedWindow("binary0", cv.WINDOW_NORMAL)
    cv.imshow("binary0", image)


# -----------------自制二值化1(判断上面左右有没有黑色)---------------------
def threshold_test2(image):

    # 把输入图像灰度化
    gray = cv.cvtColor(image, cv.COLOR_RGB2GRAY)

    # 将提出的东西模糊滤波 值越大模糊越厉害
    blured = cv.blur(gray, (5, 5))

    # 直接阈值化是对输入的单通道矩阵逐像素进行阈值分割。
    ret, binary = cv.threshold(blured, 0, 255, cv.THRESH_BINARY | cv.THRESH_OTSU)

    # 构造特定大小的元素
    kernel = cv.getStructuringElement(cv.MORPH_RECT, (5, 5))

    # 对灰度图连续开闭运算 消除小黑点与小孔洞
    opened = cv.morphologyEx(binary, cv.MORPH_OPEN, kernel)
    closed = cv.morphologyEx(opened, cv.MORPH_CLOSE, kernel)

    # 计算左上角的黑色区域
    countL = 0
    for i in range(58, 108):
        for j in range(50):
            if closed[i, j] == 0:
                countL = countL + 1
                continue
            else:
                break

    # 计算左上角的黑色区域
    countR = 0
    for i in range(58, 108):
        for j in range(639, 589, -1):
            if closed[i, j] == 0:
                countR = countR + 1
                continue
            else:
                break

    cv.namedWindow("binary0", cv.WINDOW_NORMAL)
    cv.imshow("binary0", image)

    return countL, countR


# -----------------自制二值化1(判断联通区域)---------------------
def threshold_test3(image):

    # 把输入图像灰度化
    gray = cv.cvtColor(image, cv.COLOR_RGB2GRAY)

    # 将提出的东西模糊滤波 值越大模糊越厉害
    blured = cv.blur(gray, (5, 5))

    # 直接阈值化是对输入的单通道矩阵逐像素进行阈值分割。
    ret, binary = cv.threshold(blured, 0, 255, cv.THRESH_BINARY | cv.THRESH_OTSU)

    # 构造特定大小的元素
    kernel = cv.getStructuringElement(cv.MORPH_RECT, (5, 5))

    # 对灰度图连续开闭运算 消除小黑点与小孔洞
    opened = cv.morphologyEx(binary, cv.MORPH_OPEN, kernel)
    closed = cv.morphologyEx(opened, cv.MORPH_CLOSE, kernel)

    ret, maker = cv.connectedComponents(closed)

    cv.namedWindow("binary0", cv.WINDOW_NORMAL)
    cv.imshow("binary0", closed)

    return ret


# -----------------自制二值化1(中心点)---------------------
def threshold_test4(image):

    # 把输入图像灰度化
    gray = cv.cvtColor(image, cv.COLOR_RGB2GRAY)

    gray = cv.blur(gray, (5, 5))

    # 大津法二值化
    retval, dst = cv.threshold(gray, 0, 255, cv.THRESH_OTSU)

    # # 腐蚀，白区域变小
    # dst = cv.erode(dst, None, iterations=6)
    # 膨胀，白区域变大
    dst = cv.dilate(dst, None, iterations=8)
    # 腐蚀，白区域变小
    dst = cv.erode(dst, None, iterations=2)

    # #---------------------------------------------------------------
    # # 单看第400行的像素值
    # color = dst[100]
    #
    # # 找到白色的像素点个数
    # white_count = np.sum(color == 255)
    # # 找到白色的像素点索引
    # white_index = np.where(color == 255)
    #
    # # 防止white_count=0的报错
    # if white_count == 0:
    #     white_count = 1
    #
    # if len(white_index[0]):
    #     # 找到白色像素的中心点位置
    #     center = (white_index[0][white_count - 1] + white_index[0][0]) / 2
    #
    #     # 计算出center与标准中心点的偏移量
    #     direction = int(center - 320)
    #
    #     if direction >= 0:
    #         cv.circle(dst, (400, 100), 5, (0, 255, 0), 5)
    #
    #     if direction < 0:
    #         cv.circle(dst, (100, 100), 5, (0, 255, 0), 5)
    #     # 画圆
    #     # cv.circle(dst, (direction, 200), 5, (0, 255, 0), 5)
    # else:
    #     pass

    # # ----------------------------------------------------------------------------

    # color = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    # white_count = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    # white_index = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    # center = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    #
    # height, width = image.shape[:2]
    # height_piece = int(height / 10) - 1
    #
    # # 取多行的像素
    # for i in range(0, 11):
    #     color[i] = dst[i * height_piece]
    #
    # # 找到白色的像素点个数
    # for i in range(0, 11):
    #     white_count[i] = np.sum(color[i] == 255)
    #
    # # 找到白色的像素点索引
    # for i in range(0, 11):
    #     white_index[i] = np.where(color[i] == 255)
    #
    # # 防止white_count=0的报错
    # for i in range(0, 11):
    #     if white_count[i] == 0:
    #         white_count[i] = 1
    #
    # # 找到白色像素的中心点位置
    # for i in range(0, 11):
    #     if len(white_index[i][0]):
    #         center[i] = int((white_index[i][0][white_count[i] - 1] + white_index[i][0][0]) / 2)
    #     else:
    #         center[i] = 0
    #
    #
    # # # 画跟踪直线
    # # for i in range(0, 10):
    # #     cv.line(dst, (center[i], i * height_piece), (center[i + 1], (i+1) * height_piece), (0, 255, 0), thickness=2)
    #
    # cv.line(dst, (center[9], 9 * height_piece), (center[9 + 1], (9 + 1) * height_piece), (0, 255, 0), thickness=2)
    #
    # k = center[10]-center[9]
    # print('%s' % np.float16(k))

    # # ----------------------------------------------------------------------------

    # color = np.zeros((1,2))
    # white_count = [0, 0]
    # white_index = [0, 0]
    # center = [0, 0]
    #
    # height = image.shape[0]
    #
    # # 取多行的像素
    # for i in range(0, 2):
    #     color[i] = dst[height - 10 - (i*20)]
    #
    # # 找到白色的像素点个数
    # for i in range(0, 2):
    #     white_count[i] = np.sum(color[i] == 255)
    #
    # # 找到白色的像素点索引
    # for i in range(0, 2):
    #     white_index[i] = np.where(color[i] == 255)
    #
    # # 防止white_count=0的报错
    # for i in range(0, 2):
    #     if white_count[i] == 0:
    #         white_count[i] = 1
    #
    # # 找到白色像素的中心点位置
    # for i in range(0, 2):
    #     if len(white_index[i][0]):
    #         center[i] = int((white_index[i][0][white_count[i] - 1] + white_index[i][0][0]) / 2)
    #     else:
    #         center[i] = 0
    #
    # cv.line(dst, (center[1], height - 30), (center[0], height - 10), (0, 255, 0), thickness=2)
    # cv.circle(dst, (center[0], height - 10), 5, (0, 255, 0), 5)
    #
    # k = center[0]-center[1]
    #
    # print(k)

    # -----------------------------------------------
    color = [0 for i in range(180)]
    white_count = [0 for i in range(180)]

    for i in range(1, 180):
        color[i] = dst[i]

    # 找到白色的像素点个数
    for i in range(1, 180):
        white_count[i] = np.sum(color[i] == 255)

    for i in range(1, 180):
        if white_count[i] == 0:
            print(i)

    # ----------------------------------------------------

    cv.imshow("binary0", dst)

