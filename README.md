#CppJieba是"结巴"中文分词的c++库

##Detail
* 现在支持utf8,gbk编码的分词。默认编码是utf8。  

##Algorithm

###Trie树
Trie.cpp/Trie.h 负责载入词典的trie树，主要供Segment模块使用。
###Segment模块
MPSegment.cpp/MPSegment.h 
(Maximum Probability)最大概率法:负责根据Trie树构建有向无环图和进行动态规划算法，是分词算法的核心。
###TransCode模块
TransCode.cpp/TransCode.h 负责转换编码类型，将utf8和gbk都转换成`uint16_t`类型，也负责逆转换。
###HMMSegment模块
HMMSegment.cpp/HMMSegment.h
是根据HMM模型来进行分词，主要算法思路是根据(B,E,M,S)四个状态来代表每个字的隐藏状态。
HMM模型由dicts/下面的`hmm_model.utf8`提供。
分词算法即viterbi算法。

##Demo

### MPSegment's demo

__这部分的功能经过线上考验，一直稳定运行，暂时没有发现什么bug。__

```
cd ./demo;
make;
./segment_demo testlines.utf8
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
./segment_demo testlines.utf8 --modelpath ../dicts/hmm_model.utf8 --algorithm cutHMM
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
./segment_demo testlines.utf8 --algorithm cutMix
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

##Help

run `./segment_demo` to get help.

```
usage:
        ./segment_demo[options] <filename>
options:
        --algorithm     Supported encoding methods are [cutDAG, cutHMM, cutMix] for now.
                        If not specified, the default is cutDAG
        --dictpath      If not specified, the default is ../dicts/jieba.dict.utf8
        --modelpath     If not specified, the default is ../dicts/hmm_model.utf8
        --encoding      Supported encoding methods are [gbk, utf-8] for now.
                        If not specified, the default is utf8.
example:
        ./segment_demo testlines.utf8 --encoding utf-8 --dictpath ../dicts/jieba.dict.utf8
        ./segment_demo testlines.utf8 --modelpath ../dicts/hmm_model.utf8 --algorithm cutHMM
        ./segment_demo testlines.utf8 --modelpath ../dicts/hmm_model.utf8 --algorithm cutMix
        ./segment_demo testlines.gbk --encoding gbk --dictpath ../dicts/jieba.dict.gbk

```

##Contact
如果有运行问题或者任何疑问，欢迎联系 : wuyanyi09@gmail.com

##Thanks
"结巴中文"分词作者: SunJunyi  
https://github.com/fxsjy/jieba


