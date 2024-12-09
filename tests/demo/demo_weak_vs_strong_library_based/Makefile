# *
# * Copyright 2023 RDK Management
# *
# * Licensed under the Apache License, Version 2.0 (the "License");
# * you may not use this file except in compliance with the License.
# * You may obtain a copy of the License at
# *
# * http://www.apache.org/licenses/LICENSE-2.0
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# *


ifeq ($(TARGET),)
CC = gcc
else
#CC = rm-rdk-linux-gnueabi-gcc -mthumb -mfpu=vfp -mcpu=cortex-a9 -mfloat-abi=soft -mabi=aapcs-linux -mno-thumb-interwork -ffixed-r8 -fomit-frame-pointer
endif
CFLAGS = -Wall -Wextra
LDFLAGS = 

all: weak_linked_first_static_lib strong_linked_first_static_lib weak_first_link_shared_lib strong_first_link_shared_lib

libweak/libweak.a:
	$(MAKE) -C libweak/ all

libstrong/libstrong.a:
	$(MAKE) -C libstrong/ all

weak_linked_first_static_lib: main.c libweak/libweak.a libstrong/libstrong.a
	$(CC) $(CFLAGS) -o weak_linked_first_static_lib main.c -Llibweak -lweak -Llibstrong -lstrong

strong_linked_first_static_lib: main.c libweak/libweak.a libstrong/libstrong.a
	$(CC) $(CFLAGS) -o strong_linked_first_static_lib main.c -Llibstrong -lstrong -Llibweak -lweak

weak_first_link_shared_lib: main.c libweak/libweak1.so libstrong/libstrong1.so
	$(CC) $(CFLAGS) -o weak_first_link_shared_lib main.c -Llibweak -lweak1 -Llibstrong -lstrong1
	cp libweak/libweak1.so .
	cp libstrong/libstrong1.so .

strong_first_link_shared_lib: main.c libweak/libweak1.so libstrong/libstrong1.so
	$(CC) $(CFLAGS) -o strong_first_link_shared_lib main.c -Llibstrong -lstrong1 -Llibweak -lweak1

clean:
	$(MAKE) -C libweak clean
	$(MAKE) -C libstrong clean
	rm -f weak* strong* libstrong1.so libweak1.so
