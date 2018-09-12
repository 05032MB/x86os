SRC=.
DEST=objs/
LIBK=../lib/libk.a

CXX:=i686-elf-g++
AR:=i686-elf-ar
ASM:=nasm

#CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -I../lib/include -I./include -D__k_debug -ffreestanding -O2 -Wall -Wextra -fno-exceptions  -fno-rtti

CSOURCES=\
$(wildcard $(SRC)/*.cpp)
ASOURCES=\
$(wildcard $(SRC)/*.asm)


COBJS=\
$(CSOURCES:.cpp=.o)

AOBJS=\
$(ASOURCES:.asm=.oa)

SOURCES:=$(CSOURCES) $(ASOURCES)

OBJS:=$(AOBJS) $(COBJS)

.PHONY: all clean install


all: os.bin
	 $(CXX) -T linker.ld -o os.bin -ffreestanding -O2 -nostdlib $(OBJS) $(LIBK) -lgcc

os.bin: $(OBJS)

clean:
	rm -f $(OBJS)
	rm -f os.bin
	
%.o : %.cpp %.hpp
	 $(CXX) -c  $< -o $@ $(CPPFLAGS)
%.oa : %.asm
	$(ASM) -felf32 $< -o $@
	
adolf:
	echo $(SOURCES)
hitler:
	echo $(OBJS)