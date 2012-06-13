/*
 * Pilot.h
 *
 *  Created on: 13.06.2012
 *      Author: sebastian
 */

#ifndef PILOT_H_
#define PILOT_H_

#include "../Vehicle.h"

class Pilot {
public:
	Pilot();
	virtual ~Pilot();

	Vehicle* mVehicle;

	virtual void step() = 0;

private:

};

#endif /* PILOT_H_ */
