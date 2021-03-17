CC = gcc
CFLAGS = -Wall -Werror -g
LIB = 
OUTPUT_OPTION = -MMD -MP -o $@

SOURCE = main.c list_sort.c
OBJS = $(SOURCE:.c=.o)
DEPS = $(SOURCE:.c=.d)
-include ${DEPS}

sandbox: ${OBJS}
	${CC} ${OBJS} -o main

clean:
	rm *.d
	rm *.o
	rm main
