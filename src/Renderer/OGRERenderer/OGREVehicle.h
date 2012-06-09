/*
 * OGREVehicle.h
 *
 *  Created on: 09.06.2012
 *      Author: sebastian
 */

#ifndef OGREVEHICLE_H_
#define OGREVEHICLE_H_

#include <Ogre.h>
#include "../../GameModel/Vehicle.h"

class OGREVehicle {
public:
	OGREVehicle(Ogre::SceneManager*, Vehicle*);
	virtual ~OGREVehicle();

	void update();


	Ogre::SceneNode* mVehicleNode;
	Ogre::SceneNode* mVehicleMeshNode;

	float mVehicleRollAngle;
	float mVehiclePitchAngle;

	Ogre::ParticleSystem* mVehicleEngineFlameParticleSystem;
	Ogre::ParticleSystem* mVehicleEngineSmokeParticleSystem;

	Ogre::SceneManager* mpSceneManager;

	Vehicle* mpVehicle;
};

#endif /* OGREVEHICLE_H_ */
