# CppJieba [简体中文](README.md)

[![Build Status](https://travis-ci.org/yanyiwu/cppjieba.png?branch=master)](https://travis-ci.org/yanyiwu/cppjieba) 
[![Author](https://img.shields.io/badge/author-@yanyiwu-blue.svg?style=flat)](http://yanyiwu.com/) 
[![Platform](https://img.shields.io/badge/platform-Linux,%20OS%20X,%20Windows-green.svg?style=flat)](https://github.com/yanyiwu/cppjieba)
[![Performance](https://img.shields.io/badge/performance-excellent-brightgreen.svg?style=flat)](http://yanyiwu.com/work/2015/06/14/jieba-series-performance-test.html) 
[![License](https://img.shields.io/badge/license-MIT-yellow.svg?style=flat)](http://yanyiwu.mit-license.org)

## Introduction

The Jieba Chinese Word Segmentation Implemented By C++ .

## Usage 

### Dependencies

+ `g++ (version >= 4.1 is recommended) or clang++`;
+ `cmake (version >= 2.6 is recommended)`;

### Download & Compile

```sh
git clone --depth=10 --branch=master git://github.com/yanyiwu/cppjieba.git
cd cppjieba
mkdir build
cd build
cmake ..
make
```

### Unit Testing

```
make test
```

## Demo

```
./demo
```

Output:

```
[demo] Cut With HMM
我/是/拖拉机/学院/手扶拖拉机/专业/的/。/不用/多久/，/我/就/会/升职/加薪/，/当上/CEO/，/走上/人生/巅峰/。
[demo] Cut Without HMM
我/是/拖拉机/学院/手扶拖拉机/专业/的/。/不用/多久/，/我/就/会/升职/加薪/，/当/上/C/E/O/，/走上/人生/巅峰/。
[demo] CutAll
我/是/拖拉/拖拉机/学院/手扶/手扶拖拉机/拖拉/拖拉机/专业/的/。/不用/多久/，/我/就/会升/升职/加薪/，/当上/C/E/O/，/走上/人生/巅峰/。[demo] CutForSearch
我/是/拖拉机/学院/手扶/手扶拖拉机/拖拉/拖拉机/专业/的/。/不用/多久/，/我/就/会/升职/加薪/，/当上/CEO/，/走上/人生/巅峰/。
[demo] Insert User Word
男默/女泪
男默女泪
[demo] Locate Words
南京市, 0, 3
长江大桥, 3, 7
[demo] TAGGING
我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。
["我:r", "是:v", "拖拉机:n", "学院:n", "手扶拖拉机:n", "专业:n", "的:uj", "。:x", "不用:v", "多久:m", "，:x", "我:r", "就:d", "会:v", "升职:v", "加薪:nr", "，:x", "当上:t", "CEO:eng", "，:x", "走上:v", "人生:n", "巅峰:n", "。:x"]
```

Please see details in `test/demo.cpp`.

## Cases

+ [GoJieba] 
+ [NodeJieba]
+ [simhash]
+ [exjieba]
+ [jiebaR]
+ [cjieba]
+ [jieba_rb]
+ [iosjieba]
+ [SqlJieba]
+ [pg_jieba]
+ [ngx_http_cppjieba_module]
+ [gitbook-plugin-search-pro]

## Contact

+ Email: `i@yanyiwu.com`
+ QQ: 64162451
+ WeChat: ![image](http://7viirv.com1.z0.glb.clouddn.com/5a7d1b5c0d_yanyiwu_personal_qrcodes.jpg)

[GoJieba]:https://github.com/yanyiwu/gojieba
[CppJieba]:https://github.com/yanyiwu/cppjieba
[jannson]:https://github.com/jannson
[cppjiebapy]:https://github.com/jannson/cppjiebapy
[cppjiebapy_discussion]:https://github.com/yanyiwu/cppjieba/issues/1
[NodeJieba]:https://github.com/yanyiwu/nodejieba
[jiebaR]:https://github.com/qinwf/jiebaR
[simhash]:https://github.com/yanyiwu/simhash
[exjieba]:https://github.com/falood/exjieba
[cjieba]:http://github.com/yanyiwu/cjieba
[jieba_rb]:https://github.com/altkatz/jieba_rb
[iosjieba]:https://github.com/yanyiwu/iosjieba
[SqlJieba]:https://github.com/yanyiwu/sqljieba
[pg_jieba]:https://github.com/jaiminpan/pg_jieba
[gitbook-plugin-search-pro]:https://plugins.gitbook.com/plugin/search-pro
