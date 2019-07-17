SRC=src/*
DEST=dest/
KERNDIR=../kernel

CC:=i686-elf-g++
CXX:=i686-elf-g++
AR:=i686-elf-ar

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -Iinclude -I$(KERNDIR)/include -D__is_libk -ffreestanding -std=c++17

LIBK_CFLAGS:=$(CFLAGS)
LIBK_CPPFLAGS:= -D__is_libk $(CPPFLAGS)

SOURCES1=\
$(wildcard $(SRC)/*.c) 

SOURCES2=\
$(wildcard $(SRC)/*.cpp)

LIBOBJS1=\
$(SOURCES1:.c=.o)
LIBOBJS2=\
$(SOURCES2:.cpp=.o)

LIBOBJS = $(LIBOBJS1) $(LIBOBJS2)

.PHONY: all clean install rebuild

install: libk.a

libk.a: $(LIBOBJS)
	$(AR) rcs $@ $(LIBOBJS)

all: $(LIBOBJS)

clean:
	rm -f $(LIBOBJS)
	rm -f libk.a
	
rebuild: clean all

%.o : %.c %.h
	$(CC) -c  $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

%.o : %.cpp %.h
	$(CXX) -c  $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

sources:
	echo $(SOURCES1) $(SOURCES2)
targets:
	echo $(LIBOBJS)
#$(LIBOBJS): $(SOURCES)
#	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)