/*
 * ReplayRacer.cpp
 *
 *  Created on: 27.06.2012
 *      Author: sebastian
 */

#include "ReplayRacer.h"
#include "../Application.h"

ReplayRacer::ReplayRacer() {
	// TODO Auto-generated constructor stub


}

ReplayRacer::~ReplayRacer() {
	// TODO Auto-generated destructor stub
}

void ReplayRacer::pilotStep(unsigned long step) {

	bool commandLeft = false;

	unsigned int ii = 0;

	for(ii = 0; ii < mPlayedReplay.size(); ++ii) {
		ReplayEntry cmd = mPlayedReplay[ii];

		if (cmd.timestamp == mStepsCount) {
			processCommand(cmd.cmd);
			mLastReplayCmdIndex = ii;
			commandLeft = true;
		}
	}

	// TODO 2: Reimplement!
	/*
	if (ii == mPlayedReplay.size() - 1 && !commandLeft) {
		mWantReset = true;

	}

	if (mGameState == -1) {
		mWantReset = true;
	}
	*/
}

void ReplayRacer::reset() {
	Racer::reset();

	mLastReplayCmdIndex = 0;
}
