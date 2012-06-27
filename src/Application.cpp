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

unsigned long Application::sm_timestamp = 0;

Application::Application() {

	quit = false;

	mpTrack = NULL;

	// Set up local human player's racer:
	LocalHumanRacer* playerRacer = new LocalHumanRacer();

	mLocalPlayerRacer = playerRacer;

	m_racers.push_back(playerRacer);

	mReplayRacer = new ReplayRacer();

	m_racers.push_back(mReplayRacer);

	// Set up the renderer:
	mpRenderer = new OgreRenderer(this);
	mpRenderer->init();

	mpInputHandler = new OISInputHandler(getRenderer()->getWindowSize());
	mpInputHandler->mListeners.push_back(this);

	mpInputHandler->mListeners.push_back(playerRacer);
}

Application::~Application() {
	// TODO Auto-generated destructor stub
}

void Application::handleKeyEvent(int key, bool pressed) {

	switch (key) {
	case KeyboardEventListener::KC_ESCAPE:
		quit = true;
		break;

	case KeyboardEventListener::KC_R:
		restart = true;
		break;
	default:
		break;
	}
}

void Application::playTrackFile(std::string filename) {

	quit = false;
	restart = false;

	if (mpTrack != NULL) {
		delete (mpTrack);
	}

	std::cout << "Loading track " << filename << "." << std::endl;
	mpTrack = new XMLFileTrack(filename);

	std::cout << "Preparing renderer for track.";

	// Reset vehicles:
	for (unsigned int ii = 0; ii < m_racers.size(); ii++) {
		m_racers[ii]->reset();
		m_racers[ii]->mPos = mpTrack->mStartPosition;
	}

	mReplayRacer->mPlayedReplay = mReplay;

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
	while (!quit && !restart) {

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

			sm_timestamp = stepCount;

			for (unsigned int ii = 0; ii < m_racers.size(); ii++) {

				m_racers[ii]->pilotStep(stepCount);

				doRacerStep(m_racers[ii]);
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

	// TODO 1: Uuuuuuuuuuuuglyyyyyyyyy!
	if (restart) {


		playTrackFile(filename);
	}
}

void Application::doRacerStep(Racer* racer) {

	if (!mpTrack->mExtent.containsPoint(racer->mPos)) {
		racer->mKilled = true;
	}

	if (racer->mWantReset) {
		mReplay = mLocalPlayerRacer->mReplayCommands;

		racer->mPos = mpTrack->mStartPosition;
		racer->reset();
		mpRenderer->showKilledInfo(false);
		mpRenderer->showTrackCompletedInfo(false);
	}

	// If the ship is destroyed, reset to the starting position:

	// TODO 1: Show killed/finished info only when it is true for the local player's racer!
	if (racer->mKilled) {
		mpRenderer->showKilledInfo(true);
	} else if (racer->mFinish) {
		mpRenderer->showTrackCompletedInfo(true);
	} else {
		racer->mOldVel = racer->mVelocity;

		// Find colliding track atoms:
		std::vector<CollisionInfo> collisions = findCollidingTrackAtoms(racer);

		// Apply counter-forces (prevent vehicle from going through walls):
		for (unsigned int ii = 0; ii < collisions.size(); ++ii) {
			collisions[ii].ta->applyCounterForces(racer, collisions[ii].hs);
		}

		racer->updatePosition();

		// Calculate & apply vehicle-internal effects on it's velocity:
		racer->updateVelocity();

		racer->mJumpedInThisStep = false;

		// Apply contact effects:
		for (unsigned int ii = 0; ii < collisions.size(); ++ii) {
			collisions[ii].ta->applyContactEffects(racer, collisions[ii].hs);
		}
	}
}

std::vector<CollisionInfo> Application::findCollidingTrackAtoms(Racer* racer) {

	AABB box1 = racer->mBBox;
	box1.mMin += racer->mPos;
	box1.mMax += racer->mPos;

	AABB box2 = box1;
	box2.mMin += racer->mVelocity;
	box2.mMax += racer->mVelocity;

	std::vector<CollisionInfo> collisions;

	std::vector<TrackAtom*> trackAtoms = mpTrack->getTrackAtomsAround(racer->mPos);

	for (unsigned int ii = 0; ii < trackAtoms.size(); ++ii) {

		TrackAtom* ta = trackAtoms[ii];

		if (ta->mBBox.intersectsWith(box2)) {

			bool x = false, y = false, z = false;

			if (ta->mBBox.getIntersectingAxis(box1, x, y, z) == 2) {

				TrackAtom::HitSide hitSide = TrackAtom::HIT_NONE;

				if (!x) {

					if (racer->mVelocity[0] > 0) {
						hitSide = TrackAtom::HIT_LEFT;
					} else {
						hitSide = TrackAtom::HIT_RIGHT;
					}
				}

				else if (!y) {

					if (racer->mVelocity[1] > 0) {
						hitSide = TrackAtom::HIT_BOTTOM;
					} else {
						hitSide = TrackAtom::HIT_TOP;
					}
				}

				else if (!z) {

					if (racer->mVelocity[2] > 0) {
						hitSide = TrackAtom::HIT_BACK;
					} else {
						hitSide = TrackAtom::HIT_FRONT;
					}
				}

				CollisionInfo ci;
				ci.ta = ta;
				ci.hs = hitSide;

				collisions.push_back(ci);
			} else {
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

