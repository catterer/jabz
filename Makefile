parser := parser.o
ljabz := jabz.a
jabz := jabz.o
jabztool := jabz

LDFLAGS=-ljansson
CC=g++
CPPFLAGS += -std=c++17 -Wall -I. -g
LINK := $(CC) $(CPPFLAGS) $(LDFLAGS)

.PHONY: all
$(jabztool): $(ljabz)
	$(CC) $(CPPFLAGS) $(LDFLAGS) jabz_tool.cc $(ljabz) -o $(jabztool)

$(ljabz): $(parser) $(jabz)
	$(LD) -r $(parser) $(jabz) -o $(ljabz)

$(parser):
	$(CC) $(CPPFLAGS) $(LDFLAGS) parser.cc -c -o $(parser)

$(jabz):
	$(CC) $(CPPFLAGS) $(LDFLAGS) jabz.cc -c -o $(jabz)

all: $(ljabz) $(jabztool)
clean:
	rm -rf $(ljabz) $(jabztool) $(parser) $(jabz)
