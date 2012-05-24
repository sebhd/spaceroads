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

	virtual void applyEffects(Vehicle* ship, HitSide hs);
	virtual void handleCollision(Vehicle* ship, HitSide hs);
};

#endif /* SOLIDTRACKATOM_H_ */
