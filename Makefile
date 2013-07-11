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
SRCLIB = $(SRCDIR)/cppjiebalib.a

# remove the objs after compilation
.INTERMEDIATE:  $(OBJS)
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
