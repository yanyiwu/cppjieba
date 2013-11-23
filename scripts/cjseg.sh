if [ $# -lt 2 ]; then
    echo "usage: $0 <file>"
    exit 1
fi
cjsegment --dictpath /usr/share/CppJieba/dicts/jieba.dict.utf8 --modelpath /usr/share/CppJieba/dicts/hmm_model.utf8 $1
