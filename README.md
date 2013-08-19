#CppJieba
"结巴"中文分词的cpp版本
"结巴"中文分词详见:
https://github.com/fxsjy/jieba
n的jieba分词源码写的。

#Detail
========
1.现在支持utf8,gbk编码的分词。默认编码是utf8。
2.分词算法上还没增加HMM模型这部分。
3.关键词抽取是暂时是针对类似title之类的超短语句使用的，基本上没有普适性。


#Demo

## Segment's demo
```
cd ./demo;
make;
./segment_demo testlines.gbk
```
run `./segment_demo` to get help.

#Contact
wuyanyi09@gmail.com
