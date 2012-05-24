/*
 * SolidTrackAtom.cpp
 *
 *  Created on: 04.05.2012
 *      Author: sebastian
 */

#include "SolidTrackAtom.h"

SolidTrackAtom::SolidTrackAtom(game::BoundingBox bbox) :
		TrackAtom(bbox) {

	mMaterial = "SolidTrackAtom";
}

SolidTrackAtom::~SolidTrackAtom() {
	// TODO Auto-generated destructor stub
}

void SolidTrackAtom::applyContactEffects(Vehicle* ship, HitSide hs) {

	//############### BEGIN Propulsion ###############
	// Apply sidewards friction:
	ship->mVelocity -= cml::dot(ship->mVecLeft, ship->mVelocity) * ship->mVecLeft * 0.03;

	// Sidewards speed limit:
	if ((cml::dot(ship->mVecLeft, ship->mVelocity) * ship->mVecLeft).length() < 0.1) {

		if (ship->mMoveLeft) {
			ship->mVelocity += ship->mVecLeft * ship->mAccelLeftRight;
		}

		if (ship->mMoveRight) {
			ship->mVelocity -= ship->mVecLeft * ship->mAccelLeftRight;
		}
	}
	//############### END Propulsion ###############

	// ################# BEGIN Bouncing ################
	cml::vector3f u = cml::normalize(ship->getGravity());
	cml::vector3f bounceVelocity = (cml::dot(u, ship->mVelocity) * u) * 1.5;

	if (!ship->mTryJump && bounceVelocity.length() > 0.5) {
		ship->mVelocity -= bounceVelocity;
	}
	// ################# END Bouncing ################


	// ############## BEGIN Jumping ###############
	if (ship->mTryJump) {
		cml::vector3f gravNormalized = cml::normalize(ship->getGravity());

		// NOTE: The factor that gravNormalized needs to be multiplied with depends
		// on the size/dimensions of the ship's bounding box!

		// TODO 2: Do this check with a 'ray intersects plane' test instead of the 'point is in box' test
		// This is better because it is independent of the size of the bounding boxes of vehicle & track atom.
		cml::vector3f tmp = ship->mPos + gravNormalized * 2.2;

		if (mBBox.containsPoint(tmp)) {
			ship->mVelocity -= gravNormalized * 0.7;
		}
	}
	// ############## END Jumping ###############
}

void SolidTrackAtom::applyCounterForces(Vehicle* ship, HitSide hs) {

	switch (hs) {
	case HIT_TOP:
		ship->mVelocity[1] = 0;

		break;

	case HIT_BOTTOM:
		ship->mVelocity[1] = 0;

		break;

	case HIT_FRONT:
		ship->mVelocity[2] = 0;

		break;
	case HIT_BACK:
		ship->mVelocity[2] = 0;

		break;

	case HIT_RIGHT:
		ship->mVelocity[0] = 0;

		break;

	case HIT_LEFT:
		ship->mVelocity[0] = 0;

		break;

	default:
		break;
	}
}
