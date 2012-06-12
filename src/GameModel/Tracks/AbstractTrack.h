/*
 * AbstractTrack.h
 *
 *  Created on: 25.05.2012
 *      Author: sebastian
 */

// TODO 4: Implementieren, dass sich bewegende TrackAtoms ein Vehicle mit sich schieben können...
// Derzeit ist es so, dass sich ein sich bewegendes TrackAtom durch ein stillstehendes Vehicle einfach
// hindurchbewegt (bzw. umgekehrt). Das ist doof.
#ifndef ABSTRACTTRACK_H_
#define ABSTRACTTRACK_H_

#include <vector>
#include "TrackDecorationMesh.h"
#include "../TrackAtoms/TrackAtom.h"
#include "../Vehicle.h"
#include "../../cml/cml.h"

class AbstractTrack {
public:
	AbstractTrack();
	virtual ~AbstractTrack();

	const std::vector<TrackAtom*> getTrackAtomsAround(cml::vector3f pos);

	virtual void step(Vehicle*);

	virtual void reset();

	cml::vector3f mStartPosition;
	cml::vector3f mAmbientLight;
	cml::vector3f mDirectionalLightDir;

	std::string mSkybox;

	bool mHasChanged;

	std::vector<TrackDecorationMesh> mMeshes;

protected:

	std::vector<TrackAtom*> mTrackAtoms;

};

#endif /* ABSTRACTTRACK_H_ */
