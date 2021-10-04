/*
 * pid.h
 *
 *  Created on: 2 paź 2021
 *      Author: simon
 */

#ifndef SRC_LIBS_PID_H_
#define SRC_LIBS_PID_H_

/* Controller parameters */
#define PID_KP			2.0f
#define PID_KI			0.5f
#define PID_KD 			0.25f

#define PID_TAU			0.02f

#define PID_LIM_MIN 	-10.0f
#define PID_LIM_MAX  	10.0f

#define PID_LIM_MIN_INT -5.0f
#define PID_LIM_MAX_INT  5.0f

#define SAMPLE_TIME_S	 0.01f

/* Maximum run-time of simulation */
#define SIMULATION_TIME_MAX 4.0f



typedef struct {
	/* Controller gains */
	float Kp;
	float Ki;
	float Kd;

	/* Derivative low-pass filter time constant */
	float tau;

	/* Output limits */
	float limMin;
	float limMax;

	/* Integrator limits */
	float limMinInt;
	float limMaxInt;

	/* Sample time (in seconds) */
	float T;

	/* Controller "memory" */
	float integrator;
	float prevError;			/* Required for integrator */
	float differentiator;
	float prevMeasurement;		/* Required for differentiators */

	/* Controller output */
	float out;

} PIDController;

void  PIDController_Init  ( PIDController *pid );
float PIDController_Update( PIDController *pid, float setpoint, float measurement );

#endif /* SRC_LIBS_PID_H_ */
