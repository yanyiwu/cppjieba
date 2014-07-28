[![Build Status](https://travis-ci.org/aszxqw/cppjieba.png?branch=master)](https://travis-ci.org/aszxqw/cppjieba)
- - -

# CppJieba

## Introduction

CppJieba是"结巴"中文分词的C++版本

代码细节详解请见 [代码详解]

## Feature

+ 源代码都写进头文件`src/*.hpp`里，`include`即可使用。
+ 支持`utf-8, gbk`编码，但是推荐使用`utf-8`编码。
+ 内置分词服务`server/server.cpp`，在linux环境下可安装使用。
+ 项目自带较为完善的单元测试，核心功能中文分词(utf8)的稳定性接受过线上环境检验。
+ 支持载自定义用户词典。

## Usage 

### Dependency

* `g++ (version >= 4.1 recommended) or clang++`;
* `cmake (version >= 2.6 recommended)`;

### Download & Demo

```sh
git clone --depth=10 --branch=master git://github.com/aszxqw/cppjieba.git
cd cppjieba
mkdir build
cd build
cmake ..
# 默认是utf8编码，如果要使用gbk编码则使用下句cmake命令
# cmake .. -DENC=GBK
# 需要注意的是，单元测试都是针对utf8的测试，如果是使用gbk选项的话，此测试不通过。
make
# 如果你需要服务支持用户自定义词典的话，
# 可以在 ./conf/server.conf 里面的这行 "#user_dict_path=/usr/share/CppJieba/dict/user.dict.utf8" 前面的#号去掉。
sudo make install
```

### testing

```sh
make && ./test/test.run
```

### server start & stop

```
/etc/init.d/cjserver.start >> /dev/null 2>&1
/etc/init.d/cjserver.stop
```

### testing server

然后用chrome浏览器打开 (chrome的默认编码是utf-8):

```
http://127.0.0.1:11200/?key=南京市长江大桥
```

```
http://127.0.0.1:11200/?key=南京市长江大桥&format=simple
```

或者用命令  (ubuntu中的curl安装命令`sudo apt-get install curl`) 

```
curl -G  "http://127.0.0.1:11200/" --data-urlencode "key=南京市长江大桥"
```

你可以看到返回的结果如下：(返回结果是json格式)

```
["南京市", "长江大桥"]
```

如果你使用如下调用方式：

```
curl -G  "http://127.0.0.1:11200/" --data-urlencode "key=南京市长江大桥" -d "format=simple"
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

```
make && ./segment.demo
```

see details in `test/segment_demo.cpp`.

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

自定义词典示例请看`test/testdata/userdict.utf8`。

载入自定义词典示例请看`test/segment.cpp`，产生的可执行文件示例请见 `build/segment.demo`

没有使用自定义用户词典时的结果:

```
令狐冲/是/云/计算/行业/的/专家
```

使用自定义用户词典时的结果:

```
令狐冲/是/云计算/行业/的/专家
```

### 关键词抽取

```
make && ./keyword.demo
```

see details in `test/keyword_demo.cpp`.

you will see:

```
我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。
["CEO:11.7392", "升职:10.8562", "加薪:10.6426", "手扶拖拉机:10.0089", "巅峰:9.49396"]
```

### 词性标注

```
make && ./tagging.demo
```

see details in `test/tagging_demo.cpp`.

```
["我:r", "是:v", "蓝翔:x", "技工:n", "拖拉机:n", "学院:n", "手扶拖拉机:n", "专业:n", "的:uj", "。:x", "不用:v", "多久:m", "，:x", "我:r", "就:d", "会:v", "升职:v", "加薪:nr", "，:x", "当:t", "上:f", "总经理:n", "，:x", "出任:v", "CEO:x", "，:x", "迎娶:v", "白富美:x", "，:x", "走上:v", "人生:n", "巅峰:n", "。:x"]
```

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

### libcppjieba

[libcppjieba] 是最简单易懂的CppJieba头文件库使用示例。

### `keyword_server`

[KeywordServer] 50行搭建一个**中文关键词抽取服务**。

## Online Demo

http://cppjieba-webdemo.herokuapp.com/
(建议使用chrome打开)

## Contact

I will appreciate that if you issue any question or send mails to me. (**wuyanyi09@foxmail.com**)

## Thanks

"结巴"中文分词作者: SunJunyi  
https://github.com/fxsjy/jieba

## Author

- aszxqw https://github.com/aszxqw wuyanyi09@foxmail.com
- aholic https://github.com/aholic ruochen.xu@gmail.com

[CppJieba]:https://github.com/aszxqw/cppjieba
[jannson]:https://github.com/jannson
[cppjiebapy]:https://github.com/jannson/cppjiebapy
[cppjiebapy_discussion]:https://github.com/aszxqw/cppjieba/issues/1
[NodeJieba]:https://github.com/aszxqw/nodejieba
[simhash]:https://github.com/aszxqw/simhash
[代码详解]:http://www.aszxqw.com/work/2014/02/10/cppjieba-dai-ma-xiang-jie.html
[libcppjieba]:https://github.com/aszxqw/libcppjieba
[issue25]:https://github.com/aszxqw/cppjieba/issues/25
[exjieba]:https://github.com/falood/exjieba
[KeywordServer]:https://github.com/aszxqw/keyword_server
