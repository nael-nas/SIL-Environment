#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265359

// Motor parameters
const double Rs = 0.1;  // Stator resistance (Ohms)
const double Ld = 0.005;  // d-axis inductance (Henrys)
const double Lq = 0.006;  // q-axis inductance (Henrys)
const double lambda_f = 0.1;  // Flux linkage (Wb)
const double P = 4;  // Number of pole pairs
const double J = 0.01;  // Inertia (kg.m^2)

// Control parameters
double id_ref = 0;  // d-axis current reference (Amps)
double iq_ref = 10;  // q-axis current reference (Amps)
double Kp_id = 1.0, Ki_id = 50.0;
double Kp_iq = 1.0, Ki_iq = 50.0;

// State variables
double id = 0, iq = 0;  // d-q axis currents
double vd = 0, vq = 0;  // d-q axis voltages
double theta_e = 0;  // Electrical angle
double omega_m = 0, omega_e = 0;  // Mechanical and electrical speed
double Te = 0;  // Electromagnetic torque

// PI controller integrals
double id_err_int = 0, iq_err_int = 0;

// Mechanical torque and speed update
void motor_mechanics(double Te, double Tl, double *omega_m, double time_step) {
    double domega_m = (Te - Tl) / J;
    *omega_m += domega_m * time_step;
    omega_e = *omega_m * P;  // Convert mechanical speed to electrical speed
}

// FOC Control Algorithm
void foc_control(double id_ref, double iq_ref, double id, double iq, double theta_e, double time_step) {
    // PI controllers for id and iq
    double id_err = id_ref - id;
    id_err_int += id_err * time_step;
    vd = Kp_id * id_err + Ki_id * id_err_int;

    double iq_err = iq_ref - iq;
    iq_err_int += iq_err * time_step;
    vq = Kp_iq * iq_err + Ki_iq * iq_err_int;

    // Calculate electromagnetic torque
    Te = (3.0 / 2.0) * P * (lambda_f * iq + (Ld - Lq) * id * iq);
}

// Motor electrical dynamics update
void motor_dynamics(double vd, double vq, double *id, double *iq, double time_step) {
    double did = (vd - Rs * (*id) + omega_e * Lq * (*iq)) / Ld;
    double diq = (vq - Rs * (*iq) - omega_e * (Ld * (*id) + lambda_f)) / Lq;

    *id += did * time_step;
    *iq += diq * time_step;
}

// Main simulation loop
int main(int argc, char *argv[]) {
    // Check for correct number of arguments
    if (argc != 3) {
        printf("Usage: %s <simulation_time> <time_step>\n", argv[0]);
        return 1;
    }

    // Read simulation time and time step from command-line arguments
    double simulation_time = atof(argv[1]);
    double time_step = atof(argv[2]);

    printf("Simulation Time: %.3f seconds\n", simulation_time);
    printf("Time Step: %.3f seconds\n", time_step);

    double time = 0;
    double Tl = 0.5;  // Load torque

    FILE *fp = fopen("motor_sil_output.csv", "w");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return 1;
    }
    fprintf(fp, "Time,id,iq,Te,omega_m\n");

    while (time < simulation_time) {
        // FOC control
        foc_control(id_ref, iq_ref, id, iq, theta_e, time_step);

        // Motor dynamics
        motor_dynamics(vd, vq, &id, &iq, time_step);

        // Mechanical dynamics
        motor_mechanics(Te, Tl, &omega_m, time_step);

        // Update electrical angle
        theta_e += omega_e * time_step;
        if (theta_e > 2 * PI) theta_e -= 2 * PI;

        // Write simulation results to CSV file
        fprintf(fp, "%.3f,%.3f,%.3f,%.3f,%.3f\n", time, id, iq, Te, omega_m);

        time += time_step;
    }

    fclose(fp);
    printf("Simulation completed. Results saved to motor_sil_output.csv\n");

    return 0;
}