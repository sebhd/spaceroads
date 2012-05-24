/*
 * DeadlyTrackAtom.h
 *
 *  Created on: 04.05.2012
 *      Author: sebastian
 */

#ifndef DEADLYTRACKATOM_H_
#define DEADLYTRACKATOM_H_

#include "SolidTrackAtom.h"

class DeadlyTrackAtom: public SolidTrackAtom {
public:
	DeadlyTrackAtom(game::BoundingBox bbox);
	virtual ~DeadlyTrackAtom();

	virtual void handleCollision(Vehicle* ship,  HitSide hs);
};

#endif /* DEADLYTRACKATOM_H_ */
