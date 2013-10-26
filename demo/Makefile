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
DEMOS := $(patsubst %.cpp,%.demo,$(SOURCES))

CPPJIEBADIR := ../cppjieba
LIBCPPJIEBA := $(CPPJIEBADIR)/libcppjieba.a

.PHONY: clean $(LIBCPPJIEBA) 

all: $(DEMOS)

%.demo: %.cpp $(LIBCPPJIEBA)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(INCS)

$(LIBCPPJIEBA): 
	cd $(CPPJIEBADIR) && $(MAKE)

clean:
	rm -f *.o *.ut *.d *.d.* $(DEMOS)
	cd $(CPPJIEBADIR) && make clean

sinclude $(SOURCES:.cpp=.d)
%.d:%.cpp
	@set -e; rm -f $@; \
	$(CXX) -MM $< > $@.$$$$; \
	sed 's,\($*\).o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
