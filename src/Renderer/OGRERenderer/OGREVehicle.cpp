/*
 * OGREVehicle.cpp
 *
 *  Created on: 09.06.2012
 *      Author: sebastian
 */

#include "OGREVehicle.h"
#include <OgreParticle.h>
#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>

OGREVehicle::OGREVehicle(Ogre::SceneManager* a_sceneMgr, Vehicle* a_vehicle) {

	mSceneManager = a_sceneMgr;
	mpVehicle = a_vehicle;

	mRollAngle = 0;
	mPitchAngle = 0;

	Ogre::Entity* entVehicle = mSceneManager->createEntity("Vehicle", "Vehicle.mesh");

	mOrientation.FromAngleAxis(Ogre::Radian(0), Ogre::Vector3(1, 0, 0));

	mVehicleNode = mSceneManager->getRootSceneNode()->createChildSceneNode();

	mVehicleMeshNode = mVehicleNode->createChildSceneNode();

	mVehicleMeshNode->attachObject(entVehicle);

	// Set up engine flame particle system:
	mEngineFlameParticleSystem = mSceneManager->createParticleSystem("EngineFlame", "SpaceRoads/EngineFlame");
	Ogre::SceneNode* particleNode = mVehicleNode->createChildSceneNode("EngineFlame");
	particleNode->setPosition(0, -1.5, 3);
	particleNode->attachObject((Ogre::ParticleSystem*) mEngineFlameParticleSystem);

	// Set up engine flame particle system:
	mEngineSmokeParticleSystem = mSceneManager->createParticleSystem("EngineSmoke", "SpaceRoads/EngineSmoke");
	particleNode = mVehicleNode->createChildSceneNode("EngineSmoke");
	particleNode->setPosition(0, -1.5, 3);
	particleNode->attachObject((Ogre::ParticleSystem*) mEngineSmokeParticleSystem);

}

OGREVehicle::~OGREVehicle() {
	// TODO Auto-generated destructor stub
}

void OGREVehicle::update() {

	const cml::vector3d& pos = mpVehicle->getPosition();

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

			t2.FromAngleAxis(a * 0.03, v);

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

		if (dot < 0) {
			mDesiredPitchAngle = 17;
		} else {
			mDesiredPitchAngle = -17;
		}

		mPitchAngle += (mDesiredPitchAngle - mPitchAngle) * 0.1;
	} else {
		mDesiredPitchAngle = 0;

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
		//mVehicleNode->setOrientation(q);
		mVehicleMeshNode->setOrientation(qSidewardThrustRoll * qForwardThrustPitch);
	}

	//############# BEGIN Update engine particle emitters ####################

	bool emit = mpVehicle->mThrustForward > 0;

	mEngineFlameParticleSystem->setEmitting(emit);
	mEngineSmokeParticleSystem->setEmitting(emit);

	Ogre::ParticleEmitter* emitter = mEngineFlameParticleSystem->getEmitter(0);
	emitter->setParticleVelocity(mpVehicle->mThrustForward * 6000);
	emitter->setEmissionRate(mpVehicle->mThrustForward * 10000);

	emitter = mEngineSmokeParticleSystem->getEmitter(0);
	emitter->setEmissionRate(mpVehicle->mThrustForward * 7000);

	//############# END Update engine particle emitters ####################
}

