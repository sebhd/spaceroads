/*
 * AbstractTrack.cpp
 *
 *  Created on: 25.05.2012
 *      Author: sebastian
 */

#include "AbstractTrack.h"

AbstractTrack::AbstractTrack() {

	mSkybox = "SpaceRoads/SkyBoxes/2Moons";

	mAmbientLight.set(0,0,0);
	mDirectionalLightDir.set(-1,-1,-1);
}

AbstractTrack::~AbstractTrack() {
	// TODO Auto-generated destructor stub
}

void AbstractTrack::step(Vehicle* vehicle) {
	// In derived classes, this is the place for code to modify the track at runtime
}

void AbstractTrack::computeExtent() {

	float minx = 1000000;
	float miny = 1000000;
	float minz = 1000000;

	float maxx = -1000000;
	float maxy = -1000000;
	float maxz = -1000000;

	// Apply counter-forces (prevent vehicle from going through walls):
	for (unsigned int ii = 0; ii < mTrackAtoms.size(); ++ii) {
		TrackAtom* ta = mTrackAtoms[ii];

		cml::vector3f mPos = ta->mBBox.mPos;
		cml::vector3f mSize = ta->mBBox.mSize;

		minx = std::min(minx, mPos[0]);
		miny = std::min(miny, mPos[1]);
		minz = std::min(minz, mPos[2]);

		maxx = std::max(maxx, mPos[0] + mSize[0]);
		maxy = std::max(maxy, mPos[1] + mSize[1]);
		maxz = std::max(maxz, mPos[2] + mSize[2]);
	}

	float padding = 100;

	mExtent.mPos[0] = minx - padding;
	mExtent.mPos[1] = miny - padding;
	mExtent.mPos[2] = minz - padding;

	mExtent.mSize[0] = maxx + padding;
	mExtent.mSize[1] = maxy + padding;
	mExtent.mSize[2] = maxz + padding;

	mExtent.mSize -= mExtent.mPos;
}


void AbstractTrack::reset() {

}

const std::vector<TrackAtom*> AbstractTrack::getTrackAtomsAround(cml::vector3f pos) {

	return mTrackAtoms;
}
