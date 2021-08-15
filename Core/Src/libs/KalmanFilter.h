/*
KalmanFilter.h - Header file for the Kalman Filter
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

#ifndef KalmanFilter_h
#define KalmanFilter_h

extern void Kalman__init( void );

// The angle should be in degrees and the rate should be in degrees per second and the delta time in seconds
extern float getAngle( float newAngle, float newRate, float dt );

extern void  setAngle( float angle ); // Used to set angle, this should be set as the starting angle
extern float getRate(); // Return the unbiased rate

/* These are used to tune the Kalman filter */
extern void setQangle( float Q_angle );
/**
 * setQbias(float Q_bias)
 * Default value (0.003f) is in Kalman.cpp.
 * Raise this to follow input more closely,
 * lower this to smooth result of kalman filter.
 */
extern void setQbias( float Q_bias );
extern void setRmeasure( float R_measure );

extern float getQangle(void);
extern float getQbias(void);
extern float getRmeasure(void);


#endif
