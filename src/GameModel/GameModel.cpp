/*
 * GameModel.cpp
 *
 *  Created on: 29.04.2012
 *      Author: sebastian
 */

#include "GameModel.h"
#include "Tracks/HardcodedTrack.h"

GameModel::GameModel() {

	// ATTENTION: mTrack *MUST* be instantiated/initialized *BEFORE* mpPlayerVehicle!
	mpTrack = new HardcodedTrack();

	mpPlayerVehicle = new Vehicle(this);
}

GameModel::~GameModel() {
	// TODO Auto-generated destructor stub
}
