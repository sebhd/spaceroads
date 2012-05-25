/*
 * AbstractTrack.h
 *
 *  Created on: 25.05.2012
 *      Author: sebastian
 */

#ifndef ABSTRACTTRACK_H_
#define ABSTRACTTRACK_H_

#include <vector>
#include "../TrackAtoms/TrackAtom.h"
#include "../../cml/cml.h"

class TrackAtom;

class AbstractTrack {
public:
	AbstractTrack();
	virtual ~AbstractTrack();

	virtual const std::vector<TrackAtom*>& getTrackAtomsAround(cml::vector3f pos) = 0;

	cml::vector3d mStartPosition;
};

#endif /* ABSTRACTTRACK_H_ */
