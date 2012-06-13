/*
 * SolidTrackAtom.h
 *
 *  Created on: 04.05.2012
 *      Author: sebastian
 */

#ifndef SOLIDTRACKATOM_H_
#define SOLIDTRACKATOM_H_

#include "TrackAtom.h"

class SolidTrackAtom : public TrackAtom {
public:
	SolidTrackAtom(BoundingBox bbox);
	virtual ~SolidTrackAtom();

	virtual void applyContactEffects(Racer* ship, HitSide hs);
	virtual void applyCounterForces(Racer* ship, HitSide hs);

	float mBounceThreshold;
	float mRebound;
	float mJumpForce;
	float mSlipOffset;

};

#endif /* SOLIDTRACKATOM_H_ */
