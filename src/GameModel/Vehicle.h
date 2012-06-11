/*
 * Ship.h
 *
 *  Created on: 25.04.2012
 *      Author: sebastian
 */

#ifndef SHIP_H_
#define SHIP_H_

#include "BoundingBox.h"
#include "TrackAtoms/TrackAtom.h"
#include "Tracks/AbstractTrack.h"
#include <vector>

class AbstractTrack;
struct CollisionInfo;

// So muss ein Ogre-kompatibles Quaternion aussehen:
typedef cml::quaternion<float, cml::fixed<>, cml::scalar_first, cml::negative_cross> quat;


class Vehicle {

public:

	// Allow SolidTrackAtom class to access private members of Vehicle:
	friend class SolidTrackAtom;

	Vehicle(AbstractTrack*);
	virtual ~Vehicle();

	void cmd_rotateDesiredOrientation(int axis, int steps);

	void cmd_tryJump(bool enabled);
	void step();
	const cml::vector3f& getGravity();
	const quat& getOrientation();
	const cml::vector3f& getPosition();
	void reset();

	float getThrustForward() { return mThrustForward; }
	float getThrustSideward() { return mThrustSideward; }

	void setOrientation(quat rotQuat);

	bool mAddThrustLeft, mAddThrustRight, mAddThrustForward, mReduceThrustForward;
	bool mTryJump;

	bool mJumped;
	float mThrustSideward, mThrustForward;

	float mAccelForward;
	cml::vector3f mVelocity;

	quat mOrientation;

	cml::vector3f mDirLeft;
	cml::vector3f mDirForward;


private:

	std::vector<CollisionInfo> getCollidingTAs();


	game::BoundingBox mBBox;

	AbstractTrack* mpTrack;

	bool mKilled;


	cml::vector3f mPos;

	float mMaxThrustForward, mMaxThrustSideward;
	float mMaxSpeedForward, mMaxSpeedSideward;


	cml::vector3f mGravity;
	cml::vector3f mBBoxPosOffset;
};

#endif /* SHIP_H_ */
