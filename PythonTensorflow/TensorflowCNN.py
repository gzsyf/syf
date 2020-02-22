import tensorflow.compat.v1 as tf # 兼容tensorflow 1
import os
os.environ['CUDA_VISIBLE_DEVICES'] = '-1'
from tensorflow.examples.tutorials.mnist import input_data

mnist = input_data.read_data_sets('MNIST_data', one_hot=True)



# ---------- 添加一个隐藏层 ---------
#in_size--输入大小 out_size--输出大小 activation_function--激励函数
def add_layer(inputs, in_size, out_size, activation_function=None, keep_prob=1):

    Weights = tf.Variable(tf.random_normal([in_size, out_size]))        # 初始化权重 随机初始化
    biases = tf.Variable(tf.zeros([1, out_size]) + 0.1)                 # 初始化权重 为0初始化
    Wx_plus_b = tf.matmul(inputs, Weights) + biases                     # 线性变换
    Wx_plus_b = tf.nn.dropout(Wx_plus_b, keep_prob)                     # 这里使用Dropout处理计算结果，默认keep_prob为1，即保持所有神经元不被临时删掉
    if activation_function is None:                                     # 如果没有激励函数 就不激励
        outputs = Wx_plus_b
    else:                                                               # 如果有激励函数 就激励
        outputs = activation_function(Wx_plus_b)
    return outputs


# ----------------- 测试准确度accuracy函数 -----------------
# v_xs：测试集  v_ys：标签
def compute_accuracy(v_xs, v_ys):
    global prediction                                                           # 引入全局变量prediction层
    y_pre = sess.run(prediction, feed_dict={xs: v_xs})                          # 用v_xs输入数据跑一次prediction层，得到输出
    correct_prediction = tf.equal(tf.argmax(y_pre, 1), tf.argmax(v_ys, 1))      # 对比输出和数据集label，相同的为1，不同的为0
                                                                                # argmax(y,1)按行获取最大值的index
    accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))          # 计算比对结果，可得到准确率百分比
    result = sess.run(accuracy, feed_dict={xs: v_xs, ys: v_ys})                 # 获取result，并返回
    return result


# ------------ 初始化权重 ------------------
def weight_variable(shape):
    initial = tf.random.truncated_normal(shape, stddev=0.1)      # 正态分布初始化
    return tf.Variable(initial)


# ------------ 初始化偏置 ---------------
def bias_variable(shape):
    initial = tf.constant(0.1, shape=shape)                     # 0.1初始化
    return tf.Variable(initial)

# ------------ 二维卷积(channel=1黑白) ------------
def conv2d(x, W):
    return tf.nn.conv2d(x, W, strides=[1, 1, 1, 1], padding='SAME')     # x为输入，W为参数矩阵，扫描跨度strides=[1,y_step,x_step,1]
                                                                        # 扫描方式 SAME  有两个取值'SAME'和'VALID',对应一个填充，一个不填充

# ------------ 二维池化(channel=1黑白) ------------
def max_pool_2x2(x):
    return tf.nn.max_pool2d(x, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')    # ksize

# ------------ 初始化输入输出结构 ------------
# 初始化x -- 输入的数据的大小
xs = tf.placeholder(tf.float32, [None, 784])      # None是数据的个数(不规定)
                                                            # 手写数字的图片大小为28*28
# 设置实际值 -- 输入的数据
ys = tf.placeholder(tf.float32, [None, 10])       # 输出为1*10 哪一个元素置1 就是哪个数字
x_data = tf.reshape(xs, [-1, 28, 28, 1])                    # -1表示样本数m(根据每轮训练的输入大小batch_size=100),28*28表示图片大小，1表示channel



#--------------  定义网络结构 --------------
#-------------- 卷积1层 -----------------
# output = 28*28*16
W_conv1 = weight_variable([5, 5, 1, 16])                    # 定义1层权重(卷积核)  5*5*1*16的矩阵 1表示核的channel，16表示核的个数
b_conv1 = bias_variable([16])                               # 定义conv1的bias矩阵
h_conv1 = tf.nn.relu(conv2d(x_data, W_conv1) + b_conv1)     # 先线性化(卷积) 再激活(非线性化)
#-------------- 池化1层 -----------------
# output = 14*14*16
h_pool1 = max_pool_2x2(h_conv1)
#-------------- 卷积2层 -----------------
# output = 14*14*32
W_conv2 = weight_variable([5, 5, 16, 32])
b_conv2 = bias_variable([32])
h_conv2 = tf.nn.relu(conv2d(h_pool1, W_conv2) + b_conv2)
#-------------- 池化2层 -----------------
# output = 7*7*32
h_pool2 = max_pool_2x2(h_conv2)
h_pool2_flat = tf.reshape(h_pool2, [-1, 7 * 7 * 32])        # 全连接层要1维  把7*7*32的矩阵压扁成[1568]的向量
                                                            # 检查一下矩阵维度，确认为(100,1568),其中100是batch_size
                                                            # h_shape = tf.shape(h_pool2_flat)
#-------------- 全连接1层 -----------------
W_fc1 = weight_variable([7 * 7 * 32, 200])                  # 定义权重矩阵，维度为1568*200
b_fc1 = bias_variable([200])                                # 定义偏置 200个bias
h_fc1 = tf.nn.relu(tf.matmul(h_pool2_flat, W_fc1) + b_fc1)  # 激活函数
# h_fc1_drop = tf.nn.dropout(h_fc1)                         # 是否启用dropout

#-------------- 全连接2层 -----------------
W_fc2 = weight_variable([200, 100])
b_fc2 = bias_variable([100])
h_fc2 = tf.nn.relu(tf.matmul(h_fc1, W_fc2) + b_fc2)

# --------- 输出层 -------------
w_fc3 = weight_variable([100, 10])
b_fc3 = bias_variable([10])
prediction = tf.nn.softmax(tf.matmul(h_fc2, w_fc3) + b_fc3)

# ----------- 损失函数(交叉熵损失) ----------------
cross_entropy = tf.reduce_mean(-tf.reduce_sum(ys * tf.math.log(prediction),reduction_indices=[1]))
tf.summary.scalar('Loss', cross_entropy)                            # loss这里要用scalar。如果是在减小，说明学到东西
train_step = tf.train.AdamOptimizer(1e-4).minimize(cross_entropy)   # 使用Adam优化算法，学习率为0.0001来最小化cross_entropy

# ----------- 初始化 ---------------
init = tf.global_variables_initializer()                            # 初始化变量
sess = tf.Session()                                                 # 建立会话
train_writer = tf.summary.FileWriter('logs2/train', sess.graph)     # 画出train数据集的loss曲线
test_writer = tf.summary.FileWriter('logs2/test', sess.graph)       # 画出test数据集的loss曲线
merged = tf.summary.merge_all()                                     # tensorboard 整合
sess.run(init)                                                      # 初始化

# --------- 训练迭代1000次 -------------
for i in range(1000):

    batch_x, batch_y = mnist.train.next_batch(100)                  # 每次训练的样本集 batch_size=100
    sess.run(train_step, feed_dict={xs: batch_x, ys: batch_y})      # 喂数据

    if i % 100 == 0:                                                # 迭代10次更新数据
        train_res = sess.run(merged, feed_dict={xs: batch_x, ys: batch_y})
        test_res = sess.run(merged, feed_dict={xs: mnist.test.images, ys: mnist.test.labels})
        train_writer.add_summary(train_res, i)                      # 把新train的数据放入tensorboard
        test_writer.add_summary(test_res, i)                        # 把新test的数据放入tensorboard

        # 打印准确率
        print('Acc on loop ', i, ':', compute_accuracy(mnist.test.images, mnist.test.labels))