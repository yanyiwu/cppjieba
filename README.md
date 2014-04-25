# CppJieba

## Introduction

CppJieba是"结巴"中文分词的C++版本

代码细节详解请见 [代码详解]

## Feature

+ 源代码都写进头文件hpp里，`include`即可使用。
+ 支持`utf-8, gbk`编码，但是推荐使用`utf-8`编码。
+ 内置分词服务，在linux环境下可安装使用。mac因为没有自带`epoll`，使用示例请看[libcppjieba]。
+ [libcppjieba] 最简单易懂的CppJieba头文件库使用示例。
+ 项目自带较为完善的单元测试，核心功能中文分词的稳定性接受过线上环境检验。
+ 支持载自定义用户词典。

## Usage & Example

### Dependency

* g++ (version >= 4.1 recommended);
* cmake (version >= 2.6 recommended);

### Download & Demo

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

#### testing

```sh
make test 
```

### server start & stop

```
#Usage: /etc/init.d/cjserver {start|stop|restart|force-reload}
/etc/init.d/cjserver.start
/etc/init.d/cjserver.stop
```

#### testing server

然后用chrome浏览器打开`http://127.0.0.1:11200/?key=南京市长江大桥`
(用chrome的原因是chrome的默认编码就是utf-8)

或者用命令 `curl "http://127.0.0.1:11200/?key=南京市长江大桥"` (ubuntu中的curl安装命令`sudo apt-get install curl`) 

你可以看到返回的结果如下：(返回结果是json格式)

```
["南京市", "长江大桥"]
```

如果你使用如下调用方式：

```
curl "http://127.0.0.1:11200/?key=南京市长江大桥&format=simple"
```

则返回结果如下：(返回结果按空格隔开)

```
南京市 长江大桥
```

同时，也支持HTTP POST模式，使用如下调用:

```
curl -d "南京市长江大桥" "http://127.0.0.1:11200/"
```

返回结果如下：

```
["南京市", "长江大桥"]
```

### uninstall
```sh
cd build/
cat install_manifest.txt | sudo xargs rm -rf
```

## Demo

最简单易懂的使用示例请看[libcppjieba]。它是根据[issue25]的建议专门弄的头文件库。

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


以上依次是MP,HMM,Mix三种方法的效果。  

可以看出效果最好的是Mix，也就是融合MP和HMM的切词算法。即可以准确切出词典已有的词，又可以切出像"杭研"这样的未登录词。

Full方法切出所有字典里的词语。

Query方法先使用Mix方法切词，对于切出来的较长的词再使用Full方法。

### 自定义用户词典

```
```


### 关键词抽取

```
make && ./test/keyword.demo
```

you will see:

```
我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。
["CEO:11.7392", "升职:10.8562", "加薪:10.6426", "手扶拖拉机:10.0089", "巅峰:9.49396"]
```

关键词抽取的demo代码请见`test/keyword_demo.cpp`

### 词性标注

```
make && ./test/tagging_demo
```

```
["我:r", "是:v", "蓝翔:x", "技工:n", "拖拉机:n", "学院:n", "手扶拖拉机:n", "专业:n", "的:uj", "。:x", "不用:v", "多久:m", "，:x", "我:r", "就:d", "会:v", "升职:v", "加薪:nr", "，:x", "当:t", "上:f", "总经理:n", "，:x", "出任:v", "CEO:x", "，:x", "迎娶:v", "白富美:x", "，:x", "走上:v", "人生:n", "巅峰:n", "。:x"]
```

__词性标注是一个未完成的部分，现在只是一个简单版本。__



## Application

### 关于CppJieba的跨语言包装使用

收到邮件询问跨语言包装(ios应用开发)使用的问题，这方面我没有相关的经验，建议参考如下python使用cppjieba的项目：

[jannson] 开发的供 python模块调用的项目 [cppjiebapy] ,  和相关讨论 [cppjiebapy_discussion] .

### NodeJieba

如果有需要在`node.js`中使用分词，不妨试一下[NodeJieba]。

### simhash

如果有需要在处理中文文档的的相似度计算，不妨试一下[simhash]。

### exjieba

如果有需要在`erlang`中使用分词的话，不妨试一下[exjieba]。

## Online Demo

http://cppjieba-webdemo.herokuapp.com/
(建议使用chrome打开)

## Contact

I will appreciate that if you issue any question or send mails to me(wuyanyi09@foxmail.com).

## Thanks

"结巴"中文分词作者: SunJunyi  
https://github.com/fxsjy/jieba

[CppJieba]:https://github.com/aszxqw/cppjieba
[jannson]:https://github.com/jannson
[cppjiebapy]:https://github.com/jannson/cppjiebapy
[cppjiebapy_discussion]:https://github.com/aszxqw/cppjieba/issues/1
[NodeJieba]:https://github.com/aszxqw/nodejieba
[simhash]:https://github.com/aszxqw/simhash
[代码详解]:http://aszxqw.github.io/jekyll/update/2014/02/10/cppjieba-dai-ma-xiang-jie.html
[libcppjieba]:https://github.com/aszxqw/libcppjieba
[issue25]:https://github.com/aszxqw/cppjieba/issues/25
[exjieba]:https://github.com/falood/exjieba
