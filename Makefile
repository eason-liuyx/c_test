PROGRAM := main
SRCS := $(wildcard *.c)
OBJS := ${SRCS:.c=.o}
CFLAGS=-g -std=gnu99


$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) -o $(PROGRAM) -lm

clean:
	@- $(RM) $(PROGRAM)
	@- $(RM) $(OBJS)
