
identifyFire.py -- 识别火焰
identifyLine.py -- 识别火焰
identifyWhiteBlack -- 识别黑色与白色
openCamera -- 打开相机

perpleNum.py -- 人脸识别人数 ，这个要安装face_recognition模块
faceRecognition.py -- 人脸识别 ，这个要安装face_recognition模块


identifyPedestrain.py -- 行人检测 ， 用到非极大抑制算法
https://blog.csdn.net/ZOU19900101/article/details/79125643?utm_source=blogxgwz1


studentNum.py -- 教室人数

collectFace.py -- 收集人脸
identifyFace.py -- 识别人脸
在分类器中有4种 -- alt、alt2、alt_tree、default 其中 alt 效果最好
然后可以更改 detectMultiScale()的参数 scaleFactor 表示每次图像尺寸减小的比例 -- 1.0~1.99999 越大越检不出 越小越容易误检 minNeighbors 一个目标至少要被检测到n次才算真 -- 越大越检不出 越小越容易误检

