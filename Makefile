#compiler and flags
CC			= gcc
CFLAGS	+=-Wall -O3 -lpq
LIBS = -I/usr/include/postgresql -L/usr/lib/
DEPS = testpsql.h

testpsql: testpsql.c
	$(CC) $(LIBS) -o testpsql testpsql.c $(CFLAGS)

.PHONY: clean

clean:
	rm testpsql

