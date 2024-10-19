@echo off
SETLOCAL

REM Accept parameters from Python script
set SIMULATION_TIME=%1
set TIME_STEP=%2

REM Compile the C source file
echo Compiling C source file...
gcc -o csv_motor_sil_test csv_motor_sil_test.c -lm
if %ERRORLEVEL% NEQ 0 (
    echo Compilation failed.
    exit /b 1
)

REM Run the SIL motor simulation with the provided arguments
echo Running the SIL motor simulation...
csv_motor_sil_test %SIMULATION_TIME% %TIME_STEP%
if %ERRORLEVEL% NEQ 0 (
    echo Error running C code
    exit /b 1
)

echo Simulation completed. Results saved to motor_sil_output.csv
