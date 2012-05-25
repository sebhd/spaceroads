/*
 * RandomTrack.h
 *
 *  Created on: 25.05.2012
 *      Author: sebastian
 */

#ifndef RANDOMTRACK_H_
#define RANDOMTRACK_H_

#include "AbstractTrack.h"

class RandomTrack: public AbstractTrack {
public:
	RandomTrack(Application* a_app);
	virtual ~RandomTrack();

	virtual const std::vector<TrackAtom*> getTrackAtomsAround(cml::vector3f pos);

	virtual void step();
	virtual void reset();

protected:
	std::vector<TrackAtom*> mTrackAtoms;

	int nextX, nextY, nextZ;
};

#endif /* RANDOMTRACK_H_ */
