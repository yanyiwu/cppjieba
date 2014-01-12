if [ $# -lt 1 ]; then
    echo "usage: $0 <file>"
    exit 1
fi
cjsegment --dictpath /usr/share/CppJieba/dict/jieba.dict.utf8 --modelpath /usr/share/CppJieba/dict/hmm_model.utf8 $1
