#CppJieba是"结巴"中文分词的C++版本

## 中文编码

现在支持utf8,gbk编码的分词。   

- `master`分支支持`utf8`编码   
- `gbk`分支支持`gbk`编码

## 安装与使用

### 下载和安装

```sh
wget https://github.com/aszxqw/cppjieba/archive/master.zip -O cppjieba-master.zip
unzip cppjieba-master.zip
cd cppjieba-master
mkdir build
cd build
cmake ..
make
sudo make install
```

### 启动服务

```
#启动
/etc/init.d/CppJieba/start.sh
#停止
/etc/init.d/CppJieba/stop.sh
```

#### 验证服务

然后用chrome浏览器打开`http://127.0.0.1:11258/?key=南京市长江大桥`
(用chrome的原因是chrome的默认编码就是utf-8)

或者用命令 `curl "http://127.0.0.1:11258/?key=南京市长江大桥"` (ubuntu中的curl安装命令`sudo apt-get install curl`)

### 卸载
```sh
cd build/
cat install_manifest.txt | sudo xargs rm -rf
```

### 验证

```sh
cd test/
g++ -std=c++0x -o segment.demo segment.cpp -L/usr/lib/CppJieba/ -lcppjieba 
./segment.demo # you will see the demo.
```

运行一下 `./server` 或 `./segment` 都会有对应的帮助文档显示。

同时，如果想知道开发时如何使用`libcppjieba.a` 请看`test/segment.cpp`源代码即可。

如果想知道如何搭建一个`cppjieba`中文分词的http服务请见 `test/server.cpp`源代码即可。

若还有其他问题，欢迎`send mail`或者`open issue`。  :)


## 分词效果

### MPSegment's demo

Output:
```
我来到北京清华大学
我/来到/北京/清华大学

他来到了网易杭研大厦
他/来到/了/网易/杭/研/大厦

小明硕士毕业于中国科学院计算所，后在日本京都大学深造
小/明/硕士/毕业/于/中国科学院/计算所/，/后/在/日本京都大学/深造

我来自北京邮电大学。。。学号091111xx。。。
我/来自/北京邮电大学/。。。/学/号/091111xx/。。。
```

### HMMSegment's demo

Output:
```
我来到北京清华大学
我来/到/北京/清华大学
他来到了网易杭研大厦
他来/到/了/网易/杭/研大厦
小明硕士毕业于中国科学院计算所，后在日本京都大学深造
小明/硕士/毕业于/中国/科学院/计算所/，/后/在/日/本/京/都/大/学/深/造
我来自北京邮电大学。。。学号091111xx。。。
我来/自北京/邮电大学/。。。/学号/091111xx/。。。
```

### MixSegment's demo

Output:
```
我来到北京清华大学
我/来到/北京/清华大学
他来到了网易杭研大厦
他/来到/了/网易/杭研/大厦
杭研
杭研
小明硕士毕业于中国科学院计算所，后在日本京都大学深造
小明/硕士/毕业/于/中国科学院/计算所/，/后/在/日本京都大学/深造
我来自北京邮电大学。。。学号091111xx。。。
我/来自/北京邮电大学/。。。/学号/091111xx/。。。
```

### 效果分析

以上依次是MP,HMM,Mix三种方法的效果。  

可以看出效果最好的是Mix，也就是融合MP和HMM的切词算法。即可以准确切出词典已有的词，又可以切出像"杭研"这样的未登录词。



## 模块详解

本项目主要是如下目录组成：

### src

核心目录，包含主要源代码。

#### Trie树
Trie.cpp/Trie.h 负责载入词典的trie树，主要供Segment模块使用。

#### Segment模块

MPSegment.cpp/MPSegment.h 
(Maximum Probability)最大概率法:负责根据Trie树构建有向无环图和进行动态规划算法，是分词算法的核心。

HMMSegment.cpp/HMMSegment.h
是根据HMM模型来进行分词，主要算法思路是根据(B,E,M,S)四个状态来代表每个字的隐藏状态。
HMM模型由dicts/下面的`hmm_model.utf8`提供。
分词算法即viterbi算法。

#### TransCode模块

TransCode.cpp/TransCode.h 负责转换编码类型，将utf8和gbk转换成`uint16_t`类型，也负责逆转换。

### src/Husky

提供服务的框架代码，

详见：  https://github.com/aszxqw/husky

### src/Limonp 

主要是一些工具函数，例如字符串操作等。    
直接include就可以使用。

详见：  https://github.com/aszxqw/limonp

## 分词速度

### MixSegment

分词速度大概是 62M / 54sec = 1.15M/sec
测试环境: `Intel(R) Xeon(R) CPU  E5506  @ 2.13GHz`


## 联系客服

如果有运行问题或者任何疑问，欢迎联系 : wuyanyi09@gmail.com

## 鸣谢

"结巴中文"分词作者: SunJunyi  
https://github.com/fxsjy/jieba

顾名思义，之所以叫CppJieba，是参照SunJunyi大神的Jieba分词Python程序写成的，所以饮水思源，再次感谢SunJunyi。


