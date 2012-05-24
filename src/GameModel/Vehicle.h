/*
 * Ship.h
 *
 *  Created on: 25.04.2012
 *      Author: sebastian
 */

#ifndef SHIP_H_
#define SHIP_H_

#include "GameModel.h"
#include "BoundingBox.h"

class GameModel;

// ACHTUNG: So muss ein Ogre-kompatibles Quaternion aussehen.
// Bin mir aber noch nicht sicher ob negative_cross oder positive_cross
typedef cml::quaternion<float, cml::fixed<>, cml::scalar_first, cml::negative_cross> quat;


class Vehicle {



public:
	Vehicle(GameModel* gameModel);
	virtual ~Vehicle();

	void doPhysicsStep();

	void cmd_accelerate(bool enabled);
	void cmd_brake(bool enabled);
	void cmd_tryJump(bool enabled);
	void cmd_moveLeft(bool enabled);
	void cmd_moveRight(bool enabled);

	const cml::vector3f getGravity();

	const quat& getOrientation();

	void reset();

	game::BoundingBox mBBox;

	cml::vector3f mPos;
	cml::vector3f mVelocity;

	bool mTryJump;
	bool mKilled;

	quat mDesiredOrientation;

	void setOrientation(quat rotQuat);

	cml::vector3f mVecLeft;
	cml::vector3f mVecForward;

	float mAccelLeftRight, mAccelForward;

	bool mMoveLeft, mMoveRight, mAccelerate, mBrake;


private:

	GameModel* mpGameModel;



	quat mOrientation;

	float mOrientation_angle;
	cml::vector3f mOrientation_axis;

	cml::vector3f mGravity;


	cml::vector3f mBBoxPosOffset;
};




#endif /* SHIP_H_ */
