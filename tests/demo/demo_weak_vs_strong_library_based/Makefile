ifeq ($(TARGET),)
CC = gcc
else
#CC = rm-rdk-linux-gnueabi-gcc -mthumb -mfpu=vfp -mcpu=cortex-a9 -mfloat-abi=soft -mabi=aapcs-linux -mno-thumb-interwork -ffixed-r8 -fomit-frame-pointer
endif
CFLAGS = -Wall -Wextra
LDFLAGS = 

all: weak_first strong_first weak_first_link_so strong_first_link_so

libweak/libweak.a:
	$(MAKE) -C libweak/ all

libstrong/libstrong.a:
	$(MAKE) -C libstrong/ all

weak_first: main.c libweak/libweak.a libstrong/libstrong.a
	$(CC) $(CFLAGS) -o weak_first main.c -Llibweak -lweak -Llibstrong -lstrong

strong_first: main.c libweak/libweak.a libstrong/libstrong.a
	$(CC) $(CFLAGS) -o strong_first main.c -Llibstrong -lstrong -Llibweak -lweak

weak_first_link_so: main.c libweak/libweak1.so libstrong/libstrong1.so
	$(CC) $(CFLAGS) -o weak_first_link_so main.c -Llibweak -lweak1 -Llibstrong -lstrong1
	cp libweak/libweak1.so .
	cp libstrong/libstrong1.so .

strong_first_link_so: main.c libweak/libweak1.so libstrong/libstrong1.so
	$(CC) $(CFLAGS) -o strong_first_link_so main.c -Llibstrong -lstrong1 -Llibweak -lweak1

clean:
	$(MAKE) -C libweak clean
	$(MAKE) -C libstrong clean
	rm -f weak_first* strong_first* libstrong1.so libweak1.so
