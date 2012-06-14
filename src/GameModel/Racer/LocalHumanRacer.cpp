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

void LocalHumanRacer::handleKeyEvent(int key, bool pressed) {

	if (key == KC_SPACE) {
		cmd_spacebar(pressed);
	}

	if (!mKilled) {
		switch (key) {


		case KC_A:
			if (pressed)
			cmd_rotateDesiredOrientation(1, 1);
			break;

		case KC_D:
			if (pressed)
			cmd_rotateDesiredOrientation(1, -1);
			break;

		case KC_DOWN:
			mReduceThrustForward = pressed;
			break;

		case KC_E:
			if (pressed)
			cmd_rotateDesiredOrientation(2, -1);
			break;

		case KC_LEFT:
			mAddThrustLeft = pressed;
			break;
		case KC_Q:
			if (pressed)
			cmd_rotateDesiredOrientation(2, 1);
			break;

		case KC_RIGHT:
			mAddThrustRight = pressed;
			break;

		case KC_UP:
			mAddThrustForward = pressed;
			break;


		default:
			break;
		}
	}
}
