CXX    := g++
LD     := g++
AR     := ar rc

DEBUG_CXXFLAGS     := -g -Wall -DDEBUG 
RELEASE_CXXFLAGS   := -Wall -O3

ifeq (YES, ${RELEASE})
   CXXFLAGS     := ${RELEASE_CXXFLAGS}
   LDFLAGS      := ${RELEASE_LDFLAGS}
else
   CXXFLAGS       := ${DEBUG_CXXFLAGS}
   LDFLAGS      := ${DEBUG_LDFLAGS}
endif

DOLINK := $(LD) $(LDFLAGS) -o $@ $^
DOPACK := $(AR)  
SOURCES := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SOURCES))

CMLIB = libcm.a

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $<

all: $(CMLIB)


$(CMLIB): $(OBJS)
	$(DOPACK) $@ $(OBJS)
	
file_functs.ut: file_functs.cpp file_functs.h
	g++ -o $@ $< -DTEST_FILE_FUNCTS
io_functs.test: io_functs.cpp io_functs.h
	g++ -o $@ $< -DTEST_IO_FUNCTS
str_functs.ut: str_functs.cpp str_functs.h
	g++ -o $@ $< -DTEST_STR_FUNCTS 
argv_functs.ut: argv_functs.cpp str_functs.cpp argv_functs.h
	g++ -o $@ argv_functs.cpp str_functs.cpp -DARGV_FUNCTS_UT
encoding.ut: encoding.cpp str_functs.cpp str_functs.h encoding.h
	g++ -o $@ encoding.cpp str_functs.cpp -DENCODING_UT 
vec_functs.test: vec_functs.cpp vec_functs.h vec_functs.tcc
	g++ -o $@ $< -DTEST_VEC_FUNCTS

logger.ut: logger.cpp logger.h file_functs.cpp file_functs.h str_functs.cpp str_functs.h
	g++ -o $@ $< file_functs.cpp str_functs.cpp -DLOGGER_UT 
config.ut: config.cpp config.h 
	g++ -o $@ $< -DCONFIG_UT $(CMLIB)

http_functs.ut: http_functs.cpp http_functs.h
	g++ -o $@ $< -DUT -DDEBUG $(CMLIB)

sort_functs.ut: sort_functs.cpp sort_functs.h
	g++ -o $@ $< -DUT -DDEBUG $(CMLIB)

clean:
	rm -f *.test *.ut *.o $(CMLIB)

