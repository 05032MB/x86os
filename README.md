# x86os
x86 operating system

Things that work(partially):

 - Interrupts
 - Paging (nope)
 - GDT

Things to do in near future:

 - mem management
 - userspace
 - ATA 
 - multiboot


----------
## How to build ##

1. Build GCC ELF cross-compiler

2. Install NASM

3. Add to PATH:

 - cross-compiler's binaries
 - NASM executable

3. Open command prompt in lib folder and type

>  make install

4. Open command prompt in kernel folder and type 

> make all

5. Launch in emulator e.g [QEMU](https://www.qemu.org/).

## Resources ##
- [OsdevWiki](https://www.wiki.osdev.org)
- [Executable and Linkable Format] (http://www.cs.cmu.edu/afs/cs/academic/class/15213-s00/doc/elf.pdf)
