/*
 * ReplayRacer.h
 *
 *  Created on: 27.06.2012
 *      Author: sebastian
 */

#ifndef REPLAYRACER_H_
#define REPLAYRACER_H_

#include "Racer.h"

class ReplayRacer: public Racer {
public:
	ReplayRacer();
	virtual ~ReplayRacer();
	virtual void pilotStep(unsigned long step);
	virtual void reset();
	std::vector<ReplayEntry> mPlayedReplay;

private:
	unsigned int mLastReplayCmdIndex;
};

#endif /* REPLAYRACER_H_ */
