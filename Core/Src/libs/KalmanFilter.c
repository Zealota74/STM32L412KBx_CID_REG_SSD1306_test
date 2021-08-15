/*
 KalmanFilter.cpp - Class file for the Kalman Filter
 Version: 1.0.0
 (c) 2014 Korneliusz Jarzebski
 www.jarzebski.pl
 This program is free software: you can redistribute it and/or modify
 it under the terms of the version 3 GNU General Public License as
 published by the Free Software Foundation.
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "KalmanFilter.h"

static float Q_angle; // Process noise variance for the accelerometer
static float Q_bias; // Process noise variance for the gyro bias
static float R_measure; // Measurement noise variance - this is actually the variance of the measurement noise

static float angle; // The angle calculated by the Kalman filter - part of the 2x1 state vector
static float bias; // The gyro bias calculated by the Kalman filter - part of the 2x1 state vector
static float rate; // Unbiased rate calculated from the rate and the calculated bias - you have to call getAngle to update the rate

static float P[2][2]; // Error covariance matrix - This is a 2x2 matrix

struct Vector {
	float angle;
	float bias;
	float rate;
	float Q_angle;
	float Q_bias;
	float R_measure;
};
static struct Vector this;

void Kalman__init(void) {
    /* We will set the variables like so, these can also be tuned by the user */
    Q_angle = 0.001f;
    Q_bias = 0.003f;
    R_measure = 0.03f;

    angle = 0.0f; // Reset the angle
    bias = 0.0f; // Reset bias

    P[0][0] = 0.0f; // Since we assume that the bias is 0 and we know the starting angle (use setAngle), the error covariance matrix is set like so - see: http://en.wikipedia.org/wiki/Kalman_filter#Example_application.2C_technical
    P[0][1] = 0.0f;
    P[1][0] = 0.0f;
    P[1][1] = 0.0f;
};

// The angle should be in degrees and the rate should be in degrees per second and the delta time in seconds
float Kalman__getAngle( float newAngle, float newRate, float dt) {
    // KasBot V2  -  Kalman filter module - http://www.x-firm.com/?page_id=145
    // Modified by Kristian Lauszus
    // See my blog post for more information: http://blog.tkjelectronics.dk/2012/09/a-practical-approach-to-kalman-filter-and-how-to-implement-it

    // Discrete Kalman filter time update equations - Time Update ("Predict")
    // Update xhat - Project the state ahead
    /* Step 1 */
    rate 	= newRate - bias;
    angle  += dt * rate;

    // Update estimation error covariance - Project the error covariance ahead
    /* Step 2 */
    P[0][0] += dt * (dt*P[1][1] - P[0][1] - P[1][0] + Q_angle);
    P[0][1] -= dt * P[1][1];
    P[1][0] -= dt * P[1][1];
    P[1][1] += Q_bias * dt;

    // Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
    // Calculate Kalman gain - Compute the Kalman gain
    /* Step 4 */
    float S = P[0][0] + R_measure; // Estimate error
    /* Step 5 */
    float K[2]; // Kalman gain - This is a 2x1 vector
    K[0] = P[0][0] / S;
    K[1] = P[1][0] / S;

    // Calculate angle and bias - Update estimate with measurement zk (newAngle)
    /* Step 3 */
    float y = newAngle - angle; // Angle difference
    /* Step 6 */
    angle += K[0] * y;
    bias += K[1] * y;

    // Calculate estimation error covariance - Update the error covariance
    /* Step 7 */
    float P00_temp = P[0][0];
    float P01_temp = P[0][1];

    P[0][0] -= K[0] * P00_temp;
    P[0][1] -= K[0] * P01_temp;
    P[1][0] -= K[1] * P00_temp;
    P[1][1] -= K[1] * P01_temp;

    return angle;
};

void  Kalman__setAngle( float angle ) { this.angle = angle; }; 	// Used to set angle, this should be set as the starting angle
float Kalman__getRate() { return this.rate; }; 				// Return the unbiased rate

/* These are used to tune the Kalman filter */
void  Kalman__setQangle( float Q_angle ) { this.Q_angle = Q_angle; };
float Kalman__getQangle() { return this.Q_angle; };

void  Kalman__setQbias( float Q_bias ) { this.Q_bias = Q_bias; };
float Kalman__getQbias() { return this.Q_bias; };

void  Kalman__setRmeasure( float R_measure ) { this.R_measure = R_measure; };
float Kalman__getRmeasure() { return this.R_measure; };

