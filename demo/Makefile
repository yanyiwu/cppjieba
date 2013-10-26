CXX    := g++
LD     := g++
AR     := ar rc

INCS := -I../limonp

DEBUG_CXXFLAGS     := -g -Wall -DDEBUG 
RELEASE_CXXFLAGS   := -Wall -O3

ifeq (YES, ${DEBUG})
   CXXFLAGS       := ${DEBUG_CXXFLAGS}
   LDFLAGS      := ${DEBUG_LDFLAGS}
else
   CXXFLAGS     := ${RELEASE_CXXFLAGS}
   LDFLAGS      := ${RELEASE_LDFLAGS}
endif

DOLINK := $(LD) $(LDFLAGS) 
SOURCES := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SOURCES))

CPPJIEBADIR = ../cppjieba
LIBCPPJIEBA = $(CPPJIEBADIR)/libcppjieba.a

# remove the objs after compilation
.PHONY: clean $(LIBCPPJIEBA) 

# Main Targets
all: keywordext_demo segment_demo

# This is a suffix rule 
#.c.o: 
%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $<

keywordext_demo: keywordext_demo.o $(LIBCPPJIEBA)
	$(DOLINK) -o $@ $^

segment_demo: segment_demo.o $(LIBCPPJIEBA)
	$(DOLINK) -o $@ $^

$(LIBCPPJIEBA): 
	cd $(CPPJIEBADIR) && $(MAKE)

clean:
	rm -f *.o *.ut *.d *.d.* keywordext_demo segment_demo
	cd $(CPPJIEBADIR) && make clean

sinclude $(SOURCES:.cpp=.d)
%.d:%.cpp
	@set -e; rm -f $@; \
	$(CXX) -MM $< > $@.$$$$; \
	sed 's,\($*\).o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
