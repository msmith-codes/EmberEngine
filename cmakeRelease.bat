@echo off

cmake . -G "Ninja" -DCMAKE_CXX_COMPILER="g++.exe" -DCMAKE_C_COMPILER="gcc.exe" -DCMAKE_BUILD_TYPE="Release"
pause

