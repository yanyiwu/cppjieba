CC = g++
CCOPT = -Wall -c
LINK = g++
LINKOPT =
PACKA = ar
PACKAOPT = rc
DOLINK = $(LINK) $(LINKOPT) -o $@ $^
DOPACK = $(PACKA) $(PACKAOPT) $@ 
SOURCES := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SOURCES))

CMDIR = ./cppcommon
CMLIB = $(CMDIR)/libcm.a

TMPDIR = ./cppjiebatmp

LIBA = libcppjieba.a

# remove the objs after compilation
.INTERMEDIATE: 
.PHONY: clean $(CMLIB) 

all: $(LIBA)

# This is a suffix rule 
#.c.o: 
%.o: %.cpp
	$(CC) $(CCOPT) $<

$(LIBA): $(TMPDIR) $(OBJS) $(CMLIB) 
	cp $(CMLIB) $(TMPDIR) && cd $(TMPDIR) && ar x `basename $(CMLIB)`
	$(DOPACK) $(OBJS) $(TMPDIR)/*.o 
	rm -rf $(TMPDIR)

$(TMPDIR):
	mkdir $@

$(CMLIB):
	cd $(CMDIR) && $(MAKE)

#unit test
Trie.ut: Trie.cpp Trie.h globals.h  TransCode.cpp TransCode.h $(CMLIB)
	$(CC) -o $@ Trie.cpp TransCode.cpp  -DTRIE_UT $(CMLIB)

Segment.ut: Segment.cpp Trie.cpp Segment.h Trie.h globals.h $(CMLIB)
	$(CC) -o $@ Segment.cpp Trie.cpp TransCode.cpp -DSEGMENT_UT $(CMLIB) 

KeyWordExt.ut: KeyWordExt.cpp KeyWordExt.h Segment.h Trie.h globals.h TransCode.cpp TransCode.h  $(CMLIB)
	$(CC) -o $@ KeyWordExt.cpp Segment.cpp Trie.cpp TransCode.cpp -DKEYWORDEXT_UT $(CMLIB)

TransCode.ut: TransCode.cpp TransCode.h globals.h $(CMLIB)
	$(CC) -o $@ TransCode.cpp -DCPPJIEBA_TRANSCODE_UT $(CMLIB) 

clean:
	rm -f *.o *.d *.ut $(LIBA) 
	rm -rf $(TMPDIR)
	cd $(CMDIR) && make clean

sinclude $(SOURCES:.cpp=.d)
%.d:%.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $< > $@.$$$$; \
	sed 's,\($*\).o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
