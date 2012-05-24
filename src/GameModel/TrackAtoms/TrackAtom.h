/*
 * TrackAtom.h
 *
 *  Created on: 25.04.2012
 *      Author: sebastian
 */

#ifndef TRACKATOM_H_
#define TRACKATOM_H_

#include <string>
#include "../BoundingBox.h"
#include "../Vehicle.h"
//#include "../CollisionInfo.h"

class Vehicle;


class TrackAtom {
public:

	enum HitSide { HIT_TOP, HIT_BOTTOM, HIT_LEFT, HIT_RIGHT, HIT_FRONT, HIT_BACK, HIT_NONE };

	TrackAtom(game::BoundingBox bbox);
	virtual ~TrackAtom();

	game::BoundingBox mBBox;

	std::string mMaterial;

	std::string name;

	virtual void applyCounterForces(Vehicle* ship, HitSide hs) = 0;

	virtual void applyContactEffects(Vehicle* ship, HitSide hs) = 0;
};

#endif /* TRACKATOM_H_ */
