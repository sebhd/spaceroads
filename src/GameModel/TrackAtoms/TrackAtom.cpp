/*
 * TrackAtom.cpp
 *
 *  Created on: 25.04.2012
 *      Author: sebastian
 */


#include "TrackAtom.h"

TrackAtom::TrackAtom(game::BoundingBox bbox) {

	mBBox = bbox;
	mMaterial = "SolidTrackAtom";
	name = "Unnamed Track Atom";
}

TrackAtom::~TrackAtom() {
	// TODO Auto-generated destructor stub

}
