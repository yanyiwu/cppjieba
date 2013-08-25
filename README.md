#CppJieba是"结巴"中文分词的c++库


##Detail
>1.现在支持utf8,gbk编码的分词。默认编码是utf8。  
>2.分词算法上还没增加HMM模型这部分。    
>3.关键词抽取是暂时是针对类似title之类的超短语句使用，与一般文本的关键词抽取思路不同。    

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
wuyanyi09@gmail.com

##Thanks
>"结巴中文"分词作者: SunJunyi  
>https://github.com/fxsjy/jieba


