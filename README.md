# Job_Trick_1-在python中使用numpy接口调用c++
工作经验分享系列1

做为转行狗，深知转行之不易，觉得有些工作经验和实际编码当中的一些trick需要和大家一起分享。分享的东西绝对是干货和有用的，打算后续每周做一期。

今天分享的主题是如何在python里面以numpy的接口调用c++。为什么有这个需求呢？比如你用tensorflow训练模型，需要对数据进行预处理，涉及到多重循环，直接用python写会比较慢，为了训练速度，必须写成c++。为了能使用tf.py_func将数据预处理流程写成tensorflow的op，必须使用numpy接口。

最简单的方式是使用cython。我写了一个简单的求和demo。

在示例代码中，直接运行：
```shell
sh build_ext.sh
```
即可生成python库，主要是一个.so文件，然后运行：
python test.py
即可查看调用的效果。
