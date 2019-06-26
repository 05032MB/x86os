@echo off
IF [%1] == [] (echo Usage: %1 file_to_dissassemble [target_file]. & EXIT /B )
IF [%2] == [] (objdump -M intel-mnemonic -s -S -d objs/%1) ELSE (objdump -M intel-mnemonic -s -S -d objs/%1 > %2.txt)