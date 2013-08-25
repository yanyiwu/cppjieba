#CppJieba是"结巴"中文分词的c++库

##Detail
* 现在支持utf8,gbk编码的分词。默认编码是utf8。  

##Algorithm

###Trie树
Trie.cpp/Trie.h 负责载入词典的trie树，主要供Segment模块使用。
###Segment模块
Segment.cpp/Segment.h 
负责根据Trie树构建有向无环图和进行动态规划算法，是分词算法的核心。
###TransCode模块
TransCode.cpp/TransCode.h 负责转换编码类型，将utf8和gbk都转换成`uint16_t`类型，也负责逆转换。
###HMMSegment模块
HMMSegment.cpp/HMMSegment.h
是根据HMM模型来进行分词，主要算法思路是根据(B,E,M,S)四个状态来代表每个字的隐藏状态。
HMM模型由dicts/下面的`hmm_model.utf8`提供。

##Demo

### Segment's demo
```
cd ./demo;
make;
./segment_demo testlines.gbk
```

run `./segment_demo` to get help.

```
usage:
        ./segment_demo <filename> [options]
options:
        --dictpath      If is not specified, the default is ../dicts/jieba.dict.utf8
        --encoding      Supported encoding methods are [gbk, utf-8] for now.
                        If is not specified, the default is utf8.
example:
        ./segment_demo testlines.utf8 --encoding utf-8 --dictpath ../dicts/jieba.dict.utf8
        ./segment_demo testlines.gbk --encoding gbk --dictpath ../dicts/jieba.dict.gbk

```

##Contact
如果有运行问题或者任何疑问，欢迎联系 : wuyanyi09@gmail.com

##Thanks
"结巴中文"分词作者: SunJunyi  
https://github.com/fxsjy/jieba


