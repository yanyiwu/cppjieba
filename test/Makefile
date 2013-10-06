CXX    := g++
LD     := g++
AR     := ar rc

INCS := -I../cppjieba/

DEBUG_CXXFLAGS     := -g -Wall -DDEBUG -DUT $(INCS)

CXXFLAGS       := ${DEBUG_CXXFLAGS}
LDFLAGS      := ${DEBUG_LDFLAGS}

DOLINK := $(LD) $(LDFLAGS) 
DOPACK := $(AR)  
SOURCES := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SOURCES))
UTS := $(patsubst %.cpp,%.ut,$(SOURCES))

CPPJIEBADIR = ../cppjieba
LIBCPPJIEBA = $(CPPJIEBADIR)/libcppjieba.a

CPPCOMMONDIR = ../cppcommon
LIBCPPCM = $(CPPCOMMONDIR)/libcm.a

LIBA := $(LIBCPPJIEBA) $(LIBCPPCM)
# remove the objs after compilation
.PHONY: clean $(LIBA)

# Main Targets
all: $(UTS)

# This is a suffix rule 
#.c.o: 
%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $<
%.ut: %.o $(LIBA)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(LIBCPPJIEBA): 
	cd $(CPPJIEBADIR) && $(MAKE)

$(LIBCPPCM): 
	cd $(CPPCOMMONDIR) && $(MAKE)

clean:
	rm -f *.o *.ut *.d *.d.*
#	cd $(CPPJIEBADIR) && make clean
#	cd $(CPPCOMMONDIR) && make clean

sinclude $(SOURCES:.cpp=.d)
%.d:%.cpp
	@set -e; rm -f $@; \
	$(CXX) -MM $< > $@.$$$$; \
	sed 's,\($*\).o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
