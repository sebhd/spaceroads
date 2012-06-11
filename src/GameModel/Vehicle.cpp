/*
 * Ship.cpp
 *
 *  Created on: 25.04.2012
 *      Author: sebastian
 */

// TODO 2: Implement some sort of spatial index to speed up collision detection
// TODO 3: Detect irrecoverable falling-off the track (and in that case, reset the vehicle)
// TODO 3: Implement "ground probing ray" underneath the vehicle for more solid "can i jump?"-check and possibility to implement hovering
// TODO 4: Entscheidung: Soll lenken im Flug  möglich sein?
// TODO 4: Entscheidlung: Soll man um so weiter zur Seite springen können, je schneller man fliegt?

#include "Vehicle.h"
#include <vector>
#include <cmath>
#include <ctime>
#include <sys/time.h>

Vehicle::Vehicle(AbstractTrack* a_track) {

	mpTrack = a_track;

	// Configure the vehicle's collision AABB:
	mBBox.mSize.set(4, 4, 4);
	mBBox.mPos.set(0, 4, 0);
	mBBoxPosOffset = cml::vector3f(-2, -2, -2);

	reset();
}

Vehicle::~Vehicle() {
	// TODO Auto-generated destructor stub
}

const cml::vector3f& Vehicle::getPosition() {
	return mPos;
}

void Vehicle::reset() {

	mAddThrustForward = false;
	mReduceThrustForward = false;
	mAddThrustLeft = false;
	mAddThrustRight = false;
	mTryJump = false;


	mKilled = false;

	// Somewhat close to original game:
	//mAccelForward = 0.00005;

	mAccelForward = 0.0001;


	mThrustSideward = 0;
	mThrustForward = 0;

	mMaxThrustForward = 0.03;
	mMaxThrustSideward = 0.03;

	mMaxSpeedForward = 0.7;
	mMaxSpeedSideward = 0.2;

	mPos = mpTrack->mStartPosition;
	mVelocity.set(0, 0, 0);

	cml::quaternion_rotation_axis_angle(mOrientation, cml::vector3f(1, 0, 0), (float) 0);
	cml::quaternion_rotation_axis_angle(mOrientation, cml::vector3f(0, 1, 0), (float) M_PI);

	setOrientation(mOrientation);


	mpTrack->reset();
}

void Vehicle::step() {

	// If the ship is destroyed, reset to the starting position:
	if (mKilled) {
		std::cout << "Boooooom!!!" << std::endl;
		reset();
		return;
	}

	// ################ BEGIN Apply player vehicle control commands ###################

	//############ BEGIN Update forward thrust ##############
	if (mAddThrustForward) {
		mThrustForward += mAccelForward;
	} else if (mReduceThrustForward) {
		mThrustForward -= mAccelForward;
	}

	if (mThrustForward > mMaxThrustForward) {
		mThrustForward = mMaxThrustForward;
	} else if (mThrustForward < 0) {
		mThrustForward = 0;
	}

	//############ END Update forward thrust ##############

	//############ BEGIN Apply forward thrust ##############
	cml::vector3f forwardComponent = cml::dot(mDirForward, mVelocity) * mDirForward;

	if (mThrustForward > 0 && forwardComponent.length() < mMaxSpeedForward) {
		mVelocity += mDirForward * mThrustForward;
	} else if (mThrustForward < 0 && forwardComponent.length() > 0) {
		mVelocity += mDirForward * mThrustForward;
	}
	//############ END Apply forward thrust ##############

	//############# BEGIN Update Sideward thrust ###############
	if (mAddThrustLeft) {
		mThrustSideward += 0.0005;
	} else if (mAddThrustRight) {
		mThrustSideward -= 0.0005;
	} else {
		mThrustSideward = 0;
	}

	if (mThrustSideward > mMaxThrustSideward) {
		mThrustSideward = mMaxThrustSideward;
	} else if (mThrustSideward < -mMaxThrustSideward) {
		mThrustSideward = -mMaxThrustSideward;
	}
	//############# END Update Sideward thrust ###############

	//############### BEGIN Apply sideward thrust ################
	cml::vector3f sidewardComponent = cml::dot(mDirLeft, mVelocity) * mDirLeft;

	if (sidewardComponent.length() < mMaxSpeedSideward) {
		mVelocity += mDirLeft * mThrustSideward;
	}
	//############### END Apply sideward thrust ################

	// Apply speed limits / "friction":
	mVelocity -= forwardComponent * 0.03;
	mVelocity -= sidewardComponent * 0.02;

	// ################ END Apply player vehicle control commands ###################

	// Apply gravity:
	mVelocity += getGravity();



	//######### BEGIN Collision detection & handling (may modify velocity vector) ################


	timeval before, after;

	gettimeofday(&before, NULL);

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

	gettimeofday(&after, NULL);

//	long time = after.tv_usec - before.tv_usec;

//	std::cout << time << std::endl;

	//######### END Collision detection & handling (may modify velocity vector) ################

	//################ BEGIN Enfore speed limits ################

	// ATTENTION:
	// Be aware of the fact that in the case of a collision, the vehicle will just pass through
	// a TrackAtom if its speed in the direction of the colliding wall is larger than the
	// thickness of the block in that direction! sbecht 2012-05-26
	// TODO 4: Try to fix this.

	int topSpeed = 2;

	if (mVelocity.length() > topSpeed) {
		mVelocity = mVelocity.normalize() * topSpeed;
	}

	// Apply backwards spped limit (moving backwards is not allowed at all):
	if (cml::dot(mDirForward, mVelocity) < 0) {
		mVelocity -= cml::dot(mDirForward, mVelocity) * mDirForward;
	}
	//################ END Enfore speed limits ################

	// Finally, move the vehicle by adding the velocity vector to the position:
	mPos += mVelocity;
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

const cml::vector3f& Vehicle::getGravity() {
	return mGravity;
}

const quat& Vehicle::getOrientation() {
	return mOrientation;
}

void Vehicle::setOrientation(quat rotQuat) {
	mOrientation = rotQuat;

	cml::vector3f orientation_axis;
	float orientation_angle;

	cml::quaternion_to_axis_angle(mOrientation, orientation_axis, orientation_angle, (float) 0);

	// Update gravity vector - it depends directly on the orientation:
	// TODO 3: Define gravity in track
	float g = -0.01;
	mGravity = cml::rotate_vector(cml::vector3f(0, g, 0), orientation_axis, orientation_angle);

	// Update left vector:
	mDirLeft = cml::rotate_vector(cml::vector3f(-1, 0, 0), orientation_axis, orientation_angle);

	// Update forward vector:
	mDirForward = cml::rotate_vector(cml::vector3f(0, 0, -1), orientation_axis, orientation_angle);
}

void Vehicle::cmd_rotateDesiredOrientation(int axis, int steps) {
	cml::quaternion_rotate_about_local_axis(mOrientation, axis, (float) (steps * M_PI / 2));
	setOrientation(mOrientation);
}

