CXX    := g++
LD     := g++
AR     := ar rc

INCS := -I. -I./cppcommon

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
DOPACK := $(AR)  
SOURCES := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SOURCES))

SRCDIR = ./src
SRCLIB = $(SRCDIR)/libcppjieba.a

# remove the objs after compilation
.INTERMEDIATE:  $(OBJS) 
.PHONY: clean $(SRCLIB)

# Main Targets
all: demo

# This is a suffix rule 
#.c.o: 
%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $<

demo: $(OBJS) $(SRCLIB)
	$(DOLINK) -o $@ $^

$(SRCLIB): 
	cd $(SRCDIR) && $(MAKE)

clean:
	rm -f *.o *.ut demo
	cd $(SRCDIR) && make clean

sinclude $(SOURCES:.cpp=.d)
%.d:%.cpp
	@set -e; rm -f $@; \
	$(CXX) -MM $< > $@.$$$$; \
	sed 's,\($*\).o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
