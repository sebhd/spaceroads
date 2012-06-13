/*
 * LocalHumanPilot.cpp
 *
 *  Created on: 13.06.2012
 *      Author: sebastian
 */

#include "LocalHumanPilot.h"



LocalHumanPilot::LocalHumanPilot() : Pilot() {
	// TODO Auto-generated constructor stub

	mVehicle = NULL;

}

LocalHumanPilot::~LocalHumanPilot() {
	// TODO Auto-generated destructor stub
}

void LocalHumanPilot::step() {


	//mVehicle->cmd_spacebar(true);
}



void LocalHumanPilot::handleKeyEvent(Key key, bool pressed) {


	switch (key) {

	case KEY_SPACE:
		mVehicle->cmd_spacebar(pressed);
		break;

		/*
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
*/

	case KEY_RIGHT:
		mVehicle->mAddThrustRight = pressed;
		break;

	case KEY_LEFT:
		mVehicle->mAddThrustLeft = pressed;
		break;

	case KEY_UP:
		mVehicle->mAddThrustForward = pressed;
		break;
	case KEY_DOWN:
		mVehicle->mReduceThrustForward = pressed;
		break;
	default:
		break;
	}
}
