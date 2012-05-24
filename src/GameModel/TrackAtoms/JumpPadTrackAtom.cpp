/*
 * JumpPadTrackAtom.cpp
 *
 *  Created on: 04.05.2012
 *      Author: sebastian
 */

#include "JumpPadTrackAtom.h"

JumpPadTrackAtom::JumpPadTrackAtom(game::BoundingBox bbox) : SolidTrackAtom(bbox) {

	mJumpVelocity = 0.4;
	mMaterial = "JumpPadTrackAtom";
}

JumpPadTrackAtom::~JumpPadTrackAtom() {
	// TODO Auto-generated destructor stub
}


void JumpPadTrackAtom::applyContactEffects(Vehicle* ship,  HitSide hs) {

	if (hs == HIT_TOP) {
		ship->mVelocity[1] = mJumpVelocity;
	}
}
