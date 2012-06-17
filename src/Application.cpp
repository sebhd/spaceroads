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

void Application::handleKeyEvent(int key, bool pressed) {

	switch (key) {
	case KeyboardEventListener::KC_ESCAPE:
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

	// 1/200 sec. simulation frequency
	unsigned int dt = 5000;


	// TODO 3: Make simulation step frequency changeable without having to change all acceleration/velocity-related...
	// variables in the Racer class manually

	unsigned long stepCount = 0;

	//########### BEGIN The Main Loop! ##########
	while (!quit) {

		gettimeofday(&newTime, NULL);

		long frameTime = newTime.tv_usec - currentTime.tv_usec;

		// Prevent "spiral of death":
		if (frameTime > 20000) {
			frameTime = 20000;
		}

		if (frameTime < 0) {
			frameTime = 0;
		}

	//	std::cout << frameTime << std::endl;

		currentTime = newTime;

		accumulator += frameTime;

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

		// Render next frame:
		if (!mpRenderer->renderOneFrame()) {
			//	return false;
			break;
		}
	}
	//########### END The Main Loop! ##########
}

std::vector<CollisionInfo> Application::findCollidingTrackAtoms() {

	AABB box1 = mpPlayerVehicle->mBBox;
	box1.mMin += mpPlayerVehicle->mPos;
	box1.mMax += mpPlayerVehicle->mPos;

	AABB box2 = box1;
	box2.mMin += mpPlayerVehicle->mVelocity;
	box2.mMax += mpPlayerVehicle->mVelocity;

	std::vector<CollisionInfo> collisions;

	std::vector<TrackAtom*> trackAtoms = mpTrack->getTrackAtomsAround(mpPlayerVehicle->mPos);

	for (unsigned int ii = 0; ii < trackAtoms.size(); ++ii) {

		TrackAtom* ta = trackAtoms[ii];

		if (ta->mBBox.intersectsWith(box2)) {

			bool x = false, y = false, z = false;

			if (ta->mBBox.getIntersectingAxis(box1, x, y, z) == 2) {

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
			else {
			//	std::cout << "Pathologische Situation" << std::endl;
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

