/*
 * Ship.cpp
 *
 *  Created on: 25.04.2012
 *      Author: sebastian
 */

// TODO 3: Lenken im Flug soll nicht möglich sein
// TODO 3: Je schneller man ist, desto weiter soll man zur Seite springen können
#include "Vehicle.h"
#include <vector>
#include <cmath>

Vehicle::Vehicle(GameModel* gameModel) :
		mpGameModel(gameModel) {

	mAccelLeftRight = 0.01;
	mAccelForward = 0.01;

	mBBox.mSize.set(4, 4, 4);
	mBBox.mPos.set(0, 4, 0);

	mBBoxPosOffset = cml::vector3f(-2, -2, -2);
	reset();
}

Vehicle::~Vehicle() {
	// TODO Auto-generated destructor stub
}

void Vehicle::reset() {

	mKilled = false;
	mPos = mpGameModel->mInitialPosition;
	mVelocity.set(0, 0, 0);

	cml::quaternion_rotation_axis_angle(mOrientation, cml::vector3f(1, 0, 0), (float) 0);
	setOrientation(mOrientation);

	mDesiredOrientation = mOrientation;
}

void Vehicle::cmd_accelerate(bool enabled) {
	mAccelerate = enabled;
}

void Vehicle::cmd_brake(bool enabled) {
	mBrake = enabled;
}

void Vehicle::cmd_tryJump(bool enabled) {
	mTryJump = enabled;
}

void Vehicle::cmd_moveLeft(bool enabled) {
	mMoveLeft = enabled;
}

void Vehicle::cmd_moveRight(bool enabled) {
	mMoveRight = enabled;
}

void Vehicle::doPhysicsStep() {

	// When we fall from the track, reset to start position:
	if (abs(mPos[1]) > 500 || abs(mPos[0]) > 500) {
		mKilled = true;
	}

	// If the ship is destroyed, reset to the starting position:
	if (mKilled) {
		reset();
		return;
	}

	//################# BEGIN Rotation in gewünschte Orientierung ##################
	if (mOrientation != mDesiredOrientation) {

		quat rotDiff = cml::quaternion_rotation_difference(mDesiredOrientation, mOrientation);

		float diffAngle = 0;
		cml::vector3f diffVec;
		cml::quaternion_to_axis_angle(rotDiff, diffVec, diffAngle, (float) 0);

		if (diffAngle > 0.01) {
			quat rotQuat;
			cml::quaternion_rotation_axis_angle(rotQuat, diffVec, -diffAngle * (float) 0.05);
			setOrientation(mOrientation * rotQuat);
		} else {
			setOrientation(mDesiredOrientation);
		}
	}
	//################# END Rotation in gewünschte Orientierung ##################

	// NOTE: Here begins the computation of the vehicle's new position. It consists of three steps:

	// ################ BEGIN STEP 1: Construct acceleration vector ###################

	// Initialize acceleration vector as a copy of the gravity vector:
	cml::vector3f mAccel = getGravity();

	//############ BEGIN Apply player-input-induced modifications to the acceleration vector ############

	cml::vector3f forwardComponent = cml::dot(mVecForward, mVelocity) * mVecForward;

	if (mAccelerate && !mBrake) {

		// Forward speed limit:
		if (forwardComponent.length() < 0.5) {
			mAccel[2] -= mAccelForward;
		}
	}

	if (mBrake && !mAccelerate) {

		// Driving backwards is not allowed:
//		if (cml::dot(mVecForward, mVelocity) > 0) {
		mAccel[2] += mAccelForward;
		//	}
	}
	//############ END Apply player-input-induced modifications to the acceleration vector ############

	// ################ END STEP 1: Construct acceleration vector ###################

	// ################ BEGIN STEP 2: Construct velocity vector (mostly enforcing speed limits) ###################

	// Apply acceleration:
	mVelocity += mAccel;

	// Apply all-direction speed limit:
	int topSpeed = 2;

	if (mVelocity.length() > topSpeed) {
		mVelocity = mVelocity.normalize() * topSpeed;
	}

	// Apply backwards spped limit (moving backwards is not allowed at all):
	if (cml::dot(mVecForward, mVelocity) < 0) {
		mVelocity -= cml::dot(mVecForward, mVelocity) * mVecForward;
	}

	// ################ END STEP 2: Construct velocity vector (mostly enforcing speed limits) ###################

	//######### BEGIN Collision detection & handling (may modify velocity vector) ################

	cml::vector3f bboxPos = mPos + mBBoxPosOffset;


	// ATTENTION: This *must* happen *before* the collision test!!!
	if (mFreeFlight) {

		if (mMoveLeft) {
			mVelocity += mVecLeft * mAccelLeftRight;
		}

		if (mMoveRight) {
			mVelocity -= mVecLeft * mAccelLeftRight;
		}

		// Apply sidewards friction:
		mVelocity -= cml::dot(mVecLeft, mVelocity) * mVecLeft * 0.03;
	}


	mFreeFlight = true;

	for (unsigned int ii = 0; ii < mpGameModel->mTrackAtoms.size(); ii++) {

		bool x = false, y = false, z = false;

		TrackAtom* ta = mpGameModel->mTrackAtoms[ii];

		mBBox.mPos = bboxPos + mVelocity;

		if (ta->mBBox.getIntersectingAxis(mBBox, x, y, z) == 3) {

			mBBox.mPos = bboxPos;

			if (ta->mBBox.getIntersectingAxis(mBBox, x, y, z) == 2) {

				mFreeFlight = false;

				TrackAtom::HitSide hitDir = TrackAtom::HIT_NONE;

				if (!x) {

					if (mVelocity[0] > 0) {
						hitDir = TrackAtom::HIT_LEFT;
					} else if (mVelocity[0] < 0) {
						hitDir = TrackAtom::HIT_RIGHT;
					}
				}

				else if (!y) {

					if (mVelocity[1] > 0) {
						hitDir = TrackAtom::HIT_BOTTOM;
					} else if (mVelocity[1] < 0) {
						hitDir = TrackAtom::HIT_TOP;
					}
				}

				else if (!z) {

					if (mVelocity[2] > 0) {
						hitDir = TrackAtom::HIT_BACK;
					} else if (mVelocity[2] < 0) {
						hitDir = TrackAtom::HIT_FRONT;
					}
				}

				ta->applyContactEffects(this, hitDir);

			}
		}
	}

	for (unsigned int ii = 0; ii < mpGameModel->mTrackAtoms.size(); ii++) {

		bool x = false, y = false, z = false;

		TrackAtom* ta = mpGameModel->mTrackAtoms[ii];

		mBBox.mPos = bboxPos + mVelocity;

		if (ta->mBBox.getIntersectingAxis(mBBox, x, y, z) == 3) {

			mBBox.mPos = bboxPos;

			if (ta->mBBox.getIntersectingAxis(mBBox, x, y, z) == 2) {

				TrackAtom::HitSide hitDir = TrackAtom::HIT_NONE;

				if (!x) {

					if (mVelocity[0] > 0) {
						hitDir = TrackAtom::HIT_LEFT;
					} else if (mVelocity[0] < 0) {
						hitDir = TrackAtom::HIT_RIGHT;
					}
				}

				else if (!y) {

					if (mVelocity[1] > 0) {
						hitDir = TrackAtom::HIT_BOTTOM;
					} else if (mVelocity[1] < 0) {
						hitDir = TrackAtom::HIT_TOP;
					}
				}

				else if (!z) {

					if (mVelocity[2] > 0) {
						hitDir = TrackAtom::HIT_BACK;
					} else if (mVelocity[2] < 0) {
						hitDir = TrackAtom::HIT_FRONT;
					}
				}

				ta->applyCounterForces(this, hitDir);
			}
		}
	}

	//######### END Collision detection & handling (may modify velocity vector) ################

	// Finally, move the vehicle by adding the velocity vector to the position:
	mPos += mVelocity;
}

const cml::vector3f Vehicle::getGravity() {
	return mGravity;
}

const quat& Vehicle::getOrientation() {
	return mOrientation;
}

void Vehicle::setOrientation(quat rotQuat) {

	mOrientation = rotQuat;
	cml::quaternion_to_axis_angle(mOrientation, mOrientation_axis, mOrientation_angle, (float) 0);

	// Update gravity vector - it depends directly on the orientation:
	mGravity = cml::rotate_vector(cml::vector3f(0, -0.01, 0), mOrientation_axis, mOrientation_angle);

	// Update left vector:
	mVecLeft = cml::rotate_vector(cml::vector3f(-1, 0, 0), mOrientation_axis, mOrientation_angle);

	// Update forward vector:
	mVecForward = cml::rotate_vector(cml::vector3f(0, 0, -1), mOrientation_axis, mOrientation_angle);
}
