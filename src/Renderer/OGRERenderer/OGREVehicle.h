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

	Ogre::ParticleSystem* mEngineFlameParticleSystem;
	Ogre::ParticleSystem* mEngineSmokeParticleSystem;

	Ogre::SceneManager* mSceneManager;

	Vehicle* mpVehicle;

protected:
	float mRollAngle;
	float mPitchAngle;
	float mDesiredPitchAngle;

	Ogre::Quaternion mOrientation;
};

#endif /* OGREVEHICLE_H_ */
