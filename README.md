#CppJieba是"结巴"中文分词的C++版本

功能性的代码全写成hpp文件，此处的hpp文件是将cpp和h两个文件全都写在hpp文件里面（当然需要遵守相关约束）

之所以全写成hpp文件，是因为这样在别的项目需要使用到中文分词功能的时候直接`#include"xx.hpp" `进来就可以使用，无需麻烦的链接。

实践证明写成hpp使用起来真的很爽，在后面提到的在iOS应用中的使用，和包装成`Node.js`的扩展[NodeJieba]都特别顺利。
`

## 中文编码

现在支持utf8,gbk编码的分词。   

## 安装与使用

### 依赖

* g++ (version >= 4.6 recommended);
* cmake (version >= 2.8 recommended);

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
make test 
```

### 启动服务

因为服务的后台运行需要`start-stop-daemon`，在ubuntu下是自带的。但是在CentOS下就需要自己安装了。

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

```

### 效果分析

以上依次是MP,HMM,Mix三种方法的效果。  

可以看出效果最好的是Mix，也就是融合MP和HMM的切词算法。即可以准确切出词典已有的词，又可以切出像"杭研"这样的未登录词。

Full方法切出所有字典里的词语。

Query方法先使用Mix方法切词，对于切出来的较长的词再使用Full方法。

### 关键词抽取

```
make && ./test/keyword.demo
```

you will see:

```
我是蓝翔技工拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上总经理，出任CEO，迎娶白富美，走上人生巅峰。
 ->
["CEO:11.7392", "蓝翔:11.7392", "白富美:11.7392", "升职:10.8562", "加薪:10.6426"]
```

关键词抽取的demo代码请见`test/keyword_demo.cpp`


## 代码详解

详见：http://aszxqw.com/jekyll/update/2014/02/10/cppjieba-dai-ma-xiang-jie.html

## 相关应用

### 关于CppJieba的跨语言包装使用

收到邮件询问跨语言包装(ios应用开发)使用的问题，这方面我没有相关的经验，建议参考如下python使用cppjieba的项目：

[jannson] 开发的供 python模块调用的项目 [cppjiebapy] ,  和相关讨论 [cppjiebapy_discussion] .

### NodeJieba

如果有需要在`node.js`中使用分词，不妨试一下[NodeJieba]。

### simhash

如果有需要在处理中文文档的的相似度计算，不妨试一下[simhash]。

## 演示

http://cppjieba-webdemo.herokuapp.com/

## 客服

如果有运行问题或者任何疑问，欢迎联系 : wuyanyi09@gmail.com

## 鸣谢

"结巴"中文分词作者: SunJunyi  
https://github.com/fxsjy/jieba

顾名思义，之所以叫CppJieba，是参照Jieba分词Python程序写成的，所以饮水思源，再次感谢SunJunyi。

[CppJieba]:https://github.com/aszxqw/cppjieba
[jannson]:https://github.com/jannson
[cppjiebapy]:https://github.com/jannson/cppjiebapy
[cppjiebapy_discussion]:https://github.com/aszxqw/cppjieba/issues/1
[NodeJieba]:https://github.com/aszxqw/nodejieba
[simhash]:https://github.com/aszxqw/simhash
