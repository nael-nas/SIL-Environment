from flask import Flask, render_template, request, redirect, url_for
import subprocess
import pandas as pd
import matplotlib
matplotlib.use('Agg')  # Use a non-interactive backend to avoid tkinter issues
import matplotlib.pyplot as plt
import os

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/run_simulation', methods=['POST'])
def run_simulation():
    try:
        # Get the simulation time and time step from form data
        sim_time = request.form.get('sim_time')
        time_step = request.form.get('time_step')

        # Run the batch file with provided inputs
        process = subprocess.run(['cmd', '/c', 'run_sil_environment_app_calling.bat', sim_time, time_step], text=True)

        # Check if the batch file executed successfully
        if process.returncode != 0:
            return "Error running C code", 500

        # Plot the results
        df = pd.read_csv('motor_sil_output.csv')

        # Create plots
        plt.figure(figsize=(10, 6))
        plt.plot(df['Time'], df['id'], label='Current id')
        plt.plot(df['Time'], df['iq'], label='Current iq')
        plt.xlabel('Time (s)')
        plt.ylabel('Current (A)')
        plt.title('Motor Currents (id, iq) over Time')
        plt.legend()
        plt.grid(True)
        plt.savefig('static/current_plot.png')
        plt.close()

        plt.figure(figsize=(10, 6))
        plt.plot(df['Time'], df['Te'], label='Torque Te')
        plt.xlabel('Time (s)')
        plt.ylabel('Torque (Nm)')
        plt.title('Electromagnetic Torque over Time')
        plt.legend()
        plt.grid(True)
        plt.savefig('static/torque_plot.png')
        plt.close()

        plt.figure(figsize=(10, 6))
        plt.plot(df['Time'], df['omega_m'], label='Speed omega_m')
        plt.xlabel('Time (s)')
        plt.ylabel('Speed (rad/s)')
        plt.title('Motor Speed over Time')
        plt.legend()
        plt.grid(True)
        plt.savefig('static/speed_plot.png')
        plt.close()

    except KeyError as e:
        return f"Missing form data: {e}", 400
    except Exception as e:
        return f"Error while plotting: {e}", 500

    return render_template('success.html')

@app.route('/stop_simulation', methods=['POST'])
def stop_simulation():
    # Placeholder for stopping the simulation
    return redirect(url_for('index'))

if __name__ == '__main__':
    app.run(debug=True)
