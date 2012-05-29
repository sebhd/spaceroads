/*
 * GravChangerTrackAtom.h
 *
 *  Created on: 28.05.2012
 *      Author: sebastian
 */

#ifndef GRAVCHANGERTRACKATOM_H_
#define GRAVCHANGERTRACKATOM_H_

#include "SolidTrackAtom.h"

class GravChangerTrackAtom: public SolidTrackAtom {
public:
	GravChangerTrackAtom(game::BoundingBox bbox);
	virtual ~GravChangerTrackAtom();

	virtual void applyContactEffects(Vehicle* ship, HitSide hs);

	quat mNewOrientation;
};

#endif /* GRAVCHANGERTRACKATOM_H_ */
