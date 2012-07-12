/*
 * GravChangerTrackAtom.h
 *
 *  Created on: 28.05.2012
 *      Author: sebastian
 */

#ifndef GRAVCHANGERTRACKATOM_H_
#define GRAVCHANGERTRACKATOM_H_

#include "SolidTrackAtom.h"

class OrientationChangerTrackAtom: public SolidTrackAtom {
public:
	OrientationChangerTrackAtom(AABB bbox);
	virtual ~OrientationChangerTrackAtom();

	virtual void applyContactEffects(Racer* ship, HitSide hs, unsigned int dt);

	quat mNewOrientation;
};

#endif /* GRAVCHANGERTRACKATOM_H_ */
