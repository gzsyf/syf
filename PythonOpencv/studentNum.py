import cv2 as cv
import numpy as np

def nothing(x):
    pass

# ------------------ 鼠标响应函数 ----------------------
def mouse_chose_mask(event, x, y, flags, param):
    global img   # 原图片
    global lsPointsChoose, tpPointsChoose  # 存入选择的点 ls用于提取 tp用于画线
    global pointsCount  # 对鼠标按下的点计数
    global img2   # 传入图片的副本
    global flag   # 交换点标志位
    global C2

    img2 = img.copy()  # 保证每次都重新再原图画  避免画多了

    if event == cv.EVENT_LBUTTONDOWN:  # ---- 左键点击：选择点
        pointsCount = pointsCount + 1
        point1 = (x, y)  # 用于画圆的点
        cv.circle(img2, point1, 10, (0, 255, 0), 2)  # 画圆便于提醒

        # 将选取的点保存到list列表里
        lsPointsChoose.append([x, y])  # 用于转化为数组 提取多边形ROI
        tpPointsChoose.append((x, y))  # 用于画点

        if len(flag) != 1:  # 如果是多个ROI
            for k in range(len(flag) - 1):
                cv.line(img2, tpPointsChoose[flag[k]], tpPointsChoose[flag[k+1]-1], (0, 0, 255), 2)

            # 多个ROI 扫描算法
            for k in range(len(flag) - 1):
                for i in range(flag[k+1] - flag[k] - 1):
                    cv.line(img2, tpPointsChoose[flag[k] + i], tpPointsChoose[flag[k] + i + 1], (0, 0, 255), 2)  # 划线

        for i in range(len(tpPointsChoose) - flag[-1] - 1):
            cv.line(img2, tpPointsChoose[flag[-1] + i], tpPointsChoose[flag[-1] + i + 1], (0, 0, 255), 2)  # 划线

        # 最后展现多个ROI 的区域
        cv.imshow('src', img2)

    if event == cv.EVENT_MBUTTONDOWN:  # ---- 中键点击：选择点
        flag.append(len(tpPointsChoose))


    if event == cv.EVENT_LBUTTONDBLCLK:  # --- 双击: 结束选取

        mask = makemask(img)  # 运用生成掩模的方法

        mask2 = cv.cvtColor(mask, cv.COLOR_BGR2GRAY)  # 变成灰度图channel要1 应为方便后面寻找轮廓

        R, C, H = cv.findContours(mask2, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)  # 寻找轮廓

        cv.drawContours(img, C, -1, (0, 255, 0))  # 绘制轮廓

        a = np.array(C)
        for i in range(a.shape[0] - 1):
            C[0] = np.vstack((C[0], C[i + 1]))

        C2 = C[0]

        cv.destroyAllWindows()



# ------------ 制作掩模函数 -------------------------
def makemask(img):

    # 用于制作掩模的点
    global pts

    mask = np.zeros(img.shape, np.uint8) # 画一个黑色的图片

    # 下面是用于绘制多个ROI掩模的算法
    pts.append(np.array([lsPointsChoose[flag[-1]:]], np.int32))  # 掩膜的多边形列表
    pts[0] = pts[0].reshape((-1, 1, 2)) # reshape一下
    mask = cv.polylines(mask, [pts[0]], True, (255, 255, 255)) # 画多边形
    mask = cv.fillPoly(mask, [pts[0]], (255, 255, 255)) # 填充多边形

    if len(flag) != 1:
        for i in range(len(flag) - 1):
            pts.append(np.array([lsPointsChoose[flag[i]:flag[i+1]]], np.int32))
            pts[i + 1] = pts[i + 1].reshape((-1, 1, 2))
            mask = cv.polylines(mask, [pts[i + 1]], True, (255, 255, 255))  # 画多边形
            mask = cv.fillPoly(mask, [pts[i + 1]], (255, 255, 255))  # 填充多边形


    return mask

# ---------------------------------------------------------------
# 这两个是存储鼠标的坐标
lsPointsChoose = []
tpPointsChoose = []
# flag要先置一个0
flag = [0]
# 这是一个绘制各个ROI的点的数组
pts = []
pointsCount = 0
C2 = []


img = cv.imread('C://Users//as//Pictures//Saved Pictures//2.jpg')
image = img.copy()

cv.namedWindow('src')
cv.setMouseCallback('src', mouse_chose_mask)
cv.imshow('src', img)

cv.waitKey(0)
cv.destroyAllWindows()


cv.namedWindow('tiaozheng', 0)
cv.createTrackbar('YZ', 'tiaozheng', 1, 100, nothing)
cv.createTrackbar('MH', 'tiaozheng', 1, 20, nothing)

while True:
    # 这里是逐行把掩模背景变成白色的算法
    hang = image.shape[0] - 1
    lie = image.shape[1] - 1
    for i in range(0, lie):
        for j in range(0, hang):
            if cv.pointPolygonTest(C2, (i, j), False) != 1:
                image[j, i, :] = 255

    # 把输入图像灰度化
    gray = cv.cvtColor(image, cv.COLOR_RGB2GRAY)

    MH = cv.getTrackbarPos('MH', 'tiaozheng')

    # 将提出的东西模糊滤波 值越大模糊越厉害
    blured = cv.blur(gray, (MH, MH))


    YZ = cv.getTrackbarPos('YZ', 'tiaozheng')

    # 直接阈值化是对输入的单通道矩阵逐像素进行阈值分割。
    retval, dst = cv.threshold(blured, YZ, 255, cv.THRESH_BINARY)



    # 膨胀，白区域变大
    dst1 = cv.dilate(dst, None, iterations=1)
    # 腐蚀，白区域变小
    dst2 = cv.erode(dst1, None, iterations=2)

    # 扣出图片
    dst3 = cv.bitwise_not(dst2, dst2)

    # r, c, h = cv.findContours(dst3, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)  # 寻找轮廓
    #
    # for c1 in c:
    #     # 轮廓绘制方法一
    #     # boundingRect函数计算边框值，x，y是坐标值，w，h是矩形的宽和高
    #     x, y, w, h = cv.boundingRect(c1)
    #     # 在img图像画出矩形，(x, y), (x + w, y + h)是矩形坐标，(0, 255, 0)设置通道颜色，2是设置线条粗度
    #     cv.rectangle(image, (x, y), (x + w, y + h), (0, 255, 0), 2)

    # # 寻找连通区域
    # ret, maker = cv.connectedComponents(dst3)

    cv.imshow('haha', dst3)
    cv.imshow('haha2', image)

    if cv.waitKey(1) == 27:
        break

cv.destroyAllWindows()
