@echo off
g++ project_gambling.cpp -o project_gambling.exe -lsfml-graphics -lsfml-window -lsfml-system
if %errorlevel% neq 0 (
    echo Compilation failed.
) else (
    echo Compilation successful! Running the program...
    project_gambling.exe
)

