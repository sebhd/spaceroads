/*
 * Application.cpp
 *
 *  Created on: 28.04.2012
 *      Author: sebastian
 */

// TODO 3: Implement some sort of spatial index to speed up collision detection
#include "Application.h"
#include <iostream>
#include <ctime>
#include <sys/time.h>

#include "Renderer/OGRERenderer/OgreRenderer.h"
#include "Input/OISInputHandler.h"
#include "GameModel/Tracks/XMLFileTrack.h"


Application::Application() {

	quit = false;

	mpTrack = NULL;
	mpPlayerVehicle = NULL;
	mpPlayerVehicle = new Racer();

	mpRenderer = new OgreRenderer(this);

	mpRenderer->init();

	mpInputHandler = new OISInputHandler(getRenderer()->getWindowSize());

	mpInputHandler->mListeners.push_back(this);

}

Application::~Application() {
	// TODO Auto-generated destructor stub
}

void Application::handleKeyEvent(KeyboardEventListener::Key key, bool pressed) {

	switch(key) {
	case KeyboardEventListener::KEY_ESCAPE:
		quit = true;
		break;
	default:
		break;
	}
}

void Application::playTrackFile(std::string filename) {

	if (mpTrack != NULL)
		delete (mpTrack);

	std::cout << "Loading track " << filename << "." << std::endl;
	mpTrack = new XMLFileTrack(filename);

	std::cout << "Preparing renderer for track.";

	mpPlayerVehicle->reset();
	mpPlayerVehicle->mPos = mpTrack->mStartPosition;



	mpRenderer->prepareForTrack();

	timeval currentTime, newTime;

	gettimeofday(&currentTime, NULL);
	unsigned long accumulator = 0;

	// 1/100 sec. simulation frequency
	unsigned int dt = 5000;

	// TODO 3: Understand performance problem on Eray's Macbook. Might have to do with the game speed timing code.

	// TODO 4: Make simulation step frequency configurable by introducing a time factor multiplicator in Vehicle::step() and...
	// all other places where it matters.

	unsigned int frametimetotal = 0;
	unsigned int framecount = 0;

	timeval before, after;

	//########### BEGIN The Main Loop! ##########
	while (!quit) {

		gettimeofday(&newTime, NULL);

		long frameTime = newTime.tv_usec - currentTime.tv_usec;

		currentTime = newTime;

		accumulator += frameTime;

		frametimetotal += frameTime;
		framecount++;

		if (accumulator > 20000) {
			accumulator = 20000;
			//	std::cout << "Zu lahm!" << std::endl;
		}

		// Process input:
		mpInputHandler->getInput();

		while (accumulator >= dt) {

			if (!mpTrack->mExtent.containsPoint(mpPlayerVehicle->mPos)) {
				mpPlayerVehicle->mKilled = true;
			}

			// If the ship is destroyed, reset to the starting position:
			if (mpPlayerVehicle->mKilled) {
				mpRenderer->showKilledInfo(true);

			}

			if (mpPlayerVehicle->mWantReset) {
				mpPlayerVehicle->mPos = mpTrack->mStartPosition;
				mpPlayerVehicle->reset();
				mpRenderer->showKilledInfo(false);
			}

			//		std::cout << mpPlayerVehicle->mPos << std::endl;

			mpPlayerVehicle->mOldVel = mpPlayerVehicle->mVelocity;

			// Find colliding track atoms:
			std::vector<CollisionInfo> collisions = findCollidingTrackAtoms();

			// Apply counter-forces (prevent vehicle from going through walls):
			for (unsigned int ii = 0; ii < collisions.size(); ++ii) {
				collisions[ii].ta->applyCounterForces(mpPlayerVehicle, collisions[ii].hs);
			}

			mpPlayerVehicle->updatePosition();

			mpPlayerVehicle->mJumpedInThisStep = false;

			// Apply contact effects:
			for (unsigned int ii = 0; ii < collisions.size(); ++ii) {
				collisions[ii].ta->applyContactEffects(mpPlayerVehicle, collisions[ii].hs);
			}

			// Calculate & apply vehicle-internal effects on it's velocity:
			mpPlayerVehicle->updateVelocity();

			accumulator -= dt;
		}

		gettimeofday(&before, NULL);

		// Render next frame:
		if (!mpRenderer->renderOneFrame()) {
			//	return false;
			break;
		}

		gettimeofday(&after, NULL);

		//std::cout << after.tv_usec - before.tv_usec << std::endl;

	}
	//########### END The Main Loop! ##########

	std::cout << double(frametimetotal) / framecount << std::endl;
}

std::vector<CollisionInfo> Application::findCollidingTrackAtoms() {

	std::vector<CollisionInfo> collisions;

	cml::vector3f bboxPos = mpPlayerVehicle->mPos + mpPlayerVehicle->mBBoxPosOffset;

	std::vector<TrackAtom*> trackAtoms = mpTrack->getTrackAtomsAround(mpPlayerVehicle->mPos);

	for (unsigned int ii = 0; ii < trackAtoms.size(); ++ii) {

		TrackAtom* ta = trackAtoms[ii];

		mpPlayerVehicle->mBBox.mPos = bboxPos + mpPlayerVehicle->mVelocity;

		if (ta->mBBox.intersectsWith(mpPlayerVehicle->mBBox)) {

			mpPlayerVehicle->mBBox.mPos = bboxPos;

			bool x = false, y = false, z = false;

			if (ta->mBBox.getIntersectingAxis(mpPlayerVehicle->mBBox, x, y, z) == 2) {

				HitSide hitSide = HIT_NONE;

				if (!x) {

					if (mpPlayerVehicle->mVelocity[0] > 0) {
						hitSide = HIT_LEFT;
					} else {
						hitSide = HIT_RIGHT;
					}
				}

				else if (!y) {

					if (mpPlayerVehicle->mVelocity[1] > 0) {
						hitSide = HIT_BOTTOM;
					} else {
						hitSide = HIT_TOP;
					}
				}

				else if (!z) {

					if (mpPlayerVehicle->mVelocity[2] > 0) {
						hitSide = HIT_BACK;
					} else {
						hitSide = HIT_FRONT;
					}
				}

				CollisionInfo ci;
				ci.ta = ta;
				ci.hs = hitSide;

				collisions.push_back(ci);
			}
		}
	}

	return collisions;
}

AbstractRenderer* Application::getRenderer() {
	return mpRenderer;
}

bool Application::handleFrameRenderingQueuedEvent() {

	return true;
}

