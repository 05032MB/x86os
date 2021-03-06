# x86os

x86 operating system.
For Intel 32-bit machine.

Things that work 

 - multiboot-compliancy
 - Interrupts & SystemCalls
 - GDT
 - VGA
 - Heap ?

Things that need polishing

- Paging
- Userland
- Tasks
- Filesystems

Things to do in near future:

 - mem management fix/new
 - ATA driver


----------

## Install dependencies

1. Build GNU GCC i686-elf cross-compiler or get precompiled binaries (e.g. from [here](https://github.com/lordmilko/i686-elf-tools)).

2. Install GNU Make.

3. Install NASM ([official website](https://www.nasm.us/)).

4. Install emulator of your choice (e.g [QEMU](https://www.qemu.org/)).

5. Add binaries to `PATH`:

   - cross-compiler's binaries
   - Make binaries
   - NASM binaries
   - QEMU binaries
   
6. Install mkisofs 2.+ (to build bootable iso), into *utility/isogen*
## Check dependencies

Try following commands:

```bat
i686-elf-g++ --version
make --version
nasm --version
qemu-system-i386 --version
mkisofs --version
```

(Each tool should print short info about its version.)

## Build and launch x86os

1. Open command prompt.

2. Navigate to `lib` folder and type:

   ```bat
   make install
   ```

3. Navigate to  `kernel` folder and type:

   ```bat
   make all
   deploy.bat
   ```

3. Navigate to `utility/isogen` folder and type:

   ```bat
   qemu_launch.bat
   ```

## Dive into code!

**Hint:** C++ entry point is `kernel_main`.

## Resources
- [OSDev Wiki](https://wiki.osdev.org/Expanded_Main_Page)
- [Executable and Linkable Format](http://www.cs.cmu.edu/afs/cs/academic/class/15213-s00/doc/elf.pdf)
