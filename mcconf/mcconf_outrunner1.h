/*
	Copyright 2012-2014 Benjamin Vedder	benjamin@vedder.se

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

/*
 * mcconf_outrunner1.h
 *
 * A configuration for my 2kw hobbyking outrunner. Similar size outrunners
 * should have a similar configuration.
 *
 *  Created on: 14 apr 2014
 *      Author: benjamin
 */

#ifndef MCCONF_OUTRUNNER1_H_
#define MCCONF_OUTRUNNER1_H_

/*
 * Parameters
 */
#define MCPWM_CURRENT_MAX				60.0	// Current limit in Amperes (Upper)
#define MCPWM_CURRENT_MIN				-60.0	// Current limit in Amperes (Lower)
#define MCPWM_IN_CURRENT_MAX			60.0	// Input current limit in Amperes (Upper)
#define MCPWM_IN_CURRENT_MIN			-20.0	// Input current limit in Amperes (Lower)

// Sensorless settings
#define MCPWM_IS_SENSORLESS				1		// Use sensorless commutation
#define MCPWM_MIN_RPM					200		// Auto-commutate below this RPM
#define MCPWM_CYCLE_INT_START_RPM_BR	6000.0	// RPM border between the START and LOW interval
#define MCPWM_CYCLE_INT_LIMIT_START		1700.0	// Flux integrator limit 0 ERPM
#define MCPWM_CYCLE_INT_LIMIT_LOW		260.0	// Flux integrator limit MCPWM_CYCLE_INT_START_RPM_BR ERPM
#define MCPWM_CYCLE_INT_LIMIT_HIGH		30.0	// Flux integrator limit 80K ERPM

// Speed PID parameters
#define MCPWM_PID_KP					0.0001	// Proportional gain
#define MCPWM_PID_KI					0.002	// Integral gain
#define MCPWM_PID_KD					0.0		// Derivative gain
#define MCPWM_PID_MIN_RPM				1200.0	// Minimum allowed RPM

// Current control parameters
#define MCPWM_CURRENT_CONTROL_GAIN		0.0016	// Current controller error gain
#define MCPWM_CURRENT_CONTROL_MIN		1.0		// Minimum allowed current

// Hall sensor

#endif /* MCCONF_OUTRUNNER1_H_ */
