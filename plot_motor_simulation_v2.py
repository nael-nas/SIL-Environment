import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV data
data = pd.read_csv('motor_sil_output.csv')

# Extract the data columns
time = data['Time']
id_current = data['id']
iq_current = data['iq']
torque = data['Te']
speed = data['omega_m']

# Plot the d-axis and q-axis currents
plt.figure(figsize=(10, 6))
plt.subplot(3, 1, 1)
plt.plot(time, id_current, label='i_d Current (A)')
plt.plot(time, iq_current, label='i_q Current (A)')
plt.xlabel('Time (s)')
plt.ylabel('Current (A)')
plt.title('Motor Currents')
plt.legend()
plt.grid()

# Plot the electromagnetic torque
plt.subplot(3, 1, 2)
plt.plot(time, torque, label='Electromagnetic Torque (Nm)')
plt.xlabel('Time (s)')
plt.ylabel('Torque (Nm)')
plt.title('Electromagnetic Torque')
plt.legend()
plt.grid()

# Plot the mechanical speed
plt.subplot(3, 1, 3)
plt.plot(time, speed, label='Mechanical Speed (rad/s)')
plt.xlabel('Time (s)')
plt.ylabel('Speed (rad/s)')
plt.title('Mechanical Speed')
plt.legend()
plt.grid()

# Adjust layout to prevent overlapping
plt.tight_layout()

# Show the plot
plt.show()
