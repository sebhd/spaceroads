/*
 * AbstractTrack.h
 *
 *  Created on: 25.05.2012
 *      Author: sebastian
 */

#ifndef ABSTRACTTRACK_H_
#define ABSTRACTTRACK_H_

#include <vector>
#include "TrackDecorationMesh.h"
#include "../TrackAtoms/TrackAtom.h"
#include "../Racers/Racer.h"
#include "cml/cml.h"

class Track {
public:
	Track();
	virtual ~Track();

	void computeExtent();
	const std::vector<TrackAtom*> getTrackAtomsAround(cml::vector3f pos);

	cml::vector3f mStartPosition;
	cml::vector3f mAmbientLight;
	cml::vector3f mDirectionalLightDir;

	std::string mSkybox;

	bool mHasChanged;

	std::vector<TrackDecorationMesh> mMeshes;

	BoundingBox mExtent;

protected:

	std::vector<TrackAtom*> mTrackAtoms;

};

#endif /* ABSTRACTTRACK_H_ */
