/*
 * HardcodedTrack.h
 *
 *  Created on: 25.05.2012
 *      Author: sebastian
 */

#ifndef HARDCODEDTRACK_H_
#define HARDCODEDTRACK_H_

#include "AbstractTrack.h"

class HardcodedTrack: public AbstractTrack {
public:
	HardcodedTrack();
	virtual ~HardcodedTrack();

	virtual const std::vector<TrackAtom*>& getTrackAtomsAround(cml::vector3f pos);

protected:
	std::vector<TrackAtom*> mTrackAtoms;
};

#endif /* HARDCODEDTRACK_H_ */
