CC = g++
CCOPT = -Wall -c
LINK = g++
LINKOPT = 
PACK = ar
PACKOPT = rc
SOURCES := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SOURCES))

DOPACK = $(PACK) $(PACKOPT) $@ $^
DOLINK = $(LINK) $(LINKOPT) -o $@ $^

CMLIB = libcm.a

%.o: %.cpp
	$(CC) $(CCOPT) $<


all: $(CMLIB)


$(CMLIB): $(OBJS)
	$(DOPACK) 
	
	
file_functs.ut: file_functs.cpp file_functs.h
	g++ -o $@ $< -DTEST_FILE_FUNCTS
io_functs.test: io_functs.cpp io_functs.h
	g++ -o $@ $< -DTEST_IO_FUNCTS
str_functs.ut: str_functs.cpp str_functs.h
	g++ -o $@ $< -DTEST_STR_FUNCTS -liconv
encoding.ut: encoding.cpp str_functs.cpp str_functs.h encoding.h
	g++ -o $@ encoding.cpp str_functs.cpp -DENCODING_UT -liconv
vec_functs.test: vec_functs.cpp vec_functs.h vec_functs.tcc
	g++ -o $@ $< -DTEST_VEC_FUNCTS

logger.ut: logger.cpp logger.h file_functs.cpp file_functs.h str_functs.cpp str_functs.h
	g++ -o $@ $< file_functs.cpp str_functs.cpp -DLOGGER_UT -liconv
config.ut: config.cpp config.h 
	g++ -o $@ $< -DCONFIG_UT $(CMLIB)

clean:
	rm -f *.test *.ut *.o $(CMLIB)

