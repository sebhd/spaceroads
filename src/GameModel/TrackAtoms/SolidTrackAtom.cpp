/*
 * SolidTrackAtom.cpp
 *
 *  Created on: 04.05.2012
 *      Author: sebastian
 */

#include "SolidTrackAtom.h"

SolidTrackAtom::SolidTrackAtom(game::BoundingBox bbox) :
		TrackAtom(bbox) {

	mRenderMaterial = "SpaceRoads/Track/Solid";
	mRenderMeshName = "";

	mBounceThreshold = 0.3;
	mRebound = 0.3;
	mJumpForce = 0.7;
	mSlipOffset = 0.01;
}

SolidTrackAtom::~SolidTrackAtom() {
	// TODO Auto-generated destructor stub
}

void SolidTrackAtom::applyContactEffects(Vehicle* ship, HitSide hs) {

	// Is this track atom deadly?
	if (mIsDeadly) {
		ship->mKilled = true;
	}

	cml::vector3f shipPos = ship->getPosition();

	// ############## BEGIN Jumping ###############

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
		break;
	}
	// #################### END Determine wall normal vector #################

	float dot2 = cml::dot(ship->mVelocity, wallNormal);

	cml::vector3f hitComponent = dot2 * wallNormal;

	ship->mVelocity -= hitComponent;

	//################ BEGIN Kill vehicle if it hits something with the nose too fast ###############
	if (cml::dot(ship->mDirForward, wallNormal) < -0.8 && hitComponent.length() > 0.3) {
		ship->mKilled = true;
		return;
	}
	//################ END Kill vehicle if it hits something with the nose too fast ###############


	//################ BEGIN Jumping ###############

	// Only jump if we hit the ground with the belly (that is, dot < 0)!:
	if (ship->mTryJump && cml::dot(ship->getGravity(), wallNormal) < 0) {
		cml::vector3f gravNormalized = cml::normalize(ship->getGravity());

		// NOTE: The factor that gravNormalized needs to be multiplied with depends
		// on the size/dimensions of the ship's bounding box!

		// TODO 3: Implement "ground probing ray" here. This would be better because
		// it is independent of the size of the bounding boxes of vehicle & track atom.
		// It would also allow us to implement hover behaviour for the vehicle.
		cml::vector3f tmp = shipPos + gravNormalized * 2.1;

		if (mBBox.containsPoint(tmp)) {
			ship->mVelocity -= gravNormalized * mJumpForce;
		}
	}
	// ############## END Jumping ###############


	//################ BEGIN Bouncing ###############

	// Do we hit the wall with the belly, and do we even still move towards it?
	if (cml::dot(ship->getGravity(), wallNormal) < 0 && cml::dot(ship->mVelocity, wallNormal) < 0) {

		if (hitComponent.length() > mBounceThreshold) {

			if (mRebound < 0) {
				ship->mVelocity -= wallNormal * mRebound;
			} else {
				ship->mVelocity -= hitComponent * mRebound;
			}
		}
	}
	//################ END Bouncing ###############

	//############### BEGIN Slip off blocks when we come too close to the edge ##############

	float slipSpeed = 0.01;

	if (abs(shipPos[0] - mBBox.mPos[0]) < mSlipOffset) {
		ship->mVelocity[0] -= slipSpeed;
	} else if (abs((mBBox.mPos[0] + mBBox.mSize[0]) - shipPos[0]) < mSlipOffset) {
		ship->mVelocity[0] += slipSpeed;
	}

	if (abs(shipPos[1] - mBBox.mPos[1]) < mSlipOffset) {
		ship->mVelocity[1] -= slipSpeed;
	} else if (abs((mBBox.mPos[1] + mBBox.mSize[1]) - shipPos[1]) < mSlipOffset) {
		ship->mVelocity[1] += slipSpeed;
	}

	if (abs(shipPos[2] - mBBox.mPos[2]) < mSlipOffset) {
		ship->mVelocity[2] -= slipSpeed;
	} else if (abs((mBBox.mPos[2] + mBBox.mSize[2]) - shipPos[2]) < mSlipOffset) {
		ship->mVelocity[2] += slipSpeed;
	}
	//############### END Slip off blocks when we come too close to the edge ##############

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

	// Prevent the vehicle from going through the wall:
	float dot = cml::dot(ship->mVelocity, wallNormal);

	if (dot < 0) {
		ship->mVelocity -= dot * wallNormal;
	}
}
