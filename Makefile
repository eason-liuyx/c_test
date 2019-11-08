PROGRAM := main
SRCS := $(wildcard *.c)
OBJS := ${SRCS:.c=.o}
CFLAGS=-g


$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) -o $(PROGRAM)

clean:
	@- $(RM) $(PROGRAM)
	@- $(RM) $(OBJS)
