/*
 * OGREVehicle.h
 *
 *  Created on: 09.06.2012
 *      Author: sebastian
 */

#ifndef OGREVEHICLE_H_
#define OGREVEHICLE_H_

#include <Ogre.h>
#include <string>
#include "../../GameModel/Racer/Racer.h"


class OGRERendererRacer {
public:
	OGRERendererRacer(Ogre::SceneManager*, Racer*, std::string);
	virtual ~OGRERendererRacer();

	void update();

	Ogre::SceneNode* mVehicleNode;
	Ogre::SceneNode* mVehicleMeshNode;

	Ogre::ParticleSystem* mEngineFlameParticleSystem;
	Ogre::ParticleSystem* mEngineSmokeParticleSystem;

	Ogre::SceneManager* mSceneManager;

	Ogre::Entity* mMeshEntity;
	Racer* mpVehicle;

protected:
	float mRollAngle;
	float mPitchAngle;
	std::string name;


	Ogre::Quaternion mOrientation;
};

#endif /* OGREVEHICLE_H_ */
