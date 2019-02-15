SRC=.
DEST=objs
LIBK=../lib/libk.a

CXX:=i686-elf-g++
ASM:=nasm

CPPFLAGS:=$(CPPFLAGS) -I../lib/include -I./include -D__k_debug -ffreestanding -O2 -Wall -Wextra -fno-exceptions  -fno-rtti

CSOURCES=\
$(wildcard $(SRC)/*.cpp)
ASOURCES=\
$(wildcard $(SRC)/*.asm)


COBJS=\
$(addprefix $(DEST)/, $(CSOURCES:.cpp=.o))

AOBJS=\
$(addprefix $(DEST)/, $(ASOURCES:.asm=.oa))

CRTBEGIN_OBJ:=$(shell $(CXX) $(CPPFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CXX) $(CPPFLAGS) -print-file-name=crtend.o)

LINK_LIST=\
$(DEST)/crti.oa \
$(CRTBEGIN_OBJ) \
$(COBJS) \
$(AOBJS) \
$(LIBK) \
$(CRTEND_OBJ) \
$(DEST)/crtn.oa

SOURCES:=$(CSOURCES) $(ASOURCES)

OBJS:=$(AOBJS) $(COBJS)
	
.PHONY: all clean install


all: os.bin
	 $(CXX) -T linker.ld -o os.bin -ffreestanding -O2 -nostdlib $(LINK_LIST) -lgcc

os.bin: $(LINK_LIST)

clean:
	rm -f $(OBJS)
	rm -f $(AOBJS)
	rm -f os.bin
	
$(DEST)/%.o : %.cpp
	 $(CXX) -c  $< -o $@ $(CPPFLAGS)
$(DEST)/%.oa : %.asm
	$(ASM) -felf32 $< -o $@
$(DEST)/%.oa : %.spec
	$(ASM) -felf32 $< -o $@
	
sources:
	echo $(SOURCES)
targets:
	echo $(OBJS)