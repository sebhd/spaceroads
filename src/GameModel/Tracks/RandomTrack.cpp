/*
 * RandomTrack.cpp
 *
 *  Created on: 25.05.2012
 *      Author: sebastian
 */

#include "RandomTrack.h"
#include "../TrackAtoms/SolidTrackAtom.h"
#include "../BoundingBox.h"
#include <ctime>

RandomTrack::RandomTrack(Application* a_app) : AbstractTrack(a_app) {
	// TODO Auto-generated constructor stub

	mStartPosition.set(5, 50, 50);

	nextX = 0;
	nextY = 0;
	nextZ = 0;

	//####################### BEGIN Zufallsstecke bauen ####################
	/* initialize random seed: */
	srand(time(NULL));

	/*
	for (unsigned int ii = 0; ii < 100; ii++) {

		int whoknows = rand() % 10 + 1;

		game::BoundingBox bbox;
		bbox.mPos.set(nextX, nextY - whoknows * 10, nextZ);
		bbox.mSize.set(10, whoknows * 10, 100);

		TrackAtom* ta = new SolidTrackAtom(bbox);

		mTrackAtoms.push_back(ta);

		int oneOrTwo = rand() % 2;

		if (oneOrTwo == 0) {
			nextX += 20;
		} else {
			nextX -= 20;
		}

		oneOrTwo = rand() % 2;

		if (oneOrTwo == 0) {
			nextY += 10;
		} else {
			nextY -= 10;
		}

		nextZ -= 130;
		mHasChanged = true;
	}
	*/
}

RandomTrack::~RandomTrack() {
	// TODO Auto-generated destructor stub
}

const std::vector<TrackAtom*> RandomTrack::getTrackAtomsAround(cml::vector3f pos) {

	return mTrackAtoms;
}

void RandomTrack::step() {
	// In derived classes, this is the place for code to modify the track at runtime


	cml::vector3f shipPos = mpApp->mpPlayerVehicle->getPosition();

	//############# BEGIN Delete TrackAtoms behind the player vehicle ###########
	for (unsigned int ii = 0; ii < mTrackAtoms.size(); ii++) {

		TrackAtom* ta = mTrackAtoms[ii];

		if (ta->mBBox.mPos[2] > shipPos[2] + 1000) {
			mTrackAtoms.erase(mTrackAtoms.begin() + ii);

			mHasChanged = true;
			break;
		}
	}
	//############# END Delete TrackAtoms behind the player vehicle ###########


	if (mTrackAtoms.size() < 40) {

		int whoknows = rand() % 10 + 1;

		game::BoundingBox bbox;
		bbox.mPos.set(nextX, nextY - whoknows * 10, nextZ);
		bbox.mSize.set(10, whoknows * 10, 100);

		SolidTrackAtom* ta = new SolidTrackAtom(bbox);

		if (whoknows == 7) {
			ta->mMaterial = "JumpPadTrackAtom";
			ta->mRebound = -1;
			ta->mBounceThreshold = 0;
		}

		mTrackAtoms.push_back(ta);

		int oneOrTwo = rand() % 2;

		if (oneOrTwo == 0) {
			nextX += rand() % 5 * 7;
		} else {
			nextX -= rand() % 5 * 7;
		}

		oneOrTwo = rand() % 2;

		if (oneOrTwo == 0) {
			nextY += 10;
		} else {
			nextY -= 10;
		}

		nextZ -= 130;
		mHasChanged = true;

	}

}

void RandomTrack::reset() {

	mTrackAtoms.clear();

	nextX = 0;
	nextY = 0;
	nextZ = 0;

}

