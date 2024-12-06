CC = gcc
CFLAGS = -Wall -Wextra -fPIC
LDFLAGS = -shared
AR = ar
ARFLAGS = rcs

all: libweak.a libweak1.so

libweak.a: weak.o
	$(AR) $(ARFLAGS) libweak.a weak.o

libweak1.so: weak.o
	$(CC) $(LDFLAGS) -shared -o libweak1.so weak.o

weak.o: weak.c
	$(CC) $(CFLAGS) -c weak.c

clean:
	rm -f *.o libweak*.*
