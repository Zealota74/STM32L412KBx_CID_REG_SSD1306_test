/*
 * pid.c
 *
 *  Created on: 2 paź 2021
 *      Author: simon
 */

#include <stdio.h>
#include <stdlib.h>

#include "pid.h"

/* Initialize PID controller */
static PIDController pid = {
	PID_KP,			PID_KI,			PID_KD,
	PID_TAU,		PID_LIM_MIN,	PID_LIM_MAX,
	PID_LIM_MIN_INT,PID_LIM_MAX_INT,SAMPLE_TIME_S
};

void PIDController_Init( PIDController *pid ) {
	/* Clear controller variables */
	pid->integrator = 0.0f;
	pid->prevError  = 0.0f;

	pid->differentiator  = 0.0f;
	pid->prevMeasurement = 0.0f;

	pid->out = 0.0f;
}

float PIDController_Update( PIDController *pid, float setpoint, float measurement ) {
	/*
	 * Error signal
	 */
	float error = setpoint - measurement;

	/*
	 * Proportional
	 */
	float proportional = pid->Kp * error;

	/*
	 * Integral
	 */
	pid->integrator = pid->integrator + 0.5f * pid->Ki * pid->T * (error + pid->prevError);

	/* Anti-wind-up via integrator clamping */
	if (pid->integrator > pid->limMaxInt) {
		pid->integrator = pid->limMaxInt;
	} else if (pid->integrator < pid->limMinInt) {
		pid->integrator = pid->limMinInt;
	}

	/*
	 * Derivative (band-limited differentiator)
	 */

	pid->differentiator = -(2.0f * pid->Kd * ( measurement - pid->prevMeasurement ) /* Note: derivative on measurement, therefore minus sign in front of equation! */
	+ (2.0f * pid->tau - pid->T) * pid->differentiator) / (2.0f * pid->tau + pid->T);

	/*
	 * Compute output and apply limits
	 */
	pid->out = proportional + pid->integrator + pid->differentiator;

	if (pid->out > pid->limMax) {
		pid->out = pid->limMax;
	} else if (pid->out < pid->limMin) {
		pid->out = pid->limMin;
	}

	/* Store error and measurement for later use */
	pid->prevError = error;
	pid->prevMeasurement = measurement;

	/* Return controller output */
	return pid->out;
}




/* Simulated dynamical system (first order) */
float TestSystem_Update( float inp ) {
	static float 		output 	= 0.0f;
	static const float	alpha 	= 0.02f;

	output = ( SAMPLE_TIME_S * inp + output ) / ( 1.0f + alpha * SAMPLE_TIME_S );

	return output;
}

int pid_test( void ) {
	PIDController_Init( &pid );

	/* Simulate response using test system */
	float setpoint = 1.0f;

//    printf("Time (s)\tSystem Output\tControllerOutput\r\n");

	for ( float t = 0.0f; t <= SIMULATION_TIME_MAX; t += SAMPLE_TIME_S ) {
		/* Get measurement from system */
		float measurement = TestSystem_Update(pid.out);
		/* Compute new control signal */
		PIDController_Update(&pid, setpoint, measurement);
//        printf("%f\t%f\t%f\r\n", t, measurement, pid.out);
	}
	return 0;
}

