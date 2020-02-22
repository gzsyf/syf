import tensorflow.compat.v1 as tf # 兼容tensorflow 1
import numpy as np

# ------------ 设置 隐藏层 add_layer ----------------
# n_layer--设置为第几层 in_size--输入大小 out_size--输出大小 activation_function--激励函数
def add_layer(inputs,in_size,out_size,n_layer,activation_function=None):                # None的话，默认就是线性函数

    layer_name = 'layer%s' % n_layer                                                    # 当前的层数
    with tf.name_scope('layer'):                                                        # tensorboard设置大框架layer层
        with tf.name_scope('weights'):                                                  # tensorboard设置小框架weights
            Weights = tf.Variable(tf.random_normal([in_size,out_size]),name='W')        # 初始化权重 生成In_size行和out_size列的矩阵
                                                                                        # insize个输入变成 outsize个神经元
            tf.summary.histogram(layer_name + '/weights', Weights)                      # tensorboard中 看当前层 w 的变化
        with tf.name_scope('biases'):                                                   # tensorboard设置小框架biases
            biases = tf.Variable(tf.zeros([1,out_size])+0.1,name='b')                   # 初始化偏置 配合 w
            tf.summary.histogram(layer_name + '/biases', biases)                        # tensorboard中 看当前层 b 的变化
        with tf.name_scope('Wx_plus_b'):                                                # tensorboard设置小框架 Wx_plus_b
            Wx_plus_b = tf.matmul(inputs,Weights)+biases                                # 线性变化 还没有被激活的值存储在这个变量中
        if activation_function is None:                                                 # 如果没有激励函数 就不激励
            outputs = Wx_plus_b
        else:                                                                           # 如果有激励函数 就激励
            outputs=activation_function(Wx_plus_b)
        tf.summary.histogram(layer_name + '/outputs', outputs)                          # tensorboard中 看当前层 output 的变化
        return outputs                                                                  # outputs add_layer的输出值

# 初始化x -- 输入的数据
x_data = np.linspace(-1, 1, 300)[:, np.newaxis]
# 设置noise -- 扰动
noise = np.random.normal(0, 0.05, x_data.shape)
# 设置实际值 -- 输入的数据+扰动
y_data = np.square(x_data) - 0.5 + noise

# ------------ 设置 输入层 inputs ----------------
with tf.name_scope('inputs'):                                                           #  tensorboard设置大框架input层
    xs=tf.placeholder(tf.float32,[None,1],name='x_input')                               # 输入数据x 是32位浮点型 是个列向量
    ys=tf.placeholder(tf.float32,[None,1],name='y_input')                               # 标签y 是32位浮点型 是个列向量

# ------------ 第一层 隐藏层 l1 ----------------
# 输入x
l1 = add_layer(xs,1,10,n_layer=1,activation_function=tf.nn.relu)                                  # L1仅是单隐藏层，全连接网络。

# ------------ 输出层 prediction----------------
prediction = add_layer(l1,10,1,n_layer=2,activation_function=None)

# ----------- 损失函数(回归误差函数)----------------
with tf.name_scope('loss'):
    loss = tf.reduce_mean(tf.reduce_sum(tf.square(ys-prediction),reduction_indices=[1]))    # reduction_indices=[1]:按行求和
                                                                                            # reduction_indices=[0] 按列求和
                                                                                            # sum是将所有例子求和，再求平均（mean）
    tf.summary.scalar('loss', loss)                                                         # loss这里要用scalar。如果是在减小，说明学到东西了

# ----------- 训练 ----------------
with tf.name_scope('train'):
    train_step = tf.train.GradientDescentOptimizer(0.1).minimize(loss)                      # 以0.1的学习效率来训练学习，来减小loss

init = tf.global_variables_initializer()                # 初始化变量
sess=tf.Session()                                       # 建立会话

merged = tf.summary.merge_all()                         # tensorboard 整合
writer = tf.summary.FileWriter('logs',sess.graph)       # 保存tensorboard 把图片load到log的文件夹里，在浏览器里浏览

sess.run(init)                                          # 开始运行初始化

# --------- 训练迭代500次 -------------
for i in range(500):
     sess.run(train_step,feed_dict={xs:x_data,ys:y_data})           # 喂数据
     if i%50==0:                                                    # 迭代10次更新数据
         result = sess.run(merged,feed_dict={xs:x_data,ys:y_data})  # 把新的数据放入tensorboard
         writer.add_summary(result,i)
