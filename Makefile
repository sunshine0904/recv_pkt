CC:=gcc
CFLAGS:=-w -g
OBJS:=recv

all:$(OBJS)

recv:pkt.h recv_pkt.c
	$(CC) $(CFLAGS) $^ -o $@
.PHONY:clean

clean:
	-rm $(OBJS) *.gch 
