Integrate the graphs in the Website, Start and Stop the Simulation:

PS E:\PythonTraining\C-Code\SIL-Environment_v1> python app.py
OR
PS E:\PythonTraining\C-Code\SIL-Environment_v1> .\run_sil_environment_app.bat

SIL_Dependancy_Graphics.DOT can be used to plot the file dependancies in the project folder using online service!!!

In the provided app.py file, the graphs are generated directly within the Flask application without relying on plot_motor_simulation_v2.py. Here's how this works:

Graph Plotting Inside app.py:

The plotting logic is implemented directly in the run_simulation() function of the app.py file.
After running the batch file (run_sil_environment_app_calling.bat) to execute the C code and generate the output (motor_sil_output.csv), the CSV file is read using pandas (df = pd.read_csv('motor_sil_output.csv')).
The graphs are created with matplotlib, and the output images (current_plot.png, torque_plot.png, speed_plot.png) are saved in the static directory using plt.savefig().
These saved images are then rendered in the HTML template (success.html) to display the graphs on the website.
Why plot_motor_simulation_v2.py Is Not Called:

In the current implementation of app.py, the visualization step is entirely handled inline by the Flask code. Therefore, there is no need to call the external Python script (plot_motor_simulation_v2.py).
The plot_motor_simulation_v2.py script is a standalone file that can be used to generate plots from the CSV file independently, but its logic has essentially been incorporated into app.py.