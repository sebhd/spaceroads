/*
 * AbstractTrack.cpp
 *
 *  Created on: 25.05.2012
 *      Author: sebastian
 */

#include "Track.h"

Track::Track() {

	mSkybox = "SpaceRoads/SkyBoxes/2Moons";

	mAmbientLight.set(0,0,0);
	mDirectionalLightDir.set(-1,-1,-1);
}

Track::~Track() {
	// TODO Auto-generated destructor stub
}


void Track::computeExtent() {

	float minx = 1000000;
	float miny = 1000000;
	float minz = 1000000;

	float maxx = -1000000;
	float maxy = -1000000;
	float maxz = -1000000;

	// Apply counter-forces (prevent vehicle from going through walls):
	for (unsigned int ii = 0; ii < mTrackAtoms.size(); ++ii) {
		TrackAtom* ta = mTrackAtoms[ii];

		cml::vector3f mPos = ta->mBBox.mMin;
		cml::vector3f mSize = ta->mBBox.mMax;

		minx = std::min(minx, mPos[0]);
		miny = std::min(miny, mPos[1]);
		minz = std::min(minz, mPos[2]);

		maxx = std::max(maxx, mSize[0]);
		maxy = std::max(maxy, mSize[1]);
		maxz = std::max(maxz, mSize[2]);
	}

	float padding = 100;

	mExtent.mMin[0] = minx - padding;
	mExtent.mMin[1] = miny - padding;
	mExtent.mMin[2] = minz - padding;

	mExtent.mMax[0] = maxx + padding;
	mExtent.mMax[1] = maxy + padding;
	mExtent.mMax[2] = maxz + padding;

	//mExtent.mSize -= mExtent.mPos;
}



const std::vector<TrackAtom*> Track::getTrackAtomsAround(cml::vector3f pos) {

	return mTrackAtoms;
}
