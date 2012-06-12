/*
 * Ship.h
 *
 *  Created on: 25.04.2012
 *      Author: sebastian
 */

#ifndef SHIP_H_
#define SHIP_H_

#include "BoundingBox.h"
#include <vector>


// So muss ein Ogre-kompatibles Quaternion aussehen:
typedef cml::quaternion<float, cml::fixed<>, cml::scalar_first, cml::negative_cross> quat;

class Vehicle {

public:

	// Allow SolidTrackAtom class to access private members of Vehicle:
	friend class SolidTrackAtom;

	Vehicle();
	virtual ~Vehicle();

	void cmd_rotateDesiredOrientation(int axis, int steps);

	void cmd_tryJump(bool enabled);
	void step();
	const cml::vector3f& getGravity();
	const quat& getOrientation();
	const cml::vector3f& getPosition();
	void reset();

	float getThrustForward() {
		return mThrustForward;
	}
	float getThrustSideward() {
		return mThrustSideward;
	}

	void setOrientation(quat rotQuat);

	bool mAddThrustLeft, mAddThrustRight;
	bool mAddThrustForward, mReduceThrustForward;
	bool mTryJump;

	bool mJumped;
	float mThrustSideward, mThrustForward;

	float mAccelForward;
	cml::vector3f mVelocity;

	quat mOrientation;

	cml::vector3f mDirLeft;
	cml::vector3f mDirForward;

	bool mKilled;

	cml::vector3f mPos;

	game::BoundingBox mBBox;
	cml::vector3f mBBoxPosOffset;

	void updatePosition();

private:

	float mMaxThrustForward, mMaxThrustSideward;
	float mMaxSpeedForward, mMaxSpeedSideward;

	cml::vector3f mGravity;

};

#endif /* SHIP_H_ */
