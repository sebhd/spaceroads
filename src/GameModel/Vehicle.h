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


	bool mKilled;

	bool mJumpedInThisStep;

	cml::vector3f mPos;

	BoundingBox mBBox;
	cml::vector3f mBBoxPosOffset;


private:

	float mMaxThrustForward, mMaxThrustSideward;
	float mMaxSpeedForward, mMaxSpeedSideward;

	cml::vector3f mGravity;

};

#endif /* SHIP_H_ */
