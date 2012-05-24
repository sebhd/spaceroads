/*
 * DeadlyTrackAtom.cpp
 *
 *  Created on: 04.05.2012
 *      Author: sebastian
 */

#include "DeadlyTrackAtom.h"

DeadlyTrackAtom::DeadlyTrackAtom(game::BoundingBox bbox) : SolidTrackAtom(bbox) {
	// TODO Auto-generated constructor stub

	mMaterial = "DeadlyTrackAtom";
}

DeadlyTrackAtom::~DeadlyTrackAtom() {
	// TODO Auto-generated destructor stub
}


void DeadlyTrackAtom::applyCounterForces(Vehicle* ship,  HitSide hs) {
	SolidTrackAtom::applyCounterForces(ship, hs);

	ship->mKilled = true;
}
