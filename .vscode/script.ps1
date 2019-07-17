# Remove *.o files
Write-Host 'remove *.o files' -ForegroundColor Cyan
Get-ChildItem * -Include *.o -Recurse | Remove-Item

# cd to ./lib
cd ./lib
Get-Location | Write-Host -ForegroundColor Cyan

# make install
Write-Host 'make install' -BackgroundColor Cyan -ForegroundColor Black
make install | Foreach-Object {$_ -replace "^", "`t"} | Write-Host -ForegroundColor White
if ($LastExitCode)
{
    Write-Host "lib make install failed" -ForegroundColor Red
    Exit
}
else
{
    Write-Host "lib make install - ok" -ForegroundColor Green
}

# cd to ../kernel
cd ../kernel
Get-Location | Write-Host -ForegroundColor Cyan

#make all
Write-Host 'make all' -BackgroundColor Cyan -ForegroundColor Black
make all | Foreach-Object {$_ -replace "^", "`t"} | Write-Host -ForegroundColor White
if ($LastExitCode)
{
    Write-Host "kernel make all failed" -ForegroundColor Red
    Exit
}
else
{
    Write-Host "kernel make all - ok" -ForegroundColor Green
}

#make
Write-Host 'make' -BackgroundColor Cyan -ForegroundColor Black
make | Foreach-Object {$_ -replace "^", "`t"} | Write-Host -ForegroundColor White
if ($LastExitCode)
{
    Write-Host "kernel make failed" -ForegroundColor Red
    Exit
}
else
{
    Write-Host "kernel make - ok" -ForegroundColor Green
}

# cp os.bin
Write-Host 'copy os.bin to utility/isogen/iso/boot' -ForegroundColor Cyan
Copy-Item 'os.bin' -Destination '../utility/isogen/iso/boot/'

# cd utility/isogen
cd ../utility/isogen
Get-Location | Write-Host -ForegroundColor Cyan

#mkisofs
Write-Host 'mkisofs' -BackgroundColor Cyan -ForegroundColor Black
mkisofs -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o bootable.iso ./iso | Write-Host -ForegroundColor White

#qemu
Write-Host 'qemu-system-i386' -BackgroundColor Cyan -ForegroundColor Black
qemu-system-i386 -m 256M -cdrom bootable.iso 2> qemu_error.log.txt
