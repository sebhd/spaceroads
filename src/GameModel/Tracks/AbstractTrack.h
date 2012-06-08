/*
 * AbstractTrack.h
 *
 *  Created on: 25.05.2012
 *      Author: sebastian
 */

// TODO 4: Implementieren, dass sich bewegende TrackAtoms ein Vehicle mit sich schieben können...
// Derzeit ist es so, dass sich ein sich bewegendes TrackAtom durch ein stillstehendes Vehicle einfach
// hindurchbewegt (bzw. umgekehrt). Das ist doof. sbecht 2012-05-25

#ifndef ABSTRACTTRACK_H_
#define ABSTRACTTRACK_H_

#include <vector>
#include "../TrackAtoms/TrackAtom.h"
#include "../../cml/cml.h"
#include "../../Application.h"

class Application;
class TrackAtom;

class AbstractTrack {
public:
	AbstractTrack(Application*);
	virtual ~AbstractTrack();

	virtual const std::vector<TrackAtom*> getTrackAtomsAround(cml::vector3f pos) = 0;

	virtual void step();

	virtual void reset();

	cml::vector3d mStartPosition;

	std::string mSkybox;

	bool mHasChanged;

protected:
	Application* mpApp;

};

#endif /* ABSTRACTTRACK_H_ */
