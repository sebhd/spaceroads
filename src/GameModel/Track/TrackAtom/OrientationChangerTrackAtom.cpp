/*
 * GravChangerTrackAtom.cpp
 *
 *  Created on: 28.05.2012
 *      Author: sebastian
 */

// TODO 3: Textures on GravChangerTrackAtoms that show the direction in which the vehicle will be rotated

#include "OrientationChangerTrackAtom.h"

OrientationChangerTrackAtom::OrientationChangerTrackAtom(AABB bbox) : SolidTrackAtom(bbox) {
	// TODO Auto-generated constructor stub


	cml::quaternion_rotation_world_axis(mNewOrientation,0, (float) 0.0);

}

OrientationChangerTrackAtom::~OrientationChangerTrackAtom() {
	// TODO Auto-generated destructor stub
}


void OrientationChangerTrackAtom::applyContactEffects(Racer* ship, HitSide hs, unsigned int dt) {

	ship->setOrientation(mNewOrientation);

}

