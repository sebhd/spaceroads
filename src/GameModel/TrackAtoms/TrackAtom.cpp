/*
 * TrackAtom.cpp
 *
 *  Created on: 25.04.2012
 *      Author: sebastian
 */


#include "TrackAtom.h"

TrackAtom::TrackAtom(BoundingBox bbox) {

	mBBox = bbox;
	mRenderMaterial = "SolidTrackAtom";
	mName = "Unnamed Track Atom";
	mIsDeadly = false;

}

TrackAtom::~TrackAtom() {
	// TODO Auto-generated destructor stub

}
