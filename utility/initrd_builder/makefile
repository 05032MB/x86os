CXX=g++

SRC=initrd_builder.cpp
OBJ = $(SRC:.cpp=.o)

########

install: all install-rdisk

install-rdisk:
	cp initramdisk ../isogen/iso/boot/initramdisk

all: builder

builder: $(OBJ)
	$(CXX)  $^ -o $@ -Wall -Wextra -std=c++11

clean:
	rm -f $(OBJ) prog.exe
		
rebuild: clean all 

.PHONY: all clean rebuild install-rdisk install