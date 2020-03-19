直接 fork [yanyiwu/cppjieba: "结巴" 中文分词的 C++ 版本](https://github.com/yanyiwu/cppjieba) 包装，性能接近。

## Install

```bash
$ git clone https://github.com/hscspring/cppjieba.git
$ cd cppjieba
$ pip install .
$ # or :
$ python setup.py install
```

## Usage

```python
from cppjieba import Jieba

jb = Jieba()
# 可以指定自定义路径：
# dict_path: 分词词典路径
# hmm_path: hmm 模型路径
# user_dict_path: 用户自定义词典路径
# idf_path: idf 词典路径
# stop_words_path: 停用词路径
# Example: jb = Jieba(user_dict_path="path/to/your_dict")
        
s = "他来到了网易杭研大厦"
result = jb.cut(s)
print("/".join(result))
# 他/来到/了/网易/杭研/大厦

result = jb.cut(s, hmm=False)
print("/".join(result))
# 他/来到/了/网易/杭/研/大厦

s = "我来到北京清华大学"
result = jb.cut_all(s)
print(result)
# ['我', '来到', '北京', '清华', '清华大学', '华大', '大学']

s = "小明硕士毕业于中国科学院计算所，后在日本京都大学深造"
result = jb.cut_for_search(s)
print(result)
# ['小明', '硕士', '毕业', '于', '中国', '科学', '学院', '科学院', '中国科学院', '计算', '计算所', '，', '后', '在', '日本', '京都', '大学', '日本京都大学', '深造']

s = "我是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。"
result = jb.pseg(s)
print(result)
# [('我', 'r'), ('是', 'v'), ('拖拉机', 'n'), ('学院', 'n'), ('手扶拖拉机', 'n'), ('专业', 'n'), ('的', 'uj'), ('。', 'x'), ('不用', 'v'), ('多久', 'm'), ('，', 'x'), ('我', 'r'), ('就', 'd'), ('会', 'v'), ('升职', 'v'), ('加薪', 'nr'), ('，', 'x'), ('当上', 't'), ('CEO', 'eng'), ('，', 'x'), ('走上', 'v'), ('人生', 'n'), ('巅峰', 'n'), ('。', 'x')]

result = jb.extract(s)
print(result)
# [('CEO', 11.739204307083542), ('升职', 10.8561552143), ('加薪', 10.642581114), ('手扶拖拉机', 10.0088573539), ('巅峰', 9.49395840471)]
```

