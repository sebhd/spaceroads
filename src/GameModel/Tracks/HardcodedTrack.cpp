/*
 * HardcodedTrack.cpp
 *
 *  Created on: 25.05.2012
 *      Author: sebastian
 */

#include "HardcodedTrack.h"
#include "../TrackAtoms/SolidTrackAtom.h"



HardcodedTrack::HardcodedTrack(Application* a_app) : AbstractTrack(a_app) {

	mStartPosition.set(5, 4, -4);


	//################# BEGIN Construct the track ##################

	// Anfangsteil:
	SolidTrackAtom* ta = new SolidTrackAtom(game::BoundingBox(cml::vector3d(0, 0, -40), cml::vector3d(10, 1, 40)));
	ta->name = "anfangsteil";
	mTrackAtoms.push_back(ta);

	// Querteil:
	ta = new SolidTrackAtom(game::BoundingBox(cml::vector3d(-20, 0, -50), cml::vector3d(50, 1, 10)));
	ta->name = "Querteil";
	mTrackAtoms.push_back(ta);

	// Dach über dem Anfangsteil:
	mTrackAtoms.push_back(new SolidTrackAtom(game::BoundingBox(cml::vector3d(0, 10, -10), cml::vector3d(10, 1, 10))));


	// Seitenwand vorne links:
	mTrackAtoms.push_back(new SolidTrackAtom(game::BoundingBox(cml::vector3d(-1, 1, -20), cml::vector3d(1, 10, 20))));

	// Seitenwand vorne rechts:
	mTrackAtoms.push_back(new SolidTrackAtom(game::BoundingBox(cml::vector3d(10, 1, -20), cml::vector3d(1, 10, 20))));

	mTrackAtoms.push_back(new SolidTrackAtom(game::BoundingBox(cml::vector3d(-20, 0, -100), cml::vector3d(10, 1, 50))));
	//mTrackAtoms.push_back(new SolidTrackAtom(game::BoundingBox(cml::vector3d(-20, 0, -100), cml::vector3d(10, 1, 50))));

	// Jump Pad am Anfang:
	ta = new SolidTrackAtom(game::BoundingBox(cml::vector3d(-20, 0, -120), cml::vector3d(50, 5, 20)));
	ta->mMaterial = "JumpPadTrackAtom";
	ta->mBounceThreshold = 0;
	ta->mRebound = -0.5;
	mTrackAtoms.push_back(ta);


	mTrackAtoms.push_back(new SolidTrackAtom(game::BoundingBox(cml::vector3d(20, 0, -200), cml::vector3d(10, 1, 80))));
	mTrackAtoms.push_back(new SolidTrackAtom(game::BoundingBox(cml::vector3d(20, 0, -300), cml::vector3d(10, 1, 80))));

	mTrackAtoms.push_back(new SolidTrackAtom(game::BoundingBox(cml::vector3d(20, -10, -400), cml::vector3d(10, 1, 100))));

	// JumpPad auf die höhere Ebene:
	ta = new SolidTrackAtom(game::BoundingBox(cml::vector3d(20, -20, -450), cml::vector3d(10, 1, 50)));
	ta->mMaterial = "JumpPadTrackAtom";
	ta->mBounceThreshold = 0;
	ta->mRebound = -1.5;
	mTrackAtoms.push_back(ta);




	mTrackAtoms.push_back(new SolidTrackAtom(game::BoundingBox(cml::vector3d(20, 50, -900), cml::vector3d(10, 1, 300))));

	ta = new SolidTrackAtom(game::BoundingBox(cml::vector3d(20, 50, -920), cml::vector3d(10, 1, 20)));
	ta->mIsDeadly = true;
	ta->mMaterial = "DeadlyTrackAtom";

	mTrackAtoms.push_back(ta);

	mTrackAtoms.push_back(new SolidTrackAtom(game::BoundingBox(cml::vector3d(20, 50, -1000), cml::vector3d(10, 1, 80))));

	mTrackAtoms.push_back(
			new SolidTrackAtom(game::BoundingBox(cml::vector3d(-100, 50, -1050), cml::vector3d(150, 1, 30))));


	mTrackAtoms.push_back(
			new SolidTrackAtom(game::BoundingBox(cml::vector3d(-100, 50, -1500), cml::vector3d(20, 1, 450))));
	mTrackAtoms.push_back(
			new SolidTrackAtom(game::BoundingBox(cml::vector3d(-100, 51, -1400), cml::vector3d(20, 10, 50))));

	// Hohe Seitenwand rechts:
	mTrackAtoms.push_back(
			new SolidTrackAtom(game::BoundingBox(cml::vector3d(60, -20, -400), cml::vector3d(1, 100, 300))));

	//################# END Construct the track ##################

}

HardcodedTrack::~HardcodedTrack() {
	// TODO Auto-generated destructor stub

}


const std::vector<TrackAtom*> HardcodedTrack::getTrackAtomsAround(cml::vector3f pos) {

	return mTrackAtoms;
}


void HardcodedTrack::step() {
	// In derived classes, this is the place for code to modify the track at runtime

}

