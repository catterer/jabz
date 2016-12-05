all: $(jabz)

$(jabz): $(OBJS) 
	$(LINK.c) $(program_OBJS) -c -o $(jabz)

jabz := jabz.a

LDFLAGS=-ljansson
CC=g++
CPPFLAGS += -std=c++17 -Wall -I.
LINK := $(CC) $(CPPFLAGS) $(LDFLAGS)

.PHONY: all
$(jabz):
	$(CC) $(CPPFLAGS) $(LDFLAGS) jabz.cc -c -o jabz.a
all: $(jabz)
clean:
	rm -rf $(jabz)
