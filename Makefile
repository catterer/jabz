ljabz := jabz.a
jabztool := jabz

LDFLAGS=-ljansson
CC=g++
CPPFLAGS += -std=c++17 -Wall -I. -g
LINK := $(CC) $(CPPFLAGS) $(LDFLAGS)

.PHONY: all
$(jabztool): $(ljabz)
	$(CC) $(CPPFLAGS) $(LDFLAGS) jabz_tool.cc $(ljabz) -o $(jabztool)

$(ljabz):
	$(CC) $(CPPFLAGS) $(LDFLAGS) jabz.cc -c -o $(ljabz)

all: $(ljabz) $(jabztool)
clean:
	rm -rf $(ljabz) $(jabztool)
