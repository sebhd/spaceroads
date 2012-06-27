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

	enum RacerCommand { CMD_SPACE_PRS, CMD_LEFT_PRS, CMD_RIGHT_PRS, CMD_ACCEL_PRS, CMD_BRAKE_PRS,
					 CMD_SPACE_REL, CMD_LEFT_REL, CMD_RIGHT_REL, CMD_ACCEL_REL, CMD_BRAKE_REL};

	struct ReplayCommand {
		RacerCommand cmd;
		unsigned long timestamp;
	};

	// Allow SolidTrackAtom class to access private members of Vehicle:
	friend class SolidTrackAtom;

	Racer();
	virtual ~Racer();

	void cmd_rotateDesiredOrientation(int axis, int steps);
	void cmd_spacebar(bool enabled);
	const cml::vector3f& getGravity();
	const quat& getOrientation();
	virtual void pilotStep(unsigned long step);
	void processCommand(RacerCommand);
	virtual void reset();
	void setOrientation(quat rotQuat);
	void updatePosition();
	void updateVelocity();

	bool mAddThrustLeft, mAddThrustRight;
	bool mAddThrustForward, mReduceThrustForward;
	bool mTryJump;
	bool mWantReset;

	float mThrustSideward, mThrustForward;

	float mAccelForward, mAccelSideward;

	quat mOrientation;

	AABB mBBox;

	cml::vector3f mGravity;
	cml::vector3f mPos;
	cml::vector3f mVelocity;
	cml::vector3f mDirLeft;
	cml::vector3f mDirForward;
	cml::vector3f mOldVel;

	bool mFinish;
	bool mKilled;

	bool mJumpedInThisStep;

	std::vector<ReplayCommand> mReplayCommands;

private:

	float mMaxThrustForward, mMaxThrustSideward;
	float mMaxSpeedForward, mMaxSpeedSideward;

};

#endif /* SHIP_H_ */
