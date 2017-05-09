README
------------
1. g++ -c -fPIC Jieba.cpp -o Jieba.o
2. g++ -shared -Wl,-soname,libJieba.so -o libJieba.so  Jieba.o

