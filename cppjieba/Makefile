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

SOURCES := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SOURCES))

INC := -I../limonp

LIBA := libcppjieba.a

# remove the objs after compilation
.INTERMEDIATE: 
#.PHONY: clean $(CMLIB) 
.PHONY: clean 

all: $(LIBA)

# This is a suffix rule 
#.c.o: 
%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< $(INC)


${LIBA}: $(OBJS)
	$(AR) $@ $(OBJS) 

#unit test
Trie.ut: Trie.cpp Trie.h globals.h  TransCode.cpp TransCode.hpp $(CMLIB)
	$(CXX) -o $@ $(CXXFLAGS) Trie.cpp TransCode.cpp  -DTRIE_UT $(CMLIB)

MPSegment.ut: MPSegment.cpp Trie.cpp MPSegment.h Trie.h globals.h $(CMLIB)
	$(CXX) -o $@ $(CXXFLAGS) MPSegment.cpp Trie.cpp TransCode.cpp -DSEGMENT_UT $(CMLIB) 

KeyWordExt.ut: KeyWordExt.cpp KeyWordExt.h MPSegment.h Trie.h globals.h TransCode.cpp TransCode.hpp  $(CMLIB)
	$(CXX) -o $@ $(CXXFLAGS) KeyWordExt.cpp MPSegment.cpp Trie.cpp TransCode.cpp -DKEYWORDEXT_UT $(CMLIB)

TransCode.ut: TransCode.cpp TransCode.hpp globals.h $(CMLIB)
	$(CXX) -o $@ $(CXXFLAGS) TransCode.cpp -DCPPJIEBA_TRANSCODE_UT $(CMLIB) 
HMMSegment.ut: HMMSegment.cpp TransCode.cpp TransCode.hpp HMMSegment.h $(CMLIB)
	$(CXX) -o $@ $(CXXFLAGS) TransCode.cpp HMMSegment.cpp -DHMMSEGMENT_UT $(CMLIB) 
MixSegment.ut: MixSegment.cpp MixSegment.h HMMSegment.cpp MPSegment.cpp Trie.cpp MPSegment.h Trie.h globals.h $(CMLIB)
	$(CXX) -o $@ $(CXXFLAGS) MixSegment.cpp HMMSegment.cpp MPSegment.cpp Trie.cpp TransCode.cpp -DMIXSEGMENT_UT $(CMLIB) 
ChineseFilter.ut: ChineseFilter.cpp ChineseFilter.h
	$(CXX) -o $@ $(CXXFLAGS) ChineseFilter.cpp -DUT

clean:
	rm -f *.o *.d *.d.* *.ut $(LIBA) 

sinclude $(SOURCES:.cpp=.d)
%.d:%.cpp
	@set -e; rm -f $@; \
	$(CXX) -MM $< > $@.$$$$; \
	sed 's,\($*\).o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
