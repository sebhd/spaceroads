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

void AbstractTrack::reset() {

}

const std::vector<TrackAtom*> AbstractTrack::getTrackAtomsAround(cml::vector3f pos) {

	return mTrackAtoms;
}
