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

DOLINK := $(LD) $(LDFLAGS) -o $@ $^
DOPACK := $(AR)  
SOURCES := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SOURCES))

CMDIR := ./cppcommon
CMLIB := $(CMDIR)/libcm.a

TMPDIR := ./cppjiebatmp

LIBA := libcppjieba.a

# remove the objs after compilation
.INTERMEDIATE: 
.PHONY: clean $(CMLIB) 

all: $(LIBA)

# This is a suffix rule 
#.c.o: 
%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $<

${LIBA}: $(TMPDIR) $(OBJS) $(CMLIB) 
	cp $(CMLIB) $(TMPDIR) && cd $(TMPDIR) && ar x `basename $(CMLIB)`
	$(DOPACK) $@ $(OBJS) $(TMPDIR)/*.o 
	rm -rf $(TMPDIR)

$(TMPDIR):
	mkdir $@

$(CMLIB):
	cd $(CMDIR) && $(MAKE)

#unit test
Trie.ut: Trie.cpp Trie.h globals.h  TransCode.cpp TransCode.h $(CMLIB)
	$(CXX) -o $@ $(CXXFLAGS) Trie.cpp TransCode.cpp  -DTRIE_UT $(CMLIB)

MPSegment.ut: MPSegment.cpp Trie.cpp MPSegment.h Trie.h globals.h $(CMLIB)
	$(CXX) -o $@ $(CXXFLAGS) MPSegment.cpp Trie.cpp TransCode.cpp -DSEGMENT_UT $(CMLIB) 

KeyWordExt.ut: KeyWordExt.cpp KeyWordExt.h MPSegment.h Trie.h globals.h TransCode.cpp TransCode.h  $(CMLIB)
	$(CXX) -o $@ $(CXXFLAGS) KeyWordExt.cpp MPSegment.cpp Trie.cpp TransCode.cpp -DKEYWORDEXT_UT $(CMLIB)

TransCode.ut: TransCode.cpp TransCode.h globals.h $(CMLIB)
	$(CXX) -o $@ $(CXXFLAGS) TransCode.cpp -DCPPJIEBA_TRANSCODE_UT $(CMLIB) 
HMMSegment.ut: HMMSegment.cpp TransCode.cpp TransCode.h HMMSegment.h $(CMLIB)
	$(CXX) -o $@ $(CXXFLAGS) TransCode.cpp HMMSegment.cpp -DHMMSEGMENT_UT $(CMLIB) 

clean:
	rm -f *.o *.d *.ut $(LIBA) 
	rm -rf $(TMPDIR)
	cd $(CMDIR) && make clean

sinclude $(SOURCES:.cpp=.d)
%.d:%.cpp
	@set -e; rm -f $@; \
	$(CXX) -MM $< > $@.$$$$; \
	sed 's,\($*\).o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
