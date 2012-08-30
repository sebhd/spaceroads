/*
 * SolidTrackAtom.cpp
 *
 *  Created on: 04.05.2012
 *      Author: sebastian
 */

#include "SolidTrackAtom.h"

SolidTrackAtom::SolidTrackAtom(AABB bbox) :
		TrackAtom(bbox) {

	mRenderMaterial = "SpaceRoads/Track/Solid";
	mRenderMeshName = "";

	mBounceThreshold = 0.3;
	mRebound = 0.4;
	mJumpForce = 0.6;
	mSlipOffset = 0.01;
}

SolidTrackAtom::~SolidTrackAtom() {
	// TODO Auto-generated destructor stub
}

void SolidTrackAtom::applyContactEffects(Racer* ship, HitSide hs) {

	if (mIsDeadly) {
		ship->mKilled = true;
	}

	if (mIsEnergyRefresher) {
		// TODO 3: Implement Racer class member that defines max energy!
		ship->mEnergy = ship->mMaxEnergy;
	}

	// #################### BEGIN Determine wall normal vector #################
	cml::vector3f wallNormal(0, 0, 0);

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

	cml::vector3f hitComponent = cml::dot(ship->mOldVel, wallNormal) * wallNormal;

	// #################### END Determine wall normal vector #################

	//################ BEGIN Kill vehicle if it hits something with the nose too fast ###############
	if (cml::dot(ship->mDirForward, wallNormal) < -0.8 && hitComponent.length() > 0.3) {
		ship->mKilled = true;
		return;
	}
	//################ END Kill vehicle if it hits something with the nose too fast ###############

	// ################ BEGIN Jumping & bouncing ##############
	if (cml::dot(ship->getGravity(), wallNormal) < 0 && cml::dot(ship->mOldVel, wallNormal) < 0
			&& !ship->mJumpedInThisStep) {

		// Jumping...
		if (ship->mTryJump) {
			ship->mVelocity -= cml::normalize(ship->getGravity()) * mJumpForce;
		}

		// ... or bouncing (never both):
		else if (hitComponent.length() > mBounceThreshold) {

			if (mRebound < 0) {
				ship->mVelocity -= wallNormal * mRebound;
			} else {
				ship->mVelocity -= hitComponent * mRebound;
			}
		}

		ship->mJumpedInThisStep = true;
	}
	// ################ END Jumping & bouncing ##############

	//############### BEGIN Slip off blocks when we come too close to the edge ##############

	float slipSpeed = 0.005;

	cml::vector3f shipPos = ship->mPos;

	if (abs(shipPos[0] - mBBox.mMin[0]) < mSlipOffset) {
		ship->mVelocity[0] -= slipSpeed;
	} else if (abs(shipPos[0] - mBBox.mMax[0]) < mSlipOffset) {
		ship->mVelocity[0] += slipSpeed;
	}

	if (abs(shipPos[1] - mBBox.mMin[1]) < mSlipOffset) {
		ship->mVelocity[1] -= slipSpeed;
	} else if (abs(shipPos[1] - mBBox.mMax[1]) < mSlipOffset) {
		ship->mVelocity[1] += slipSpeed;
	}

	if (abs(shipPos[2] - mBBox.mMin[2]) < mSlipOffset) {
		ship->mVelocity[2] -= slipSpeed;
	} else if (abs(shipPos[2] - mBBox.mMax[2]) < mSlipOffset) {
		ship->mVelocity[2] += slipSpeed;
	}

	//############### END Slip off blocks when we come too close to the edge ##############

}

void SolidTrackAtom::applyCounterForces(Racer* ship, HitSide hs) {

	if (hs == HIT_TOP || hs == HIT_BOTTOM) {
		ship->mVelocity[1] = 0;
	} else if (hs == HIT_LEFT || hs == HIT_RIGHT) {
		ship->mVelocity[0] = 0;
	} else if (hs == HIT_FRONT || hs == HIT_BACK) {
		ship->mVelocity[2] = 0;
	}
}
