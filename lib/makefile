SRC=src/*
DEST=dest/
KERNDIR=../kernel

CC:=i686-elf-g++
AR:=i686-elf-ar

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -Iinclude -I$(KERNDIR)/include -D__is_libk

LIBK_CFLAGS:=$(CFLAGS)
LIBK_CPPFLAGS:= -D__is_libk $(CPPFLAGS)

SOURCES=\
$(wildcard $(SRC)/*.c)

LIBOBJS=\
$(SOURCES:.c=.o)
#$(patsubst $(SRC)/*.c, $(DEST)/*.o, $(SOURCES))
#

.PHONY: all clean install

install: libk.a

libk.a: $(LIBOBJS)
	$(AR) rcs $@ $(LIBOBJS)

all: $(LIBOBJS)

clean:
	rm -f $(LIBOBJS)
	rm -f libk.a
	
%.o : %.c %.h
	$(CC) -c  $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

sources:
	echo $(SOURCES)
targets:
	echo $(LIBOBJS)
#$(LIBOBJS): $(SOURCES)
#	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)