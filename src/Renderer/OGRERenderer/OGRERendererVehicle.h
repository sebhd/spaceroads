/*
 * OGREVehicle.h
 *
 *  Created on: 09.06.2012
 *      Author: sebastian
 */

#ifndef OGREVEHICLE_H_
#define OGREVEHICLE_H_

#include <Ogre.h>
#include "../../GameModel/Racer/Racer.h"

class OGRERendererVehicle {
public:
	OGRERendererVehicle(Ogre::SceneManager*, Racer*);
	virtual ~OGRERendererVehicle();

	void update();

	Ogre::SceneNode* mVehicleNode;
	Ogre::SceneNode* mVehicleMeshNode;

	Ogre::ParticleSystem* mEngineFlameParticleSystem;
	Ogre::ParticleSystem* mEngineSmokeParticleSystem;

	Ogre::SceneManager* mSceneManager;

	Racer* mpVehicle;

protected:
	float mRollAngle;
	float mPitchAngle;


	Ogre::Quaternion mOrientation;
};

#endif /* OGREVEHICLE_H_ */
