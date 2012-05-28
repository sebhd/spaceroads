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
	meshName = "";

	mBounceThreshold = 0.5;
	mRebound = 0.5;

}

SolidTrackAtom::~SolidTrackAtom() {
	// TODO Auto-generated destructor stub
}

void SolidTrackAtom::applyContactEffects(Vehicle* ship, HitSide hs) {

	if (mIsDeadly) {
		ship->mKilled = true;
	}

	//############### BEGIN Propulsion ###############
	// Apply sidewards friction:
	ship->mVelocity -= cml::dot(ship->mDirLeft, ship->mVelocity) * ship->mDirLeft * 0.03;

	// Sidewards speed limit:
	if ((cml::dot(ship->mDirLeft, ship->mVelocity) * ship->mDirLeft).length() < 0.1) {

		if (ship->mMoveLeft) {
			ship->mVelocity += ship->mDirLeft * ship->mAccelLeftRight;
		}

		if (ship->mMoveRight) {
			ship->mVelocity -= ship->mDirLeft * ship->mAccelLeftRight;
		}
	}
	//############### END Propulsion ###############


	// #################### BEGIN Determine wall normal vector #################
	cml::vector3f wallNormal;

	switch (hs) {
	case HIT_TOP:
		wallNormal.set(0, 1, 0);
		break;

	case HIT_BOTTOM:
		wallNormal.set(0, -1, 0);
		break;

	case HIT_FRONT:
		wallNormal.set(0, 0, 1);
		break;
	case HIT_BACK:
		wallNormal.set(0, 0, -1);
		break;

	case HIT_RIGHT:
		wallNormal.set(1, 0, 0);
		break;

	case HIT_LEFT:
		wallNormal.set(-1, 0, 0);
		break;

	default:
		return;
	}
	// #################### END Determine wall normal vector #################


	// ############## BEGIN Jumping ###############
	if (ship->mTryJump) {
		cml::vector3f gravNormalized = cml::normalize(ship->getGravity());

		// NOTE: The factor that gravNormalized needs to be multiplied with depends
		// on the size/dimensions of the ship's bounding box!

		// TODO 2: Do this check with a 'ray intersects plane' test instead of the 'point is in box' test
		// This is better because it is independent of the size of the bounding boxes of vehicle & track atom.
		cml::vector3f tmp = ship->mPos + gravNormalized * 2.7;

		if (mBBox.containsPoint(tmp)) {
			ship->mVelocity -= gravNormalized * 0.7;
		}
	}
	// ############## END Jumping ###############

}

void SolidTrackAtom::applyCounterForces(Vehicle* ship, HitSide hs) {

	// #################### BEGIN Determine wall normal vector #################
	cml::vector3f wallNormal;

	switch (hs) {
	case HIT_TOP:
		wallNormal.set(0, 1, 0);
		break;

	case HIT_BOTTOM:
		wallNormal.set(0, -1, 0);
		break;

	case HIT_FRONT:
		wallNormal.set(0, 0, 1);
		break;
	case HIT_BACK:
		wallNormal.set(0, 0, -1);
		break;

	case HIT_RIGHT:
		wallNormal.set(1, 0, 0);
		break;

	case HIT_LEFT:
		wallNormal.set(-1, 0, 0);
		break;

	default:
		return;
	}
	// #################### END Determine wall normal vector #################

	float dot = cml::dot(ship->mVelocity, wallNormal);
	cml::vector3f hitComponent = dot * wallNormal;

	ship->mVelocity -= hitComponent;

	if (hitComponent.length() > mBounceThreshold) {

		if (mRebound < 0) {
			ship->mVelocity -= wallNormal * mRebound;
		} else {
			ship->mVelocity -= hitComponent * mRebound;
		}
	}


	//################ BEGIN Kill vehicle if it hits something with the nose too fast ###############
	dot = cml::dot(ship->mDirForward, wallNormal);

	if (dot < -0.8 && hitComponent.length() > 0.5) {
		ship->mKilled = true;
	}
	//################ END Kill vehicle if it hits something with the nose too fast ###############

}
