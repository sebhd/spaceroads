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

	float dot = cml::dot(ship->getGravity(), wallNormal);

	// Only jump if we hit the ground with the belly (that is, dot < 0)!:
	if (ship->mTryJump && dot < 0) {
		cml::vector3f gravNormalized = cml::normalize(ship->getGravity());

		// NOTE: The factor that gravNormalized needs to be multiplied with depends
		// on the size/dimensions of the ship's bounding box!

		// TODO 3: Implement "ground probing ray" here. This would be better because
		// it is independent of the size of the bounding boxes of vehicle & track atom.
		// It would also allow us to implement hover behaviour for the vehicle.
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
	// #################### END Determine wall normal vector #################

	float dot = cml::dot(ship->mVelocity, wallNormal);

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

	// First of all, prevent the vehicle from going through the wall:
	ship->mVelocity -= hitComponent;



	// ############ BEGIN Bouncing ###################

	// Only allow bouncing if we hit the wall with the belly!
	// ATTENTION: This is not just an arbitrary rule! When jumping inside
	// tunnels, it is required to prevent bouncing off the tunnels roof
	// and being thrown back through the floor of the tunnel.

	// Of course, if we had better collision detection, the vehicle
	// wouldn't be pushed through the floor, but since our collision
	// detection is not perfect, we need to take all possible measures
	// to prevent the vehicle physis simulation from going into states
	// that our collision detection cannot handle correctly.
	// sbecht 2012-06-12

	// Do we hit the wall with the belly?
	dot = cml::dot(ship->getGravity(), wallNormal);

	if (dot >= 0) {
		return;
	}

	if (hitComponent.length() > mBounceThreshold) {

		if (mRebound < 0) {

			ship->mVelocity -= wallNormal * mRebound;
		} else {
			ship->mVelocity -= hitComponent * mRebound;
		}
	}
	// ############ END Bouncing ###################


	//################ BEGIN Kill vehicle if it hits something with the nose too fast ###############
	dot = cml::dot(ship->mDirForward, wallNormal);

	if (dot < -0.8 && hitComponent.length() > 0.3) {
		ship->mKilled = true;
	}
	//################ END Kill vehicle if it hits something with the nose too fast ###############
}
