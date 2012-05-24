/*
 * OISInputHandler.cpp
 *
 *  Created on: 28.04.2012
 *      Author: sebastian
 */

#include "OISInputHandler.h"

OISInputHandler::OISInputHandler(std::string windowSize) {

	OIS::ParamList pl;

	//std::string windowHndStr = mpApp->getRenderer()->getWindowSize();

	pl.insert(std::make_pair(std::string("WINDOW"), windowSize));

	mpInputManager = OIS::InputManager::createInputSystem(pl);

	mpKeyboard = static_cast<OIS::Keyboard*>(mpInputManager->createInputObject(OIS::OISKeyboard, true));

	mpKeyboard->setEventCallback(this);

	mpMouse = static_cast<OIS::Mouse*>(mpInputManager->createInputObject(OIS::OISMouse, false));

}

OISInputHandler::~OISInputHandler() {

	if (mpInputManager) {
		mpInputManager->destroyInputObject(mpMouse);
		mpInputManager->destroyInputObject(mpKeyboard);

		OIS::InputManager::destroyInputSystem(mpInputManager);
		mpInputManager = 0;
	}

}

bool OISInputHandler::keyPressed(const OIS::KeyEvent& evt) {

	for (unsigned int ii = 0; ii < mKeyListeners.size(); ++ii) {
		mKeyListeners[ii]->keyPressed(evt);
	}
	//mpApp->keyPressed(evt);

	return true;
}

bool OISInputHandler::keyReleased(const OIS::KeyEvent& evt) {

//	mpApp->keyReleased(evt);

	for (unsigned int ii = 0; ii < mKeyListeners.size(); ++ii) {
		mKeyListeners[ii]->keyReleased(evt);
	}

	return true;
}

void OISInputHandler::processInput() {
	// BEGIN ############ Keyboard input #################

	//Need to capture/update each device:
	mpKeyboard->capture();
	mpMouse->capture();

	// END ############ Keyboard input #################

}

void OISInputHandler::handleWindowResizeEvent(int width, int height) {
	const OIS::MouseState &ms = mpMouse->getMouseState();
	ms.width = width;
	ms.height = height;

}

void OISInputHandler::handleWindowClosedEvent() {
	if (mpInputManager) {
		mpInputManager->destroyInputObject(mpMouse);
		mpInputManager->destroyInputObject(mpKeyboard);

		OIS::InputManager::destroyInputSystem(mpInputManager);
		mpInputManager = 0;
	}
}

void OISInputHandler::addKeyListener(OIS::KeyListener* listener) {
	mKeyListeners.push_back(listener);
}
