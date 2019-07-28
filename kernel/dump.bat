@echo off
IF [%1] == [] (echo Usage: %1 kernel_obj_file_to_del [target_file.o/.oa]. & EXIT /B )

DEL /Q OBJS\%1.o 2> nul
IF errorlevel 0 (DEL /Q OBJS\%1.oa 2> nul)
ECHO OK