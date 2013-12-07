all: testp demo
testp:
	g++ -o test_performance test_performance.cpp -std=c++0x -O3 -pg
demo:
	g++ -o segment.demo segment.cpp -std=c++0x -L/usr/lib/CppJieba -lcppjieba
