@echo off
set /p sim_time="Enter simulation time (seconds): "
set /p time_step="Enter time step (seconds): "

echo Compiling C source file...
gcc -o csv_motor_sil_test csv_motor_sil_test.c -lm

if %errorlevel% neq 0 (
    echo Compilation failed. Exiting.
    exit /b 1
)

echo Running the SIL motor simulation...
csv_motor_sil_test %sim_time% %time_step%

if %errorlevel% neq 0 (
    echo Error running C code
    exit /b 1
)

echo Simulation completed. Results saved to motor_sil_output.csv
echo Plotting results...
python plot_motor_simulation_v2.py

if %errorlevel% neq 0 (
    echo Error while plotting
    exit /b 1
)

echo SIL environment process completed successfully.
pause