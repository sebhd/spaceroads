/*
 * SolidTrackAtom.h
 *
 *  Created on: 04.05.2012
 *      Author: sebastian
 */

#ifndef SOLIDTRACKATOM_H_
#define SOLIDTRACKATOM_H_

#include "TrackAtom.h"

class SolidTrackAtom: public TrackAtom {
public:
	SolidTrackAtom(game::BoundingBox bbox);
	virtual ~SolidTrackAtom();

	virtual void applyContactEffects(Vehicle* ship, HitSide hs);
	virtual void applyCounterForces(Vehicle* ship, HitSide hs);

	float mBounceThreshold;
	float mRebound;

};

#endif /* SOLIDTRACKATOM_H_ */
