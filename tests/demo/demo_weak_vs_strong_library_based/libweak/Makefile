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
