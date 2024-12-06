ifeq ($(TARGET),)
CC = gcc
else
#CC = rm-rdk-linux-gnueabi-gcc -mthumb -mfpu=vfp -mcpu=cortex-a9 -mfloat-abi=soft -mabi=aapcs-linux -mno-thumb-interwork -ffixed-r8 -fomit-frame-pointer
endif
CFLAGS = -Wall -Wextra -fPIC
LDFLAGS = -shared
AR = ar
ARFLAGS = rcs

all: libstrong.a libstrong1.so

libstrong.a: strong.o
	$(AR) $(ARFLAGS) libstrong.a strong.o

libstrong1.so: strong.o
	$(CC) $(LDFLAGS) -shared -o libstrong1.so strong.o

strong.o: strong.c
	$(CC) $(CFLAGS) -c strong.c


clean:
	rm -f *.o libstrong*.*
