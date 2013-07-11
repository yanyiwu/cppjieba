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

$(LIBA): $(OBJS) $(CMLIB) 
	mkdir $(TMPDIR)
	cp $(CMLIB) $(TMPDIR) && cd $(TMPDIR) && ar x `basename $(CMLIB)`
	$(DOPACK) $(OBJS) $(TMPDIR)/*.o 
	rm -rf $(TMPDIR)

$(CMLIB):
	cd $(CMDIR) && $(MAKE)

#unit test
Trie.ut: Trie.cpp Trie.h globals.h $(CMLIB)
	$(CC) -o $@ $< -DTRIE_UT $(CMLIB) 

Segment.ut: Segment.cpp Trie.cpp Segment.h Trie.h globals.h $(CMLIB)
	$(CC) -o $@ Segment.cpp Trie.cpp -DSEGMENT_UT $(CMLIB) 

clean:
	rm -f *.o *.d *.ut $(LIBA) 
	rm -rf $(TMPDIR)
	cd $(CMDIR) && make clean

%.d:%.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $< > $@.$$$$; \
	sed 's,\($*\).o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
