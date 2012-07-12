/*
 * Application.cpp
 *
 *  Created on: 28.04.2012
 *      Author: sebastian
 */

// TODO 3: Implement some sort of spatial index to speed up collision detection
#include "Application.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <tinyxml.h>

#include "Renderer/OGRERenderer/OgreRenderer.h"
#include "Input/OISInputHandler.h"
#include "GameModel/Track/XMLFileTrack.h"

Application::Application() {

	watchReplay = true;
	quit = false;

	mpTrack = NULL;

	// Set up local human player's racer:
	Racer* playerRacer = new Racer();

	mLocalPlayerRacer = playerRacer;

	m_racers.push_back(playerRacer);

	mReplayRacer = new ReplayRacer();

	//m_racers.push_back(mReplayRacer);

	// Set up the renderer:
	mpRenderer = new OgreRenderer(this);
	mpRenderer->init();

	mpInputHandler = new OISInputHandler(getRenderer()->getWindowSize());
	mpInputHandler->mListeners.push_back(this);
}

Application::~Application() {
	// TODO Auto-generated destructor stub
}

void Application::handleKeyEvent(int key, bool pressed) {

	if (watchReplay) {

		switch (key) {
		case KeyboardEventListener::KC_ESCAPE:
			quit = true;
			stopPlayingTrack = true;
			break;

		default:
			watchReplay = false;
			stopPlayingTrack = true;

			break;
		}
	} else {

		if (key == KeyboardEventListener::KC_ESCAPE) {
			stopPlayingTrack = true;
			quit = true;
		}

		if (key == KC_SPACE) {
			if (pressed) {
				mLocalPlayerRacer->processCommand(Racer::CMD_SPACE_PRS);
			} else {
				mLocalPlayerRacer->processCommand(Racer::CMD_SPACE_REL);
			}

		}

		if (!mLocalPlayerRacer->mKilled) {
			switch (key) {

			case KC_A:
				if (pressed) {
					mLocalPlayerRacer->processCommand(Racer::CMD_ROTATE1);
					//mLocalPlayerRacer->cmd_rotateDesiredOrientation(1, 1);
				}
				break;

			case KC_D:
				if (pressed) {
					mLocalPlayerRacer->processCommand(Racer::CMD_ROTATE2);
					//mLocalPlayerRacer->cmd_rotateDesiredOrientation(1, -1);
				}

				break;

			case KC_Q:
				if (pressed) {
					mLocalPlayerRacer->processCommand(Racer::CMD_ROTATE3);
					//mLocalPlayerRacer->cmd_rotateDesiredOrientation(2, 1);
				}
				break;

			case KC_E:
				if (pressed) {
					mLocalPlayerRacer->processCommand(Racer::CMD_ROTATE4);
					//mLocalPlayerRacer->cmd_rotateDesiredOrientation(2, -1);
				}
				break;

			case KC_DOWN:
				//mReduceThrustForward = pressed;
				if (pressed) {
					mLocalPlayerRacer->processCommand(Racer::CMD_BRAKE_PRS);
				} else {
					mLocalPlayerRacer->processCommand(Racer::CMD_BRAKE_REL);
				}
				break;

			case KC_LEFT:
				//mAddThrustLeft = pressed;
				if (pressed) {
					mLocalPlayerRacer->processCommand(Racer::CMD_LEFT_PRS);
				} else {
					mLocalPlayerRacer->processCommand(Racer::CMD_LEFT_REL);
				}

				break;

			case KC_RIGHT:
				//mAddThrustRight = pressed;
				if (pressed) {
					mLocalPlayerRacer->processCommand(Racer::CMD_RIGHT_PRS);
				} else {
					mLocalPlayerRacer->processCommand(Racer::CMD_RIGHT_REL);
				}

				break;

			case KC_UP:

				if (pressed) {
					mLocalPlayerRacer->processCommand(Racer::CMD_ACCEL_PRS);
				} else {
					mLocalPlayerRacer->processCommand(Racer::CMD_ACCEL_REL);
				}
				//mAddThrustForward = pressed;
				break;

			}
		}
	}
}

void Application::playTrackFile(std::string filename) {

	stopPlayingTrack = false;
	quit = false;

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

	mReplayRacer->mPlayedReplay = loadReplayFromFile("replay.xml");

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
	while (!stopPlayingTrack) {

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

			mReplayRacer->pilotStep(stepCount);

			for (unsigned int ii = 0; ii < m_racers.size(); ii++) {
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
}

void Application::doRacerStep(Racer* racer) {

	racer->mRaceTime++;

	if (!mpTrack->mExtent.containsPoint(racer->mPos)) {
		racer->mKilled = true;
	}

	if (racer->mWantReset) {
		mReplay = mLocalPlayerRacer->mReplayCommands;

		racer->mPos = mpTrack->mStartPosition;
		racer->reset();

		// TODO 3: Was hiermit?
		mpRenderer->showKilledInfo(false);
		mpRenderer->showTrackCompletedInfo(false);
	}

	// If the ship is destroyed, reset to the starting position:

	if (racer->mKilled) {

		if (racer == mLocalPlayerRacer) {
			mpRenderer->showKilledInfo(true);
		} else {
			racer->mWantReset = true;
		}

	} else if (racer->mFinish) {

		if (racer == mLocalPlayerRacer) {
			mpRenderer->showTrackCompletedInfo(true);
			writeReplayToFile(mLocalPlayerRacer->mReplayCommands);
		} else {
			racer->mWantReset = true;
		}

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

std::vector<Racer::ReplayEntry> Application::loadReplayFromFile(std::string filename) {

	TiXmlDocument doc;
	doc.LoadFile(filename);

	std::vector<Racer::ReplayEntry> result;

	TiXmlElement* root = doc.FirstChildElement("replay");

	for (TiXmlElement* elem = root->FirstChildElement("action"); elem != NULL;
			elem = elem->NextSiblingElement("action")) {

		Racer::ReplayEntry entry;

		entry.timestamp = atoi(elem->Attribute("time"));
		entry.cmd = (Racer::RacerCommand) atoi(elem->Attribute("cmd"));

		result.push_back(entry);
	}

	return result;
}

void Application::run() {

	watchReplay = true;
	std::string trackFilePath = "tracks/default_track.xml";

	while (!quit) {
		playTrackFile(trackFilePath);
	}
}

void Application::writeReplayToFile(std::vector<Racer::ReplayEntry> replay) {
	std::ofstream myfile;
	myfile.open("replay.xml");

	myfile << "<replay>\n";

	for (unsigned int ii = 0; ii < replay.size(); ii++) {
		Racer::ReplayEntry cmd = replay[ii];

		myfile << "<action time=\"" << cmd.timestamp << "\" cmd=\"" << cmd.cmd << "\"/>\n";
	}

	myfile << "</replay>\n";

	myfile.close();
}
