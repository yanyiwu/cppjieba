g++ -c -fPIC Jieba.cpp -o Jieba.o
g++ -shared -Wl,-soname,libJieba.so -o libJieba.so  Jieba.o

