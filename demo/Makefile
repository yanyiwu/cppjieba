CXX    := g++
LD     := g++
AR     := ar rc


DEBUG_CXXFLAGS     := -g -Wall -DDEBUG 
RELEASE_CXXFLAGS   := -Wall -O3

ifeq (YES, ${DEBUG})
   CXXFLAGS       := ${DEBUG_CXXFLAGS}
   LDFLAGS      := ${DEBUG_LDFLAGS}
else
   CXXFLAGS     := ${RELEASE_CXXFLAGS}
   LDFLAGS      := ${RELEASE_LDFLAGS}
endif

INCS := -I../limonp
LINK := -lpthread

SOURCES := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SOURCES))
DEMOS := $(patsubst %.cpp,%.demo,$(SOURCES))

CPPJIEBADIR := ../cppjieba
LIBCPPJIEBA := $(CPPJIEBADIR)/libcppjieba.a

HUSKYDIR := ../husky
LIBHUSKYA := $(HUSKYDIR)/libhusky.a

.PHONY: clean $(LIBCPPJIEBA) 

all: $(DEMOS)

%.demo: %.cpp $(LIBCPPJIEBA) $(LIBHUSKYA)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(INCS) $(LINK)

$(LIBCPPJIEBA): 
	cd $(CPPJIEBADIR) && $(MAKE)

$(LIBHUSKYA): 
	cd $(HUSKYDIR) && $(MAKE)

clean:
	rm -f *.o *.ut *.d *.d.* $(DEMOS)
	cd $(CPPJIEBADIR) && make clean
	cd $(HUSKYDIR) && make clean

sinclude $(SOURCES:.cpp=.d)
%.d:%.cpp
	@set -e; rm -f $@; \
	$(CXX) -MM $< > $@.$$$$; \
	sed 's,\($*\).o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
