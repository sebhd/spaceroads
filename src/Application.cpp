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

Application::Application() {

	quit = false;
	mpGameModel = new GameModel();
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
		mpGameModel->mpPlayerVehicle->cmd_tryJump(true);
		break;

	case OIS::KC_A:
		cml::quaternion_rotate_about_local_y(mpGameModel->mpPlayerVehicle->mDesiredOrientation, (float) M_PI / 2);
		break;

	case OIS::KC_D:
		cml::quaternion_rotate_about_local_y(mpGameModel->mpPlayerVehicle->mDesiredOrientation, (float) -M_PI / 2);
		break;

	case OIS::KC_W:
		cml::quaternion_rotate_about_local_x(mpGameModel->mpPlayerVehicle->mDesiredOrientation, float(-M_PI/2));
		break;
	case OIS::KC_S:
		cml::quaternion_rotate_about_local_x(mpGameModel->mpPlayerVehicle->mDesiredOrientation, float(M_PI/2));
		break;



	case OIS::KC_Q:
		cml::quaternion_rotate_about_local_z(mpGameModel->mpPlayerVehicle->mDesiredOrientation, float(-M_PI/2));
		break;

	case OIS::KC_E:
		cml::quaternion_rotate_about_local_z(mpGameModel->mpPlayerVehicle->mDesiredOrientation, float(M_PI/2));
		break;

	case OIS::KC_B:
		mpGameModel->mpPlayerVehicle->mVelocity.set(0, 0, 0);
		break;

	case OIS::KC_R:
		mpGameModel->mpPlayerVehicle->reset();
		break;


	case OIS::KC_RIGHT:
		mpGameModel->mpPlayerVehicle->cmd_moveRight(true);
		break;

	case OIS::KC_LEFT:
		mpGameModel->mpPlayerVehicle->cmd_moveLeft(true);
		break;


	case OIS::KC_UP:
		mpGameModel->mpPlayerVehicle->cmd_accelerate(true);
		break;
	case OIS::KC_DOWN:
		mpGameModel->mpPlayerVehicle->cmd_brake(true);
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
		mpGameModel->mpPlayerVehicle->cmd_tryJump(false);
		break;
	case OIS::KC_LEFT:
		mpGameModel->mpPlayerVehicle->cmd_moveLeft(false);
		break;
	case OIS::KC_RIGHT:
		mpGameModel->mpPlayerVehicle->cmd_moveRight(false);
		break;
	case OIS::KC_UP:
		mpGameModel->mpPlayerVehicle->cmd_accelerate(false);
		break;
	case OIS::KC_DOWN:
		mpGameModel->mpPlayerVehicle->cmd_brake(false);
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


	//########### BEGIN The Main Loop! ##########
	while (!quit) {

		gettimeofday(&newTime, NULL);

		long frameTime = newTime.tv_usec - currentTime.tv_usec;
		currentTime = newTime;

		accumulator += frameTime;

		if (accumulator > 20000) {
			accumulator = 20000;
		}

		while (accumulator >= dt) {

			// Process input:
			mpInputHandler->processInput();

			// Do physics / game logic step:
			mpGameModel->mpPlayerVehicle->doPhysicsStep();
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

