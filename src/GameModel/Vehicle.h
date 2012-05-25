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
	Vehicle(AbstractTrack*);
	virtual ~Vehicle();

	void cmd_accelerate(bool enabled);
	void cmd_brake(bool enabled);
	void cmd_moveLeft(bool enabled);
	void cmd_moveRight(bool enabled);
	void cmd_rotateDesiredOrientation(int axis, int steps);
	void cmd_tryJump(bool enabled);

	void doPhysicsStep();

	const cml::vector3f getGravity();
	const quat& getOrientation();

	void reset();

	cml::vector3f mDirLeft;
	cml::vector3f mDirForward;
	cml::vector3f mPos;
	cml::vector3f mVelocity;

	bool mTryJump;

	bool mKilled;
	bool mMoveLeft, mMoveRight, mAccelerate, mBrake;

	float mAccelLeftRight, mAccelForward;

private:

	std::vector<CollisionInfo> getCollidingTAs();
	void setOrientation(quat rotQuat);

	game::BoundingBox mBBox;

	AbstractTrack* mpTrack;

	quat mDesiredOrientation;
	quat mOrientation;

	float mOrientation_angle;

	cml::vector3f mOrientation_axis;
	cml::vector3f mGravity;
	cml::vector3f mBBoxPosOffset;
};

#endif /* SHIP_H_ */
