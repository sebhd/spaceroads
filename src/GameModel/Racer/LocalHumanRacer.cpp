/*
 * LocalHumanRacer.cpp
 *
 *  Created on: 13.06.2012
 *      Author: sebastian
 */

#include "LocalHumanRacer.h"

LocalHumanRacer::LocalHumanRacer() {
	// TODO Auto-generated constructor stub

}

LocalHumanRacer::~LocalHumanRacer() {
	// TODO Auto-generated destructor stub
}

void LocalHumanRacer::handleKeyEvent(Key key, bool pressed) {

	switch(key) {
	case KEY_SPACE:
		cmd_spacebar(pressed);
		break;

	case KEY_LEFT:
		mAddThrustLeft = pressed;
		break;

	case KEY_RIGHT:
		mAddThrustRight = pressed;
		break;

	case KEY_UP:
		mAddThrustForward = pressed;
		break;

	case KEY_DOWN:
		mReduceThrustForward = pressed;
		break;

	default:
			break;
	}

}
