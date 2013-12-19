#CppJieba是"结巴"中文分词的C++版本

功能性的代码全写成hpp文件，此处的hpp文件是将cpp和h两个文件全都写在hpp文件里面（当然需要遵守相关约束）

之所以全写成hpp文件，是因为这样在别的项目需要使用到中文分词功能的时候直接`#include"xx.hpp" `进来就可以使用，无需麻烦的链接。

## 中文编码

现在支持utf8,gbk编码的分词。   

## 安装与使用

### 下载和安装

```sh
wget https://github.com/aszxqw/cppjieba/archive/master.zip -O cppjieba-master.zip
unzip cppjieba-master.zip
cd cppjieba-master
mkdir build
cd build
cmake ..
# 默认是utf8编码，如果要使用gbk编码则使用下句cmake命令
# cmake .. -DENC=GBK
make
sudo make install
```

#### 测试

```sh
cd build && ./test/segment.demo
```

### 启动服务

```
#Usage: /etc/init.d/cjserver {start|stop|restart|force-reload}
#启动
sudo /etc/init.d/cjserver start
#停止
sudo /etc/init.d/cjserver stop
```

#### 测试服务

然后用chrome浏览器打开`http://127.0.0.1:11200/?key=南京市长江大桥`
(用chrome的原因是chrome的默认编码就是utf-8)

或者用命令 `curl "http://127.0.0.1:11200/?key=南京市长江大桥"` (ubuntu中的curl安装命令`sudo apt-get install curl`)


### 卸载
```sh
cd build/
cat install_manifest.txt | sudo xargs rm -rf
```

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

小明硕士毕业于中国科学院计算所，后在日本京都大学深造
小明/硕士/毕业/于/中国科学院/计算所/，/后/在/日本京都大学/深造

我来自北京邮电大学。。。学号091111xx。。。
我/来自/北京邮电大学/。。。/学号/091111xx/。。。
```

### FullSegment's demo

Output:
```
我来到北京清华大学
我/来到/北京/清华/清华大学/华大/大学

他来到了网易杭研大厦
他/来到/了/网易/杭/研/大厦

小明硕士毕业于中国科学院计算所，后在日本京都大学深造
小/明/硕士/毕业/于/中国/中国科学院/科学/科学院/学院/计算/计算所/，/后/在/日本/日本京都大学/京都/京都大学/大学/深造

我来自北京邮电大学。。。 学号 091111xx。。。
我/来自/北京/北京邮电/北京邮电大学/邮电/邮电大学/电大/大学/。/。/。/ /学号/ 091111xx/。/。/。
```

### QuerySegment's demo

Output:
```
我来到北京清华大学
我/来到/北京/清华/清华大学/华大/大学

他来到了网易杭研大厦
他/来到/了/网易/杭研/大厦

小明硕士毕业于中国科学院计算所，后在日本京都大学深造
小明/硕士/毕业/于/中国/中国科学院/科学/科学院/学院/计算所/，/后/在/中国/中国科学院/科学/科学院/学院/日本/日本京都大学/京都/京都大学/大学/深造

我来自北京邮电大学。。。 学号 091111xx。。。
我/来自/北京/北京邮电/北京邮电大学/邮电/邮电大学/电大/大学/。/。/。/ /学号/ 091111xx/。/。/。
```

### 效果分析

以上依次是MP,HMM,Mix三种方法的效果。  

可以看出效果最好的是Mix，也就是融合MP和HMM的切词算法。即可以准确切出词典已有的词，又可以切出像"杭研"这样的未登录词。

Full方法切出所有字典里的词语。

Query方法先使用Mix方法切词，对于切出来的较长的词再使用Full方法。

## 模块详解

本项目主要是如下目录组成：

### src

核心目录，包含主要源代码。

#### TrieManager模块
TrieManager.hpp 提供一个单例TrieManager，负责管理trie树。
通过该单例获取trie树时，会先判断是否已经由该字典文件生成了一颗trie树，如果已有则返回已有的trie树，否则重新创建一颗trie树返回。

#### Trie树
Trie.hpp 负责载入词典的trie树，主要供Segment模块使用。

#### Segment模块

MPSegment.hpp
(Maximum Probability)最大概率法:负责根据Trie树构建有向无环图和进行动态规划算法，是分词算法的核心。

HMMSegment.hpp
是根据HMM模型来进行分词，主要算法思路是根据(B,E,M,S)四个状态来代表每个字的隐藏状态。
HMM模型由dicts/下面的`hmm_model.utf8`提供。
分词算法即viterbi算法。

FullSegment.hpp
枚举句子中所有可能成词的情况，找出字典里存在的即可。

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

分词速度大概是 = 2M/sec
测试环境: `Intel(R) Xeon(R) CPU  E5506  @ 2.13GHz` 电脑下开的ubuntu虚拟机


## 联系客服

如果有运行问题或者任何疑问，欢迎联系 : wuyanyi09@gmail.com

## 鸣谢

"结巴中文"分词作者: SunJunyi  
https://github.com/fxsjy/jieba

顾名思义，之所以叫CppJieba，是参照SunJunyi大神的Jieba分词Python程序写成的，所以饮水思源，再次感谢SunJunyi。

