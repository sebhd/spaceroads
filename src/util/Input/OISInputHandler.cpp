/*
 * OISInputHandler.cpp
 *
 *  Created on: 28.04.2012
 *      Author: sebastian
 */

#include "OISInputHandler.h"

#include <iostream>

OISInputHandler::OISInputHandler(std::string windowSize) {

	OIS::ParamList pl;

	//std::string windowHndStr = mpApp->getRenderer()->getWindowSize();

	pl.insert(std::make_pair(std::string("WINDOW"), windowSize));

	mpInputManager = OIS::InputManager::createInputSystem(pl);

	mpKeyboard = static_cast<OIS::Keyboard*>(mpInputManager->createInputObject(OIS::OISKeyboard, true));

	mpKeyboard->setEventCallback(this);

	mpMouse = static_cast<OIS::Mouse*>(mpInputManager->createInputObject(OIS::OISMouse, false));

	addKeyListener(this);

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

	for (unsigned int ii = 0; ii < mOISKeyListeners.size(); ++ii) {
		if (mOISKeyListeners[ii] != this) {
			mOISKeyListeners[ii]->keyPressed(evt);
		}
	}


	bool pressed = true;

	for (unsigned int ii = 0; ii < mListeners.size(); ++ii) {

		mListeners[ii]->handleKeyEvent(int(evt.key), pressed);

		/*
		switch (evt.key) {
		case OIS::KC_A:
			mListeners[ii]->handleKeyEvent(KeyboardEventListener::KEY_A, pressed);
			break;
		case OIS::KC_D:
			mListeners[ii]->handleKeyEvent(KeyboardEventListener::KEY_D, pressed);
			break;
		case OIS::KC_DOWN:
			mListeners[ii]->handleKeyEvent(KeyboardEventListener::KEY_DOWN, pressed);
			break;
		case OIS::KC_E:
			mListeners[ii]->handleKeyEvent(KeyboardEventListener::KEY_E, pressed);
			break;
		case OIS::KC_ESCAPE:
			mListeners[ii]->handleKeyEvent(KeyboardEventListener::KEY_ESCAPE, pressed);
			break;
		case OIS::KC_LEFT:
			mListeners[ii]->handleKeyEvent(KeyboardEventListener::KEY_LEFT, pressed);
			break;
		case OIS::KC_Q:
			mListeners[ii]->handleKeyEvent(KeyboardEventListener::KEY_Q, pressed);
			break;
		case OIS::KC_RIGHT:
			mListeners[ii]->handleKeyEvent(KeyboardEventListener::KEY_RIGHT, pressed);
			break;
		case OIS::KC_SPACE:
			mListeners[ii]->handleKeyEvent(KeyboardEventListener::KEY_SPACE, pressed);
			break;
		case OIS::KC_UP:
			mListeners[ii]->handleKeyEvent(KeyboardEventListener::KEY_UP, pressed);
			break;


		default:
			break;
		}
		*/
	}

	return true;
}

bool OISInputHandler::keyReleased(const OIS::KeyEvent& evt) {


	for (unsigned int ii = 0; ii < mOISKeyListeners.size(); ++ii) {
		if (mOISKeyListeners[ii] != this) {

		mOISKeyListeners[ii]->keyReleased(evt);
		}
	}

	bool pressed = false;

	for (unsigned int ii = 0; ii < mListeners.size(); ++ii) {
		mListeners[ii]->handleKeyEvent(evt.key, pressed);
		/*
		switch (evt.key) {

		case OIS::KC_ESCAPE:
			mListeners[ii]->handleKeyEvent(KeyboardEventListener::KEY_ESCAPE, pressed);
			break;
		case OIS::KC_SPACE:
			mListeners[ii]->handleKeyEvent(KeyboardEventListener::KEY_SPACE, pressed);
			break;

		case OIS::KC_RIGHT:
			mListeners[ii]->handleKeyEvent(KeyboardEventListener::KEY_RIGHT, pressed);
			break;

		case OIS::KC_LEFT:
			mListeners[ii]->handleKeyEvent(KeyboardEventListener::KEY_LEFT, pressed);
			break;

		case OIS::KC_UP:
			mListeners[ii]->handleKeyEvent(KeyboardEventListener::KEY_UP, pressed);
			break;
		case OIS::KC_DOWN:
			mListeners[ii]->handleKeyEvent(KeyboardEventListener::KEY_DOWN, pressed);
			break;
		default:
			break;
		}
		*/
	}

	return true;
}

void OISInputHandler::getInput() {
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
	mOISKeyListeners.push_back(listener);
}
