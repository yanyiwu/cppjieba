#CppJieba
"结巴"中文分词的cpp版本
"结巴"中文分词详见:
https://github.com/fxsjy/jieba
n的jieba分词源码写的。

#Detail
========
1.现在只支持gbk编码的分词。
2.分词算法上还没增加HMM模型这部分。
3.关键词抽取是暂时是针对类似title之类的超短语句使用的，基本上没有普适性。


#Demo

## Segment's demo
```
cd ./demo;
make;
./segment_demo testlines.gbk
```

#Contact
wuyanyi09@gmail.com
