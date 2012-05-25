/*
 * Ship.cpp
 *
 *  Created on: 25.04.2012
 *      Author: sebastian
 */

// TODO 4: Entscheidung: Soll lenken im Flug  möglich sein?
// TODO 4: Entscheidlung: Soll man um so weiter zur Seite springen können, je schneller man fliegt?
#include "Vehicle.h"
#include <vector>
#include <cmath>

Vehicle::Vehicle(AbstractTrack* a_track) {

	mpTrack = a_track;

	// Acceleration capability:
	mAccelLeftRight = 0.01;
	mAccelForward = 0.01;

	// Configure the vehicle's collision AABB:
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
	mPos = mpTrack->mStartPosition;
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

	cml::vector3f forwardComponent = cml::dot(mDirForward, mVelocity) * mDirForward;

	if (mAccelerate && !mBrake) {
		// Forward speed limit:
		if (forwardComponent.length() < 0.5) {
			mVelocity += mDirForward * mAccelForward;
		}
	}

	if (mBrake && !mAccelerate) {

		// Driving backwards is not allowed:
		if (forwardComponent.length() > 0) {
			mVelocity -= mDirForward * mAccelForward;
		}
	}

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
	if (cml::dot(mDirForward, mVelocity) < 0) {
		mVelocity -= cml::dot(mDirForward, mVelocity) * mDirForward;
	}

	// ################ END STEP 2: Construct velocity vector (mostly enforcing speed limits) ###################

	//######### BEGIN STEP 3: Collision detection & handling (may modify velocity vector) ################

	// Find colliding track atoms and apply contact effects:
	std::vector<CollisionInfo> collisions = getCollidingTAs();

	for (unsigned int ii = 0; ii < collisions.size(); ++ii) {
		collisions[ii].ta->applyContactEffects(this, collisions[ii].hs);
	}

	// Find colliding track atoms again, and now apply counter forces:
	collisions = getCollidingTAs();

	for (unsigned int ii = 0; ii < collisions.size(); ++ii) {
		collisions[ii].ta->applyCounterForces(this, collisions[ii].hs);
	}

	//######### END STEP 3: Collision detection & handling (may modify velocity vector) ################

	// Finally, move the vehicle by adding the velocity vector to the position:
	mPos += mVelocity;

	//############### BEGIN Things that we do when there was no contact with a track atom #############

	// ATTENTION:
	// This *MUST* be done *AFTER* the position vector is updated! Otherwise, collision
	// handling will NOT work correctly and the vehicle will fall throught walls!!!

	if (collisions.size() == 0) {
		if (mMoveLeft) {
			mVelocity += mDirLeft * mAccelLeftRight;
		}

		if (mMoveRight) {
			mVelocity -= mDirLeft * mAccelLeftRight;
		}

		// Apply sidewards friction:
		mVelocity -= cml::dot(mDirLeft, mVelocity) * mDirLeft * 0.03;
	}
	//############### END Things that we do when there was no contact with a track atom #############
}

std::vector<CollisionInfo> Vehicle::getCollidingTAs() {

	std::vector<CollisionInfo> collisions;

	cml::vector3f bboxPos = mPos + mBBoxPosOffset;

	std::vector<TrackAtom*> trackAtoms = mpTrack->getTrackAtomsAround(mPos);

	for (unsigned int ii = 0; ii < trackAtoms.size(); ++ii) {

		TrackAtom* ta = trackAtoms[ii];

		mBBox.mPos = bboxPos + mVelocity;

		if (ta->mBBox.intersectsWith(mBBox)) {

			mBBox.mPos = bboxPos;

			bool x = false, y = false, z = false;

			if (ta->mBBox.getIntersectingAxis(mBBox, x, y, z) == 2) {

				TrackAtom::HitSide hitSide = TrackAtom::HIT_NONE;

				if (!x) {

					if (mVelocity[0] > 0) {
						hitSide = TrackAtom::HIT_LEFT;
					} else {
						hitSide = TrackAtom::HIT_RIGHT;
					}
				}

				else if (!y) {

					if (mVelocity[1] > 0) {
						hitSide = TrackAtom::HIT_BOTTOM;
					} else {
						hitSide = TrackAtom::HIT_TOP;
					}
				}

				else if (!z) {

					if (mVelocity[2] > 0) {
						hitSide = TrackAtom::HIT_BACK;
					} else {
						hitSide = TrackAtom::HIT_FRONT;
					}
				}

				CollisionInfo ci;
				ci.ta = ta;
				ci.hs = hitSide;

				collisions.push_back(ci);
			}
		}
	}

	return collisions;
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
	mDirLeft = cml::rotate_vector(cml::vector3f(-1, 0, 0), mOrientation_axis, mOrientation_angle);

	// Update forward vector:
	mDirForward = cml::rotate_vector(cml::vector3f(0, 0, -1), mOrientation_axis, mOrientation_angle);
}

void Vehicle::cmd_rotateDesiredOrientation(int axis, int steps) {
	cml::quaternion_rotate_about_local_axis(mDesiredOrientation, axis, (float) (steps * M_PI / 2));
}
