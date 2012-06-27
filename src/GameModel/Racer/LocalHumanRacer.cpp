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

		/*
		case KC_A:
			if (pressed)
			cmd_rotateDesiredOrientation(1, 1);
			break;

		case KC_D:
			if (pressed)
			cmd_rotateDesiredOrientation(1, -1);
			break;

		case KC_Q:
			if (pressed)
			cmd_rotateDesiredOrientation(2, 1);
			break;

		case KC_E:
			if (pressed)
			cmd_rotateDesiredOrientation(2, -1);
			break;
*/
		case KC_DOWN:
			//mReduceThrustForward = pressed;
			if (pressed) {
				processCommand(CMD_BRAKE_PRS);
			}
			else {
				processCommand(CMD_BRAKE_REL);
			}
			break;


		case KC_LEFT:
			//mAddThrustLeft = pressed;
			if (pressed) {
				processCommand(CMD_LEFT_PRS);
			}
			else {
				processCommand(CMD_LEFT_REL);
			}

			break;

		case KC_RIGHT:
			//mAddThrustRight = pressed;
			if (pressed) {
				processCommand(CMD_RIGHT_PRS);
			}
			else {
				processCommand(CMD_RIGHT_REL);
			}

			break;

		case KC_UP:

			if (pressed) {
				processCommand(CMD_ACCEL_PRS);
			}
			else {
				processCommand(CMD_ACCEL_REL);
			}
			//mAddThrustForward = pressed;
			break;


		default:
			break;
		}
	}
}

void LocalHumanRacer::pilotStep(unsigned long step) {

}
