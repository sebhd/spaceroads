/*
 * OGREVehicle.cpp
 *
 *  Created on: 09.06.2012
 *      Author: sebastian
 */

// TODO 4: Implement pitching on up/down movement (jumping/falling) like in the original game

#include "OGREVehicle.h"

#include <OgreParticle.h>
#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>
#include "cml/cml.h"

OGREVehicle::OGREVehicle(Ogre::SceneManager* a_sceneMgr, Vehicle* a_vehicle) {

	mpSceneManager = a_sceneMgr;
	mpVehicle = a_vehicle;

	mVehicleRollAngle = 0;
	mVehiclePitchAngle = 0;

	Ogre::Entity* entVehicle = mpSceneManager->createEntity("Vehicle", "Vehicle.mesh");

	mVehicleNode = mpSceneManager->getRootSceneNode()->createChildSceneNode();

	mVehicleMeshNode = mVehicleNode->createChildSceneNode();
	mVehicleMeshNode->attachObject(entVehicle);

	// Set up engine flame particle system:
	mVehicleEngineFlameParticleSystem = mpSceneManager->createParticleSystem("EngineFlame", "SpaceRoads/EngineFlame");
	Ogre::SceneNode* particleNode = mVehicleNode->createChildSceneNode("EngineFlame");
	particleNode->setPosition(0, -1.5, 3);
	particleNode->attachObject((Ogre::ParticleSystem*) mVehicleEngineFlameParticleSystem);

	// Set up engine flame particle system:
	mVehicleEngineSmokeParticleSystem = mpSceneManager->createParticleSystem("EngineSmoke", "SpaceRoads/EngineSmoke");
	particleNode = mVehicleNode->createChildSceneNode("EngineSmoke");
	particleNode->setPosition(0, -1.5, 3);
	particleNode->attachObject((Ogre::ParticleSystem*) mVehicleEngineSmokeParticleSystem);

}

OGREVehicle::~OGREVehicle() {
	// TODO Auto-generated destructor stub
}

void OGREVehicle::update() {

	const cml::vector3d& pos = mpVehicle->getPosition();

	mVehicleNode->setPosition(pos[0], pos[1], pos[2]);

	Ogre::Quaternion q;

	q.FromAngleAxis(Ogre::Radian(10), Ogre::Vector3(1, 0, 0));

	const quat& orientation = mpVehicle->getOrientation();

	q.w = orientation.as_vector()[0];
	q.x = orientation.as_vector()[1];
	q.y = orientation.as_vector()[2];
	q.z = orientation.as_vector()[3];



	//################# BEGIN Construct Sideward thrust roll quaternion ################

	if (mpVehicle->mAddThrustLeft || mpVehicle->mAddThrustRight) {
		mVehicleRollAngle = -mpVehicle->mThrustSideward * 500;
	} else {
		mVehicleRollAngle *= 0.98;
	}

	Ogre::Quaternion qSidewardThrustRoll(Ogre::Degree(mVehicleRollAngle), Ogre::Vector3(0, 0, -1));

	/*
	if (mpVehicle->mJumped) {
		mVehiclePitchAngle = 10;
		mpVehicle->mJumped = false;
	}
*/
	if (mpVehicle->mAddThrustForward && mVehiclePitchAngle > -7) {
		mVehiclePitchAngle -= 0.1;
	} else {
		mVehiclePitchAngle *= 0.95;
	}

	Ogre::Quaternion qForwardThrustPitch(Ogre::Degree(mVehiclePitchAngle), Ogre::Vector3(1, 0, 0));

	//################# END Construct Sideward thrust roll quaternion ################

	bool mSidewardThrustRollCamera = false;

	if (mSidewardThrustRollCamera) {
		// Roll both vehicle and camera:
		mVehicleNode->setOrientation(q * qSidewardThrustRoll * qForwardThrustPitch);
	} else {
		// Roll only the vehicle, not the camera:
		mVehicleNode->setOrientation(q);
		mVehicleMeshNode->setOrientation(qSidewardThrustRoll * qForwardThrustPitch);
	}

	//############# BEGIN Update engine particle emitter ####################

	// Set number of particles:

	mVehicleEngineFlameParticleSystem->setEmitting(mpVehicle->mThrustForward > 0);
	mVehicleEngineSmokeParticleSystem->setEmitting(mpVehicle->mThrustForward > 0);

	Ogre::ParticleEmitter* emitter = mVehicleEngineFlameParticleSystem->getEmitter(0);
	emitter->setParticleVelocity(mpVehicle->mThrustForward * 6000);
	emitter->setEmissionRate(mpVehicle->mThrustForward * 10000);

	emitter = mVehicleEngineSmokeParticleSystem->getEmitter(0);
	emitter->setEmissionRate(mpVehicle->mThrustForward * 7000);

	//############# END Update engine particle emitter ####################
}

