# CppJieba字典

文件后缀名代表的是词典的编码方式。
比如filename.utf8 是 utf8编码，filename.gbk 是 gbk编码方式。

## jieba.dict.utf8/gbk

作为最大概率法(MPSegment: Max Probability)分词所使用的词典。

## hmm_model.utf8/gbk

作为隐式马尔科夫模型(HMMSegment: Hidden Markov Model)分词所使用的词典。

__对于MixSegment(混合MPSegment和HMMSegment两者)则同时使用以上两个词典__

