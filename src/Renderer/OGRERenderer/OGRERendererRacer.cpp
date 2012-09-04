/*
 * OGREVehicle.cpp
 *
 *  Created on: 09.06.2012
 *      Author: sebastian
 */

// TODO 3: Make vehicle/camera rotation speed framerate-independent.
#include "OGRERendererRacer.h"
#include <OgreParticle.h>
#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>

OGRERendererRacer::OGRERendererRacer(Ogre::SceneManager* a_sceneMgr, Racer* a_vehicle, std::string aname) {

	name = aname;

	mSceneManager = a_sceneMgr;
	mpVehicle = a_vehicle;

	mRollAngle = 0;
	mPitchAngle = 0;

	mOrientation.FromAngleAxis(Ogre::Radian(0), Ogre::Vector3(1, 0, 0));

	mVehicleNode = mSceneManager->getRootSceneNode()->createChildSceneNode();

	mVehicleMeshNode = mVehicleNode->createChildSceneNode();
	//mVehicleMeshNode->setScale(0.05,0.05,0.05);

	//Ogre::Entity* entVehicle = mSceneManager->getEntity("Vehicle");
	// Racer model:
	mMeshEntity = mSceneManager->createEntity("Vehicle.mesh");

	mVehicleMeshNode->attachObject(mMeshEntity);

	// TODO 1: Reimplement

	// Set up engine flame particle system:
	mEngineFlameParticleSystem = mSceneManager->createParticleSystem(name + "_flame", "SpaceRoads/EngineFlame");
	Ogre::SceneNode* particleNode = mVehicleMeshNode->createChildSceneNode();
	particleNode->setPosition(0, -1.5, 3);
	particleNode->attachObject((Ogre::ParticleSystem*) mEngineFlameParticleSystem);

	// Set up engine flame particle system:
	mEngineSmokeParticleSystem = mSceneManager->createParticleSystem(name + "_smoke", "SpaceRoads/EngineSmoke");
	particleNode = mVehicleMeshNode->createChildSceneNode();
	particleNode->setPosition(0, -1.5, 3);
	particleNode->attachObject((Ogre::ParticleSystem*) mEngineSmokeParticleSystem);

}

OGRERendererRacer::~OGRERendererRacer() {
	// TODO Auto-generated destructor stub
}

void OGRERendererRacer::update() {

	const cml::vector3d& pos = mpVehicle->mPos;

	mVehicleNode->setPosition(pos[0], pos[1], pos[2]);

	Ogre::Quaternion q;

	const quat& orientation = mpVehicle->getOrientation();

	q.w = orientation.as_vector()[0];
	q.x = orientation.as_vector()[1];
	q.y = orientation.as_vector()[2];
	q.z = orientation.as_vector()[3];

	if (q != mOrientation) {

		Ogre::Quaternion transition = q * mOrientation.Inverse();

		Ogre::Radian a;
		Ogre::Vector3 v;

		transition.ToAngleAxis(a, v);

		if (a.valueDegrees() > 0.1) {

			Ogre::Quaternion t2;

			// TODO 3: Make rotation speed configurable
			t2.FromAngleAxis(a * 0.05, v);

			mOrientation = t2 * mOrientation;
		} else {
			mOrientation = q;
		}
	}

	//################# BEGIN Construct Sideward thrust roll quaternion ################

//	cml::vector3f velDirSidewardComponent = cml::dot(mpVehicle->mDirLeft, mpVehicle->mVelocity) * mpVehicle->mDirLeft;

	if (mpVehicle->mAddThrustLeft || mpVehicle->mAddThrustRight) {
		mRollAngle = -mpVehicle->mThrustSideward * 500;
	} else {
		mRollAngle *= 0.98;
	}

	float dot = cml::dot(mpVehicle->mDirLeft, mpVehicle->mVelocity);
	mRollAngle = -dot * 50;

	Ogre::Quaternion qSidewardThrustRoll(Ogre::Degree(mRollAngle), Ogre::Vector3(0, 0, -1));

	cml::vector3f g = mpVehicle->getGravity();

	cml::vector3f velDirGravityComponent = cml::dot(g, mpVehicle->mVelocity) * g;

	dot = cml::dot(velDirGravityComponent, g);

	if (velDirGravityComponent.length() > 0.000001) {
		mPitchAngle = -cml::sign(dot) * velDirGravityComponent.length() * 500000;
	} else {

		if (mpVehicle->mAddThrustForward && mPitchAngle > -7) {
			mPitchAngle -= 0.1;
		} else {
			mPitchAngle *= 0.95;
		}
	}

	Ogre::Quaternion qForwardThrustPitch(Ogre::Degree(mPitchAngle), Ogre::Vector3(1, 0, 0));

	//################# END Construct Sideward thrust roll quaternion ################

	bool mSidewardThrustRollCamera = false;

	if (mSidewardThrustRollCamera) {
		// Roll both vehicle and camera:
		mVehicleNode->setOrientation(q * qSidewardThrustRoll * qForwardThrustPitch);
	} else {
		// Roll only the vehicle, not the camera:
		mVehicleNode->setOrientation(mOrientation);
		mVehicleMeshNode->setOrientation(qSidewardThrustRoll * qForwardThrustPitch);
	}

	//############# BEGIN Update engine particle emitters ####################

	// TODO 1: Reimplement

	bool emit = !mpVehicle->mKilled && mpVehicle->mThrustForward > 0;

	mEngineFlameParticleSystem->setEmitting(emit);
	mEngineSmokeParticleSystem->setEmitting(emit);

	Ogre::ParticleEmitter* emitter = mEngineFlameParticleSystem->getEmitter(0);
	emitter->setParticleVelocity(mpVehicle->mThrustForward * 6000);
	emitter->setEmissionRate(mpVehicle->mThrustForward * 10000);

	emitter = mEngineSmokeParticleSystem->getEmitter(0);
	emitter->setEmissionRate(mpVehicle->mThrustForward * 7000);

	//############# END Update engine particle emitters ####################
}

