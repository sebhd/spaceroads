/*
 * GravChangerTrackAtom.cpp
 *
 *  Created on: 28.05.2012
 *      Author: sebastian
 */

// TODO 3: Textures on GravChangerTrackAtoms that show the direction in which the vehicle will be rotated

#include "GravChangerTrackAtom.h"

GravChangerTrackAtom::GravChangerTrackAtom(AABB bbox) : SolidTrackAtom(bbox) {
	// TODO Auto-generated constructor stub


	cml::quaternion_rotation_world_axis(mNewOrientation,0, (float) 0.0);

}

GravChangerTrackAtom::~GravChangerTrackAtom() {
	// TODO Auto-generated destructor stub
}


void GravChangerTrackAtom::applyContactEffects(Racer* ship, HitSide hs) {

	ship->setOrientation(mNewOrientation);

}

