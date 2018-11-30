SRC=.
DEST=objs/
LIBK=../lib/libk.a

CXX:=i686-elf-g++
#AR:=i686-elf-ar
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

CRTBEGIN_OBJ:=$(shell $(CXX) $(CPPFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CXX) $(CPPFLAGS) -print-file-name=crtend.o)

LINK_LIST=\
crti.oa \
$(CRTBEGIN_OBJ) \
$(COBJS) \
$(AOBJS) \
$(LIBK) \
$(CRTEND_OBJ) \
crtn.oa

SOURCES:=$(CSOURCES) $(ASOURCES)

OBJS:=$(AOBJS) $(COBJS)

.PHONY: all clean install


all: os.bin
	 $(CXX) -T linker.ld -o os.bin -ffreestanding -O2 -nostdlib $(LINK_LIST) -lgcc

os.bin: $(LINK_LIST)
#$(OBJS) crtn.oa crti.oa

clean:
	rm -f $(OBJS)
	rm -f $(AOBJS)
	rm -f os.bin
	
%.o : %.cpp %.hpp
	 $(CXX) -c  $< -o $@ $(CPPFLAGS)
%.oa : %.asm
	$(ASM) -felf32 $< -o $@
%.oa : %.spec
	$(ASM) -felf32 $< -o $@
	
sources:
	echo $(SOURCES)
targets:
	echo $(OBJS)