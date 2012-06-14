/*
 * Ship.h
 *
 *  Created on: 25.04.2012
 *      Author: sebastian
 */

#ifndef SHIP_H_
#define SHIP_H_

#include "AABB.h"
#include <vector>


// So muss ein Ogre-kompatibles Quaternion aussehen:
typedef cml::quaternion<float, cml::fixed<>, cml::scalar_first, cml::negative_cross> quat;

class Racer {

public:

	// Allow SolidTrackAtom class to access private members of Vehicle:
	friend class SolidTrackAtom;

	Racer();
	virtual ~Racer();

	void cmd_rotateDesiredOrientation(int axis, int steps);

	void cmd_spacebar(bool enabled);

	void updateVelocity();
	const cml::vector3f& getGravity();
	const quat& getOrientation();

	void reset();


	void setOrientation(quat rotQuat);

	void updatePosition();


	bool mAddThrustLeft, mAddThrustRight;
	bool mAddThrustForward, mReduceThrustForward;
	bool mTryJump;
	bool mWantReset;



	float mThrustSideward, mThrustForward;

	float mAccelForward, mAccelSideward;
	cml::vector3f mVelocity;

	quat mOrientation;

	cml::vector3f mDirLeft;
	cml::vector3f mDirForward;
	cml::vector3f mOldVel;

	bool mFinish;
	bool mKilled;

	bool mJumpedInThisStep;

	cml::vector3f mPos;

	AABB mBBox;
	cml::vector3f mBBoxPosOffset;

	cml::vector3f mGravity;

private:

	float mMaxThrustForward, mMaxThrustSideward;
	float mMaxSpeedForward, mMaxSpeedSideward;



};

#endif /* SHIP_H_ */
