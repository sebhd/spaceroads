/*
 * TrackAtom.h
 *
 *  Created on: 25.04.2012
 *      Author: sebastian
 */

#ifndef TRACKATOM_H_
#define TRACKATOM_H_

#include <string>
#include "../util/BoundingBox.h"
#include "../Vehicle.h"

enum HitSide {
	HIT_TOP, HIT_BOTTOM, HIT_LEFT, HIT_RIGHT, HIT_FRONT, HIT_BACK, HIT_NONE
};

class TrackAtom {
public:


	TrackAtom(BoundingBox bbox);
	virtual ~TrackAtom();

	virtual void applyContactEffects(Vehicle* ship, HitSide hs) = 0;
	virtual void applyCounterForces(Vehicle* ship, HitSide hs) = 0;

	BoundingBox mBBox;

	std::string mName;
	std::string mRenderMaterial;
	std::string mRenderMeshName;

	bool mIsDeadly;
};



struct CollisionInfo {
	TrackAtom* ta;
	HitSide hs;
};


#endif /* TRACKATOM_H_ */
