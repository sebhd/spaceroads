/*
 * GameModel.h
 *
 *  Created on: 29.04.2012
 *      Author: sebastian
 */

#ifndef GAMEMODEL_H_
#define GAMEMODEL_H_

#include <vector>
#include "Vehicle.h"
#include "TrackAtoms/TrackAtom.h"
#include "Tracks/AbstractTrack.h"
#include "cml/cml.h"

class Vehicle;
class TrackAtom;
class AbstractTrack;

class GameModel {
public:
	GameModel();
	virtual ~GameModel();

	Vehicle* mpPlayerVehicle;

	AbstractTrack* mpTrack;
};

#endif /* GAMEMODEL_H_ */
