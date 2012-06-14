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
#include "GameModel/Track/XMLFileTrack.h"

Application::Application() {

	quit = false;

	mpTrack = NULL;
	mpPlayerVehicle = NULL;
	mpPlayerVehicle = new LocalHumanRacer();

	mpRenderer = new OgreRenderer(this);

	mpRenderer->init();

	mpInputHandler = new OISInputHandler(getRenderer()->getWindowSize());

	mpInputHandler->mListeners.push_back(this);

}

Application::~Application() {
	// TODO Auto-generated destructor stub
}

void Application::handleKeyEvent(KeyboardEventListener::Key key, bool pressed) {

	switch (key) {
	case KeyboardEventListener::KEY_ESCAPE:
		quit = true;
		break;
	default:
		break;
	}

	mpPlayerVehicle->handleKeyEvent(key, pressed);

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

	// TODO 4: Make simulation step frequency configurable by introducing a time factor multiplicator in Vehicle::step() and...
	// all other places where it matters.

	unsigned long stepCount = 0;

	//########### BEGIN The Main Loop! ##########
	while (!quit) {

		gettimeofday(&newTime, NULL);

		long frameTime = newTime.tv_usec - currentTime.tv_usec;

		currentTime = newTime;

		accumulator += frameTime;

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

			if (mpPlayerVehicle->mWantReset) {
				mpPlayerVehicle->mPos = mpTrack->mStartPosition;
				mpPlayerVehicle->reset();
				mpRenderer->showKilledInfo(false);
				mpRenderer->showTrackCompletedInfo(false);
			}

			// If the ship is destroyed, reset to the starting position:
			if (mpPlayerVehicle->mKilled) {
				mpRenderer->showKilledInfo(true);
			} else if (mpPlayerVehicle->mFinish) {
				mpRenderer->showTrackCompletedInfo(true);
			} else {
				mpPlayerVehicle->mOldVel = mpPlayerVehicle->mVelocity;

				// Find colliding track atoms:
				std::vector<CollisionInfo> collisions = findCollidingTrackAtoms();

				// Apply counter-forces (prevent vehicle from going through walls):
				for (unsigned int ii = 0; ii < collisions.size(); ++ii) {
					collisions[ii].ta->applyCounterForces(mpPlayerVehicle, collisions[ii].hs);
				}

				mpPlayerVehicle->updatePosition();

				// Calculate & apply vehicle-internal effects on it's velocity:
				mpPlayerVehicle->updateVelocity();

				mpPlayerVehicle->mJumpedInThisStep = false;

				// Apply contact effects:
				for (unsigned int ii = 0; ii < collisions.size(); ++ii) {
					collisions[ii].ta->applyContactEffects(mpPlayerVehicle, collisions[ii].hs);
				}
			}

			accumulator -= dt;

			stepCount++;
		}

		/*
		if (stepCount % 100 == 0) {
			std::cout << double(stepCount) / 100 << std::endl;
		}
*/
		//	gettimeofday(&before, NULL);

		// Render next frame:
		if (!mpRenderer->renderOneFrame()) {
			//	return false;
			break;
		}
	}
	//########### END The Main Loop! ##########
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

				TrackAtom::HitSide hitSide = TrackAtom::HIT_NONE;

				if (!x) {

					if (mpPlayerVehicle->mVelocity[0] > 0) {
						hitSide = TrackAtom::HIT_LEFT;
					} else {
						hitSide = TrackAtom::HIT_RIGHT;
					}
				}

				else if (!y) {

					if (mpPlayerVehicle->mVelocity[1] > 0) {
						hitSide = TrackAtom::HIT_BOTTOM;
					} else {
						hitSide = TrackAtom::HIT_TOP;
					}
				}

				else if (!z) {

					if (mpPlayerVehicle->mVelocity[2] > 0) {
						hitSide = TrackAtom::HIT_BACK;
					} else {
						hitSide = TrackAtom::HIT_FRONT;
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

