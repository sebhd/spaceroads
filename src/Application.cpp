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
#include "GameModel/Tracks/RandomTrack.h"
#include "GameModel/Tracks/XMLFileTrack.h"

Application::Application() {

	quit = false;

	mpTrack = NULL;
	mpPlayerVehicle = NULL;
	mpPlayerVehicle = new Vehicle();
}

Application::~Application() {
	// TODO Auto-generated destructor stub
}

bool Application::keyPressed(const OIS::KeyEvent& evt) {

	switch (evt.key) {

	case OIS::KC_ESCAPE:
		quit = true;
		break;
	case OIS::KC_SPACE:
		mpPlayerVehicle->mTryJump = true;
		break;

	case OIS::KC_A:
		mpPlayerVehicle->cmd_rotateDesiredOrientation(1, 1);
		break;

	case OIS::KC_D:
		mpPlayerVehicle->cmd_rotateDesiredOrientation(1, -1);
		break;

	case OIS::KC_W:
		mpPlayerVehicle->cmd_rotateDesiredOrientation(0, -1);
		break;

	case OIS::KC_S:
		mpPlayerVehicle->cmd_rotateDesiredOrientation(0, 1);
		break;

	case OIS::KC_Q:
		mpPlayerVehicle->cmd_rotateDesiredOrientation(2, 1);
		break;

	case OIS::KC_E:
		mpPlayerVehicle->cmd_rotateDesiredOrientation(2, -1);
		break;

	case OIS::KC_R:
		mpPlayerVehicle->mKilled = true;
		break;

	case OIS::KC_RIGHT:
		mpPlayerVehicle->mAddThrustRight = true;
		break;

	case OIS::KC_LEFT:
		mpPlayerVehicle->mAddThrustLeft = true;
		break;

	case OIS::KC_UP:
		mpPlayerVehicle->mAddThrustForward = true;
		break;
	case OIS::KC_DOWN:
		mpPlayerVehicle->mReduceThrustForward = true;
		break;
	default:
		break;
	}

	return true;
}

bool Application::keyReleased(const OIS::KeyEvent& evt) {
	switch (evt.key) {

	case OIS::KC_ESCAPE:
		quit = true;
		break;
	case OIS::KC_SPACE:
		mpPlayerVehicle->mTryJump = false;
		break;
	case OIS::KC_LEFT:
		mpPlayerVehicle->mAddThrustLeft = false;
		break;
	case OIS::KC_RIGHT:
		mpPlayerVehicle->mAddThrustRight = false;
		break;
	case OIS::KC_UP:
		mpPlayerVehicle->mAddThrustForward = false;
		break;
	case OIS::KC_DOWN:
		mpPlayerVehicle->mReduceThrustForward = false;
		break;
	default:
		break;
	}

	return true;
}

void Application::playTrackFile(std::string filename) {

	if (mpTrack != NULL)
		delete (mpTrack);

	std::cout << "Loading track " << filename << "." << std::endl;
	mpTrack = new XMLFileTrack(filename);

	std::cout << "Preparing renderer for track.";

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

	mpPlayerVehicle->mKilled = true;

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

		while (accumulator >= dt) {

			// Process input:
			mpInputHandler->processInput();

			// Do track step:
			mpTrack->step(mpPlayerVehicle);

			// If the ship is destroyed, reset to the starting position:
			if (mpPlayerVehicle->mKilled) {
				std::cout << "Boooooom!!!" << std::endl;
				mpPlayerVehicle->mPos = mpTrack->mStartPosition;
				mpPlayerVehicle->reset();
				mpTrack->reset();
			}


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


void Application::init() {

	mpRenderer = new OgreRenderer(this);

	mpRenderer->init();

	mpInputHandler = new OISInputHandler(getRenderer()->getWindowSize());

	mpInputHandler->addKeyListener(this);

}

AbstractRenderer* Application::getRenderer() {
	return mpRenderer;
}

bool Application::handleFrameRenderingQueuedEvent() {

	return true;
}

