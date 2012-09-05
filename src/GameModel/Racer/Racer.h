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
#include "../Track/TrackAtom.h"

// So muss ein Ogre-kompatibles Quaternion aussehen:
typedef cml::quaternion<float, cml::fixed<>, cml::scalar_first, cml::negative_cross> quat;

class TrackAtom;

class Racer {

public:

	enum RacerCommand {CMD_SPACE_PRS,
						CMD_LEFT_PRS,
						CMD_RIGHT_PRS,
						CMD_ACCEL_PRS,
						CMD_BRAKE_PRS,
						CMD_SPACE_REL,
						CMD_LEFT_REL,
						CMD_RIGHT_REL,
						CMD_ACCEL_REL,
						CMD_BRAKE_REL,
						CMD_ROTATE1,
						CMD_ROTATE2,
						CMD_ROTATE3,
						CMD_ROTATE4};

	struct ReplayEntry {
		RacerCommand cmd;
		unsigned long timestamp;
	};

	TrackAtom* mLastHit;

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

	void updatePosition();
	void updateVelocity();

	bool mAddThrustLeft, mAddThrustRight;
	bool mAddThrustForward, mReduceThrustForward;
	bool mTryJump;


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

	int mGameState;

	bool mJumpedInThisStep;

	std::vector<ReplayEntry> mReplayCommands;

	// TODO 3: Merge these two into one?
	unsigned long int mStepsCount;
	unsigned long int mRaceTime;

	unsigned int mRotatorCountdown;
	unsigned int mRotatorSteps;
	unsigned int mRotatorAxis;


	unsigned int mEnergy;
	unsigned int mMaxEnergy;

private:
	void setOrientation(quat rotQuat);

	float mMaxThrustForward, mMaxThrustSideward;
	float mMaxSpeedForward, mMaxSpeedSideward;

};

#endif /* SHIP_H_ */
