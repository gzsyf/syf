# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
import matplotlib.pyplot as plt

from keras.applications.vgg16 import VGG16
# 导入keras预处理模块
from keras.preprocessing import image
# 导入preprocess_input
from keras.applications.vgg16 import preprocess_input, decode_predictions
import numpy as np

def percent(value):
    return '%.2f%%' % (value * 100)

# include_top=True，表示會載入完整的 VGG16 模型，包括加在最後3層的卷積層
# include_top=False，表示會載入 VGG16 的模型，不包括加在最後3層的卷積層，通常是取得 Features
# 若下載失敗，請先刪除 c:\<使用者>\.keras\models\vgg16_weights_tf_dim_ordering_tf_kernels.h5
model = VGG16(weights='imagenet', include_top=True)


# 要辨别的图像的路径
img_path = 'frog.jpg'

# 载入图像并转化为224*224的格式
img = image.load_img(img_path, target_size=(224, 224))

# 转化为浮点型
x = image.img_to_array(img) 
# 转化为张量size为(1, 224, 224, 3) 的四维张量
x = np.expand_dims(x, axis=0)
# 预处理图像(类似于归一化) 能够加快图像的处理速度
x = preprocess_input(x)

# 預測，取得features，維度為 (1,1000)
features = model.predict(x)

# 取得前 五 個最可能的類別及機率
pred = decode_predictions(features, top=5)[0]


#整理预测结果,value
values = []
bar_label = []
for element in pred:
    values.append(element[2])
    bar_label.append(element[1])

#绘图并保存
fig=plt.figure(u"Top-5 预测结果")
ax = fig.add_subplot(111) 
ax.bar(range(len(values)), values, tick_label=bar_label, width=0.5, fc='g')
ax.set_ylabel(u'probability') 
ax.set_title(u'Top-5') 
for a,b in zip(range(len(values)), values):
    ax.text(a, b+0.0005, percent(b), ha='center', va = 'bottom', fontsize=7)

fig = plt.gcf()
plt.show()

name=img_path[0:-4]+'_pred'
fig.savefig(name, dpi=200)