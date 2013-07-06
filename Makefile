CC = g++
CCOPT = -Wall -c
LINK = g++
LINKOPT =
PACKA = ar
PARCAOPT = rc
DOLINK = $(LINK) $(LINKOPT) -o $@ $^
SOURCES := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SOURCES))

CMDIR = ./cppcommon/
CMLIB = $(CMDIR)cmlib.a


# remove the objs after compilation
.INTERMEDIATE: 
.PHONY: clean

# This is a suffix rule 
#.c.o: 
%.o: %.cpp
	$(CC) $(CCOPT) $<

# Main Targets
all: main


main: $(OBJS) $(CMLIB)
	$(DOLINK)

$(CMLIB): $(CMDIR)
	cd $(CMDIR) && $(MAKE)

#unit test
Trie.ut: Trie.cpp Trie.h globals.h $(CMLIB)
	$(CC) -o $@ $< -DTRIE_UT $(CMLIB) 

Segment.ut: Segment.cpp Trie.cpp Segment.h Trie.h globals.h $(CMLIB)
	$(CC) -o $@ Segment.cpp Trie.cpp -DSEGMENT_UT $(CMLIB) 


clean:
	rm -f *.o *.ut $(CMLIB) main

sinclude $(SOURCES:.cpp=.d)
%.d:%.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $< > $@.$$$$; \
	sed 's,\($*\).o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
