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
SOURCES = $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SOURCES))

INC := -I../limonp
LIBA := libhusky.a

.PHONY: clean 

all: $(LIBA)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< $(INC)

${LIBA}: $(OBJS) 
	$(DOPACK) $@ $(OBJS) 

clean:
	rm -f *.o *.d *.d.* $(LIBA) 

sinclude $(SOURCES:.cpp=.d)
%.d:%.cpp
	@set -e; rm -f $@; \
	$(CXX) -MM $< > $@.$$$$; \
	sed 's,\($*\).o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
