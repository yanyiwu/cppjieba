CC = g++
CCOPT = -Wall -c
LINK = g++
LINKOPT =
PACKA = ar
PARCAOPT = rc
DOLINK = $(LINK) $(LINKOPT) -o $@ $^
SOURCES := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SOURCES))

SRCDIR = ./src
SRCLIB = $(SRCDIR)/libcppjieba.a

# remove the objs after compilation
.INTERMEDIATE:  $(OBJS) *.o
.PHONY: clean $(SRCLIB)

# Main Targets
all: demo

# This is a suffix rule 
#.c.o: 
%.o: %.cpp
	$(CC) $(CCOPT) $<

demo: $(OBJS) $(SRCLIB)
	$(DOLINK) 

$(SRCLIB): $(SRCLIB_DEPS)
	cd $(SRCDIR) && $(MAKE)

clean:
	rm -f *.o *.ut demo

sinclude $(SOURCES:.cpp=.d)
%.d:%.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $< > $@.$$$$; \
	sed 's,\($*\).o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
