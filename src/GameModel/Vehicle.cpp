/*
 * Ship.cpp
 *
 *  Created on: 25.04.2012
 *      Author: sebastian
 */
// TODO 3: Detect irrecoverable falling-off the track (and in that case, reset the vehicle)
// TODO 4: Entscheidung: Soll lenken im Flug  möglich sein?
// TODO 4: Entscheidlung: Soll man um so weiter zur Seite springen können, je schneller man fliegt?

#include "Vehicle.h"
#include <vector>
#include <cmath>
#include <ctime>
#include <sys/time.h>

Vehicle::Vehicle() {

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
	mAccelSideward = 0.0003;

	mThrustSideward = 0;
	mThrustForward = 0;

	mMaxThrustForward = 0.03;
	mMaxThrustSideward = 0.03;

	mMaxSpeedForward = 0.7;
	mMaxSpeedSideward = 0.2;


	mVelocity.set(0, 0, 0);

	cml::quaternion_rotation_axis_angle(mOrientation, cml::vector3f(1, 0, 0),
			(float) 0);
	cml::quaternion_rotation_axis_angle(mOrientation, cml::vector3f(0, 1, 0),
			(float) M_PI);

	setOrientation(mOrientation);
}

void Vehicle::updateVelocity() {

	// Apply gravity:
	mVelocity += getGravity();

	// ################ BEGIN Apply player vehicle control commands ###################

	//############ BEGIN Update forward thrust ##############
	if (mAddThrustForward) {
		mThrustForward += mAccelForward;
	} else if (mReduceThrustForward) {
		mThrustForward -= mAccelForward;
	}

	// Clamp forward thrust:
	if (mThrustForward > mMaxThrustForward) {
		mThrustForward = mMaxThrustForward;
	} else if (mThrustForward < 0) {
		mThrustForward = 0;
	}

	//############ END Update forward thrust ##############

	//############ BEGIN Apply forward thrust ##############
	cml::vector3f forwardComponent = cml::dot(mDirForward, mVelocity) * mDirForward;

	if ((mThrustForward > 0 && forwardComponent.length() < mMaxSpeedForward) || (mThrustForward < 0 && forwardComponent.length() > 0)) {
		mVelocity += mDirForward * mThrustForward;
	}
	//############ END Apply forward thrust ##############

	//############# BEGIN Update Sideward thrust ###############
	if (mAddThrustLeft) {
		mThrustSideward += mAccelSideward;
	} else if (mAddThrustRight) {
		mThrustSideward -= mAccelSideward;
	} else {
		mThrustSideward = 0;
	}

	// Clamp sideward thrust:
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

	// Apply "friction":
	mVelocity -= forwardComponent * 0.03;
	mVelocity -= sidewardComponent * 0.02;

	// ################ END Apply player vehicle control commands ###################
}


void Vehicle::updatePosition() {


	//################ BEGIN Enforce speed limits ################

	// TODO 3: Think about and - if it is so - explain why velocity
	// changes due to speed limits can be done *between* collision
	// detection and the 'applc counter forces' part of collision
	// detection without causing trouble.

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
	//################ END Enforce speed limits ################

	// Finally, move the vehicle by adding the velocity vector to the position:
	mPos += mVelocity;
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

	cml::quaternion_to_axis_angle(mOrientation, orientation_axis,
			orientation_angle, (float) 0);

	// Update gravity vector - it depends directly on the orientation:
	// TODO 3: Define gravity in track
	float g = -0.01;
	mGravity = cml::rotate_vector(cml::vector3f(0, g, 0), orientation_axis,
			orientation_angle);

	// Update left vector:
	mDirLeft = cml::rotate_vector(cml::vector3f(-1, 0, 0), orientation_axis,
			orientation_angle);

	// Update forward vector:
	mDirForward = cml::rotate_vector(cml::vector3f(0, 0, -1), orientation_axis,
			orientation_angle);
}

void Vehicle::cmd_rotateDesiredOrientation(int axis, int steps) {
	cml::quaternion_rotate_about_local_axis(mOrientation, axis,
			(float) (steps * M_PI / 2));
	setOrientation(mOrientation);
}

