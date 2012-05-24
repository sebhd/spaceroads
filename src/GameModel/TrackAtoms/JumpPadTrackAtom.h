/*
 * JumpPadTrackAtom.h
 *
 *  Created on: 04.05.2012
 *      Author: sebastian
 */

#ifndef JUMPPADTRACKATOM_H_
#define JUMPPADTRACKATOM_H_

#include "SolidTrackAtom.h"

class JumpPadTrackAtom: public SolidTrackAtom {
public:
	JumpPadTrackAtom(game::BoundingBox bbox);
	virtual ~JumpPadTrackAtom();

	virtual void applyEffects(Vehicle* ship, HitSide hs);
	//virtual void handleCollision(Vehicle* ship,  HitSide hs);

	float mJumpVelocity;
};

#endif /* JUMPPADTRACKATOM_H_ */
