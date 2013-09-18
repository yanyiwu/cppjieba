# CppJieba是"结巴"中文分词的C++库

## 中文编码

`master`分支为utf8编码，此分支支持gbk编码。

## GBK版本的Demo

### MPSegment's demo

```
cd ./demo;
make;
./segment_demo testlines.gbk
```

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

```
cd ./demo;
make;
./segment_demo testlines.gbk --modelpath ../dicts/hmm_model.gbk --algorithm cutHMM
```

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
```
cd ./demo;
make;
./segment_demo testlines.gbk --algorithm cutMix
```

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
```

### 效果分析

以上依次是MP,HMM,Mix三种方法的效果。  
可以看出效果最好的是Mix，也就是融合MP和HMM的切词算法。即可以准确切出词典已有的词，又可以切出像"杭研"这样的未登录词。

## 其它详见`master`分支的README.md

## Contact

如果有运行问题或者任何疑问，欢迎联系 : wuyanyi09@gmail.com

## Thanks

"结巴中文"分词作者: SunJunyi  
https://github.com/fxsjy/jieba

顾名思义，之所以叫CppJieba，就是边参照边膜拜SunJunyi大神的Jieba分词Python程序写成的，所以饮水思源，再次感谢SunJunyi。


