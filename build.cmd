@echo off
REM CMD build script

REM Initialize MSVC environment for x86
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x86

REM Compile the C++ program from the cpp_src directory and output to the build directory
cl /EHsc cpp_src\main.cpp cpp_src\manager.cpp cpp_src\algorithms.cpp cpp_src\threads.cpp cpp_src\io.cpp cpp_src\measurements.cpp cpp_src\input.cpp cpp_src\output.cpp /Febuild\program.exe

REM Remove the intermediate object files
del *.obj
del cpp_src\*.obj
