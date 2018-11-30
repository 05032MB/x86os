#qemu-system-x86_64 -kernel os.bin
#copy /b os.bin os.img
qemu-system-i386 -s -d guest_errors,int -kernel os.bin > currentsession.log.txt 2>&1
exit
PAUSE