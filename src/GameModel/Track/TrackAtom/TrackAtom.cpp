/*
 * TrackAtom.cpp
 *
 *  Created on: 25.04.2012
 *      Author: sebastian
 */


#include "TrackAtom.h"

TrackAtom::TrackAtom(AABB bbox) {

	mBBox = bbox;
	mRenderMaterial = "SolidTrackAtom";
	mName = "Unnamed Track Atom";

	mIsDeadly = false;
	mIsEnergyRefresher = false;
	mIsFinish = false;

	mRotatorAxis = 0;
	mRotatorSteps = 0;

}

TrackAtom::~TrackAtom() {
	// TODO Auto-generated destructor stub

}

void TrackAtom::applyContactEffects(Racer* ship, HitSide hs) {
	if (mIsFinish) {
		ship->mFinish = true;
	}
}

void TrackAtom::applyCounterForces(Racer* ship, HitSide hs) {

}
