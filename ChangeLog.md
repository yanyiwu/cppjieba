# CppJieba ChangeLog

## v2.5.0 (untagged)

1. 使得 QuerySegment 支持自定义词典（可选参数）。
2. 使得 KeywordExtractor 支持自定义词典（可选参数）。
3. 修改 Code Style ，参照 google code style 。 
4. 增加更详细的错误日志，在初始化过程中合理使用LogFatal。

## v2.4.4

1. 修改两条更细粒度的特殊过滤规则，将连续的数字（包括浮点数）和连续的字母单独切分出来（而不会混在一起）。
2. 修改最大概率法时动态规划过程需要使用的 DAG 数据结构（同时也修改 Trie 的 DAG 查询函数），提高分词速度 8% 。
3. 使用了 `Aho-Corasick-Automation` 算法提速 Trie 查找的过程等优化，提升性能。
4. 增加词性标注的两条特殊规则。

## v2.4.3

1. 更新 [Husky] 服务代码，新 [Husky] 为基于线程池的服务器简易框架。并且修复当 HTTP POST 请求时 body 过长数据可能丢失的问题。
2. 修改 PosTagger 的参数结构，删除暂时无用的参数。并添加使用自定义字典的参数，也就是支持 **自定义词性**。
3. 更好的支持 `mac osx` (原谅作者如此屌丝，这么晚才买 `mac` )。
4. 支持 `Docker` ，具体请见 `Dockerfile` 。

## v2.4.2

1. 适当使用 `vector`， 的基础上，使用`Limonp/LocalVector.hpp`作为`Unicode`的类型等优化，约提高性能 `30%`。
2. 使 `cjserver` 支持用户自定义词典，通过在 `conf/server.conf` 里面配置 `user_dict_path` 来实现。
3. 修复 `MPSegment` 切词时，当句子中含有特殊字符时，切词结果不完整的问题。
4. 修改 `FullSegment` 减少内存使用。 
5. 修改 `-std=c++0x` 或者 `-std=c++11` 时编译失败的问题。

## v2.4.1

1. 完善一些特殊字符和字母串的切词效果。
2. 提高关键词抽取的速度。
3. 提供用户自定义词典的接口。
4. 将server相关的代码独立出来，单独放在`server/`目录下。
5. 修复用户自定义词典中单字会被MixSegment的新词发现功能给忽略的问题。也就是说，现在的词典是用户词典优先级最高，其次是自带的词典，再其次是新词发现出来的词。

## v2.4.0

1. 适配更低级版本的`g++`和`cmake`，已在`g++ 4.1.2`和`cmake 2.6`上测试通过。
2. 修改一些测试用例的文件，减少测试时编译的时间。
3. 修复`make install`相关的问题。
4. 增加HTTP服务的POST请求接口。
5. 拆分`Trie.hpp`成`DictTrie.hpp`和`Trie.hpp`，将trie树这个数据结构抽象出来，并且修复Trie这个类潜在的bug并完善单元测试。
6. 重写cjserver的启动和停止，新启动和停止方法详见README.md。

## v2.3.4

1. 修改了设计上的问题，删除了`TrieManager`这个类，以避免造成一些可能的隐患。
2. 增加`stop_words.utf8`词典，并修改`KeywordExtractor`的初始化函数用以使用此词典。
3. 优化了`Trie`树相关部分代码结构。

## v2.3.3

1. 修复因为使用unordered_map导致的在不同机器上结果不一致的问题。
2. 将部分数据结果从unordered_map改为map，提升了差不多1/6的切词速度。(因为unordered_map虽然查找速度快，但是在范围迭代的效率较低。)

## v2.3.2

1. 修复单元测试的问题，有些case在x84和x64中结果不一致。
2. merge进词性标注的简单版本。

## v2.3.1

1. 修复安装时的服务启动问题（不过安装切词服务只是linux下的一个附加功能，不影响核心代码。）

## v2.3.0 

1. 增加`KeywordExtractor.hpp`来进行关键词抽取。
2. 使用`gtest`来做单元测试。

## v2.2.0

1. 性能优化，提升切词速度约6倍。
2. 其他暂时也想不起来了。

## v2.1.1 (v2.1.1之前的统统一起写在 v2.1.1里面了)

1. 完成__最大概率分词算法__和__HMM分词算法__，并且将他们结合起来成效果最好的`MixSegment`。
2. 进行大量的代码重构，将主要的功能性代码都写成了hpp文件。
3. 使用`cmake`工具来管理项目。
4. 使用 [Limonp]作为工具函数库，比如日志，字符串操作等常用函数。
5. 使用 [Husky] 搭简易分词服务的服务器框架。

[Limonp]:http://github.com/yanyiwu/limonp.git
[Husky]:http://github.com/yanyiwu/husky.git
