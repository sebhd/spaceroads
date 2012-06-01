/*
 * Application.cpp
 *
 *  Created on: 28.04.2012
 *      Author: sebastian
 */

#include "Application.h"
#include <iostream>
#include <ctime>
#include <sys/time.h>

#include "Renderer/OgreRenderer.h"
#include "GameModel/Tracks/HardcodedTrack.h"
#include "GameModel/Tracks/RandomTrack.h"

Application::Application() {

	quit = false;

	mpTrack = new HardcodedTrack(this);
	//mpTrack = new RandomTrack(this);

	mpPlayerVehicle = new Vehicle(mpTrack);
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
		//mpPlayerVehicle->cmd_tryJump(true);
		mpPlayerVehicle->mTryJump = true;
		break;


	case OIS::KC_A:
		mpPlayerVehicle->cmd_rotateDesiredOrientation(1,1);
		break;

	case OIS::KC_D:
		mpPlayerVehicle->cmd_rotateDesiredOrientation(1,-1);
		break;

	case OIS::KC_W:
		mpPlayerVehicle->cmd_rotateDesiredOrientation(0,-1);
		break;

	case OIS::KC_S:
		mpPlayerVehicle->cmd_rotateDesiredOrientation(0,1);
		break;

	case OIS::KC_Q:
		mpPlayerVehicle->cmd_rotateDesiredOrientation(2,1);
		break;

	case OIS::KC_E:
		mpPlayerVehicle->cmd_rotateDesiredOrientation(2,-1);
		break;

	case OIS::KC_R:
		mpPlayerVehicle->reset();
		break;


	case OIS::KC_RIGHT:
		//mpPlayerVehicle->cmd_moveRight(true);
		mpPlayerVehicle->mDoThrustRight = true;
		break;

	case OIS::KC_LEFT:
		//mpPlayerVehicle->cmd_moveLeft(true);
		mpPlayerVehicle->mDoThrustLeft = true;
		break;


	case OIS::KC_UP:
		//mpPlayerVehicle->cmd_accelerate(true);
		mpPlayerVehicle->mDoThrustForward = true;
		break;
	case OIS::KC_DOWN:
		mpPlayerVehicle->mDoBrake = true;
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
		//mpPlayerVehicle->cmd_tryJump(false);
		mpPlayerVehicle->mTryJump = false;
		break;
	case OIS::KC_LEFT:
		//mpPlayerVehicle->cmd_moveLeft(false);
		mpPlayerVehicle->mDoThrustLeft = false;
		break;
	case OIS::KC_RIGHT:
		//mpPlayerVehicle->cmd_moveRight(false);
		mpPlayerVehicle->mDoThrustRight = false;
		break;
	case OIS::KC_UP:
		//mpPlayerVehicle->cmd_accelerate(false);
		mpPlayerVehicle->mDoThrustForward = false;
		break;
	case OIS::KC_DOWN:
		//mpPlayerVehicle->cmd_brake(false);
		mpPlayerVehicle->mDoBrake = false;
		break;
	default:
		break;
	}

	return true;
}

void Application::run() {

	mpRenderer = new OgreRenderer(this);

	mpRenderer->init();

	mpInputHandler = new OISInputHandler(getRenderer()->getWindowSize());

	mpInputHandler->addKeyListener(this);


	timeval currentTime, newTime;

	gettimeofday(&currentTime, NULL);
	unsigned long accumulator = 0;

	// 1/100 sec. simulation frequency
	unsigned int dt = 5000;

	// TODO 3: Understand performance problem on Eray's Macbook. Might have to do with the game speed timing code.


	// TODO 4: Make simulation step frequency configurable by introducing a time factor multiplicator in Vehicle::step() and...
	// all other places where it matters.

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

		while (accumulator >= dt) {

			// Process input:
			mpInputHandler->processInput();

			// Do track step:
			mpTrack->step();

			// Do physics / game logic step:
			mpPlayerVehicle->step();

			accumulator -= dt;
		}

		// Render next frame:
		if (!mpRenderer->renderOneFrame()) {
			//	return false;
			break;
		}
	}
	//########### END The Main Loop! ##########
}

AbstractRenderer* Application::getRenderer() {
	return mpRenderer;
}

bool Application::handleFrameRenderingQueuedEvent() {

	return true;
}

