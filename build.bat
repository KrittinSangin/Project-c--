@echo off ::Change project_gambling.cpp and project_gambling.exe to your own file name!!!!
g++ project_gambling.cpp -o project_gambling.exe -lsfml-graphics -lsfml-window -lsfml-system
if %errorlevel% neq 0 (
    echo Compilation failed.
) else (
    echo Compilation successful! Running the program...
    project_gambling.exe
)

