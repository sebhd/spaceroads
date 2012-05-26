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
}

SolidTrackAtom::~SolidTrackAtom() {
	// TODO Auto-generated destructor stub
}

void SolidTrackAtom::applyContactEffects(Vehicle* ship, HitSide hs) {

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

	/*
	// ################# BEGIN Bouncing ################
	cml::vector3f u = cml::normalize(ship->getGravity());
	cml::vector3f bounceVelocity = (cml::dot(u, ship->mVelocity) * u) * 1.5;


	// NOTE: The value on the right side of the following inequation is the 'bouncyness factor'.
	// The smaller it is, the longer the vehicle will bounce:
	if (!ship->mTryJump && bounceVelocity.length() > 0.7) {
		ship->mVelocity -= bounceVelocity;
	}
	// ################# END Bouncing ################
*/

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

	// #################### BEGIN Determine wall normal vector #################
	cml::vector3f wallNormal;

	switch (hs) {
	case HIT_TOP:
		wallNormal.set(0,1,0);
		break;

	case HIT_BOTTOM:
		wallNormal.set(0,-1,0);
		break;

	case HIT_FRONT:
		wallNormal.set(0,0,1);
		break;
	case HIT_BACK:
		wallNormal.set(0,0,-1);
		break;

	case HIT_RIGHT:
		wallNormal.set(1,0,0);
		break;

	case HIT_LEFT:
		wallNormal.set(-1,0,0);
		break;

	default:
		return;
	}
	// #################### END Determine wall normal vector #################


	// #################### BEGIN Stop or bounce #################
	float bounceThreshold = 0.3;
	float rebound = 1.5;

	cml::vector3f hitComponent = cml::dot(wallNormal, ship->mVelocity) * wallNormal;

	if (hitComponent.length() > bounceThreshold) {
		ship->mVelocity -= hitComponent * rebound;
	}
	else {
		ship->mVelocity -= hitComponent;
	}
	// #################### END Stop or bounce #################
}
