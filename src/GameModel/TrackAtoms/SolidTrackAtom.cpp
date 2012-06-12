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

	// TODO 4: Is this the correct place for the jump code?
	if (ship->mTryJump) {
		cml::vector3f gravNormalized = cml::normalize(ship->getGravity());

		// NOTE: The factor that gravNormalized needs to be multiplied with depends
		// on the size/dimensions of the ship's bounding box!

		// This is better because it is independent of the size of the bounding boxes of vehicle & track atom.
		cml::vector3f tmp = shipPos + gravNormalized * 2.1;

		if (mBBox.containsPoint(tmp)) {
			ship->mVelocity -= gravNormalized * mJumpForce;
			ship->mJumped = true;
		}
	}
	// ############## END Jumping ###############

	//############### BEGIN Slip off blocks when we come too close to the edge ##############

	float slipSpeed = 0.01;

	if (abs(shipPos[0] - mBBox.mPos[0]) < mSlipOffset) {
		ship->mVelocity[0] -= slipSpeed;
	} else if (abs((mBBox.mPos[0] + mBBox.mSize[0]) - shipPos[0])
			< mSlipOffset) {
		ship->mVelocity[0] += slipSpeed;
	}

	if (abs(shipPos[1] - mBBox.mPos[1]) < mSlipOffset) {
		ship->mVelocity[1] -= slipSpeed;
	} else if (abs((mBBox.mPos[1] + mBBox.mSize[1]) - shipPos[1])
			< mSlipOffset) {
		ship->mVelocity[1] += slipSpeed;
	}

	if (abs(shipPos[2] - mBBox.mPos[2]) < mSlipOffset) {
		ship->mVelocity[2] -= slipSpeed;
	} else if (abs((mBBox.mPos[2] + mBBox.mSize[2]) - shipPos[2])
			< mSlipOffset) {
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
	// #################### END Determine wall normal vector #################

	float dot = cml::dot(ship->mVelocity, wallNormal);
	//float dot = cml::dot(ship->mVelocity + ship->mAddVelocity, wallNormal);

	// ATTENTION:
	// Here we do a sanity check to get around problems which may otherwise happen
	// further down in the code: If the vehicle's velocity vector points *AWAY* from
	// the wall (instead of towards it, as we expect it), we abort the function call here.

	// Note that *in theory*, this situation should never happen at all at this part
	// of the code. However, it occurs in at least one situation, namely when
	// mBounceThreshold == 0 and the vehicle passes over the border between two TrackAtoms
	// with the same surface level. In this case, the 'bouncing' will accelerate the vehicle
	// into the opposite direction than intended, causing it to fall through the ground.
	// Since this is shit, we better stop it right here. sbecht 2012-05-29

	// TODO 5: Understand why this happens

	if (dot >= 0) {
		return;
	}

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

	if (dot < -0.8 && hitComponent.length() > 0.3) {
		ship->mKilled = true;
	}
	//################ END Kill vehicle if it hits something with the nose too fast ###############
}
